/*
 * Author: Andrew Wesie <andrew.wesie@kapricasecurity.com>
 *
 * Copyright (c) 2014 Kaprica Security, Inc.
 *
 * Permission is hereby granted, __free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "endian.h"
#include "usb.h"

typedef int (*cmd_handler_t) (usb_t *self, void *data);

typedef struct {
    uint32_t code;
    cmd_handler_t handler;
} command_t;

/*
 * hard-code information for one device with one interface
 */
static const device_desc_t device_desc = {
    .bLength = sizeof(device_desc_t),
    .bDescriptorType = 1,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = 64,
    .idVendor = 0x6666,
    .idProduct = 0xDEAD,
    .bcdDevice = 0x0100,
    .iManufacturer = 0,
    .iProduct = 0,
    .iSerialNumber = 0,
    .bNumConfigurations = 1
};

static const config_desc_t config_desc = {
    .bLength = sizeof(config_desc_t),
    .bDescriptorType = 2,
    .wTotalLength = sizeof(config_desc_t) + sizeof(intf_desc_t) + 2 * sizeof(ep_desc_t),
    .bNumInterfaces = 1,
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = 0x80, /* bit 7 must be set */
    .bMaxPower = 50
};

static const intf_desc_t intf_desc = {
    .bLength = sizeof(intf_desc_t),
    .bDescriptorType = 4,
    .bInterfaceNumber = 0,
    .bAlternateSetting = 0,
    .bNumEndpoints = 2,
    .bInterfaceClass = 0x08,
    .bInterfaceSubClass = 0x06,
    .bInterfaceProtocol = 0x50,
    .iInterface = 0
};

static const ep_desc_t ep_desc[2] = {
    {
        .bLength = sizeof(ep_desc_t),
        .bDescriptorType = 5,
        .bEndpointAddress = 0x81,
        .bmAttributes = 0x02,
        .wMaxPacketSize = 512,
        .bInterval = 0
    },
    {
        .bLength = sizeof(ep_desc_t),
        .bDescriptorType = 5,
        .bEndpointAddress = 0x02,
        .bmAttributes = 0x02,
        .wMaxPacketSize = 512,
        .bInterval = 0
    }
};

static devinfo_t device_info = {
    .path = "/sys/devices/pci0000:00/0000:00:1d.1/usb1/1-1",
    .busid = "1-1",
    .busnum = __htobe32(1),
    .devnum = __htobe32(1),
    .speed = __htobe32(3), /* USB_SPEED_HIGH */
    .vendor = __htobe16(0x6666),
    .product = __htobe16(0xDEAD),
    .device = __htobe16(0x0100),
    .dev_class = 0x00,
    .dev_subclass = 0x00,
    .dev_proto = 0x00,
    .current_config = 0,
    .num_configs = 1,
    .num_intf = 1
};

static const intf_t device_intf = {
    .intf_class = 0x08, /* Mass Storage Class */
    .intf_subclass = 0x06, /* SCSI transparent command set */
    .intf_proto = 0x50 /* Bulk Only Transport Protocol */
};

static void fill_submit_reply(submit_rep_t *rep, urb_t *urb, uint32_t status, uint32_t length)
{
    rep->hdr.command = __htobe32(SUBMIT_REP);
    rep->hdr.seqnum = __htobe32(urb->seqnum);
    rep->hdr.devid = __htobe32(urb->devid);
    rep->hdr.direction = __htobe32(urb->direction);
    rep->hdr.endpoint = __htobe32(urb->endpoint);
    rep->length = __htobe32(length);
    rep->status = __htobe32(status);
    rep->start_frame = __htobe32(0);
    rep->number_of_packets = __htobe32(0);
    rep->error_length = __htobe32(0);
    __memset(rep->setup, 0, sizeof(rep->setup));
}

static void send_config_rep(usb_t *self, urb_t *urb)
{
    uint8_t data[config_desc.wTotalLength];
    __memcpy(&data[0], &config_desc, sizeof(config_desc));
    __memcpy(&data[sizeof(config_desc)], &intf_desc, sizeof(intf_desc));
    __memcpy(&data[sizeof(config_desc)+sizeof(intf_desc)], &ep_desc, sizeof(ep_desc));

    uint32_t length = urb->length;
    if (config_desc.wTotalLength < length)
        length = config_desc.wTotalLength;
    usb_send_reply(self, urb, 0, length);
    self->send(data, length);
}

static int handle_ep0(usb_t *self, urb_t *urb)
{
#ifdef DEBUG
    fprintf(stderr, "setup %02x %02x %02x %02x %02x %02x %02x %02x\n",
        urb->setup[0], urb->setup[1], urb->setup[2], urb->setup[3],
        urb->setup[4], urb->setup[5], urb->setup[6], urb->setup[7]);
#endif

    if (urb->setup[0] == 0x80)
    {
        if (urb->setup[1] == 0x06) /* GET_DESCRIPTOR */
        {
            if (urb->setup[3] == 0x01) /* Device descriptor */
            {
                uint32_t length = urb->length;
                if (device_desc.bLength < length)
                    length = device_desc.bLength;
                usb_send_reply(self, urb, 0, length);
                self->send(&device_desc, length);
                return 1;
            }
            if (urb->setup[3] == 0x02) /* Config descriptor */
            {
                send_config_rep(self, urb);
                return 1;
            }
        }
    }
    else if (urb->setup[0] == 0x00)
    {
        if (urb->setup[1] == 0x09) /* SET_CONFIGURATION */
        {
            if (urb->setup[2] == config_desc.bConfigurationValue)
            {
                usb_send_reply(self, urb, 0, 0);
                return 1;
            }
        }
    }
    else if ((urb->setup[0] & 0x60) == 0x20)
    {
        return msc_handle_ep0(self, urb);
    }
    return 0;
}

static int handle_devlist(usb_t *self, metadata_hdr_t *hdr)
{
    devlist_rep_t rep;
    __memset(&rep, 0, sizeof(rep));

    rep.hdr.version = __htobe16(VERSION_CODE);
    rep.hdr.command = __htobe16(DEVLIST_REP);
    rep.hdr.status = __htobe16(0);
    /*
     * hard-coded to 1 USB Mass Storage device
     */
    rep.length = __htobe32(1);

    self->send(&rep, sizeof(rep));
    self->send(&device_info, sizeof(device_info));
    self->send(&device_intf, sizeof(device_intf));
    return 1;
}

static int handle_import(usb_t *self, metadata_hdr_t *hdr)
{
    import_req_t req;
    import_rep_t rep;

    if (self->recv(&req, sizeof(req)) != sizeof(req))
        return 0;

    if (memcmp(req.busid, device_info.busid, sizeof(req.busid)) == 0)
    {
        rep.hdr.version = __htobe16(VERSION_CODE);
        rep.hdr.command = __htobe16(IMPORT_REP);
        rep.hdr.status = __htobe16(0);
        rep.device = device_info;

        if (msc_init(&self->msc))
            self->attached = 1;

        self->send(&rep, sizeof(rep));
    }
    else
    {
        rep.hdr.version = __htobe16(VERSION_CODE);
        rep.hdr.command = __htobe16(IMPORT_REP);
        rep.hdr.status = __htobe32(1);

        self->send(&rep.hdr, sizeof(rep.hdr));
    }

    return 1;
}

static int handle_submit(usb_t *self, data_hdr_t *hdr)
{
    submit_req_t req;
    urb_t *urb;
    urb_t small_urb;

    if (self->recv(&req, sizeof(req)) != sizeof(req))
        return 0;

    if (__be32toh(req.length) >= 0x80000000)
        return 0;

    if (__be32toh(hdr->direction) == DIR_OUT)
        urb = __malloc(sizeof(urb_t) + __be32toh(req.length));
    else
        urb = &small_urb;
    if (urb == NULL)
        return 0;

    urb->devid = __be32toh(hdr->devid);
    urb->seqnum = __be32toh(hdr->seqnum);
    urb->direction = __be32toh(hdr->direction);
    urb->endpoint = __be32toh(hdr->endpoint);
    urb->flags = __be32toh(req.flags);
    urb->length = __be32toh(req.length);
    urb->interval = __be32toh(req.interval);
    __memcpy(urb->setup, req.setup, sizeof(urb->setup));
    if (urb->direction == DIR_OUT && self->recv(urb->data, urb->length) != urb->length)
    {
        __free(urb);
        return 0;
    }

    if (urb->endpoint == 0)
        return handle_ep0(self, urb);
    else
        return msc_handle_urb(self, urb);
}

static int handle_unlink(usb_t *self, data_hdr_t *hdr)
{
    unlink_req_t req;

    if (self->recv(&req, sizeof(req)) != sizeof(req))
        return 0;
    return 1;
}

static const command_t commands[] = {
#define COMMAND(data_or_metadata,code,handler) {(!!(data_or_metadata) << 31)|code, (cmd_handler_t)handler}
    COMMAND(1, DEVLIST_REQ, handle_devlist),
    COMMAND(1, IMPORT_REQ, handle_import),
    COMMAND(0, SUBMIT_REQ, handle_submit),
    COMMAND(0, UNLINK_REQ, handle_unlink),
    {0, NULL}
};

static const command_t *lookup_command(uint32_t cmd)
{
    const command_t *command;
    for (command = &commands[0]; command->handler != NULL; command++)
        if (command->code == cmd)
            return command;
    return NULL;
}

int usb_handle_packet(usb_t *self)
{
    uint32_t cmd;
    if (self->attached == 0)
    {
        /* only support metadata commands */
        metadata_hdr_t hdr;
        if (self->recv(&hdr, sizeof(hdr)) != sizeof(hdr))
            return 0;
        if (__be16toh(hdr.version) != VERSION_CODE)
            return 0;
        const command_t *command = lookup_command((1<<31) | __be16toh(hdr.command));
        if (command == NULL)
            return 0;
        return command->handler(self, &hdr);
    }
    else
    {
        /* only support data commands */
        data_hdr_t hdr;
        if (self->recv(&hdr, sizeof(hdr)) != sizeof(hdr))
            return 0;
        const command_t *command = lookup_command(__be32toh(hdr.command));
        if (command == NULL)
            return 0;
        return command->handler(self, &hdr);
    }
}

int usb_send_reply(usb_t *self, urb_t *urb, uint32_t status, uint32_t length)
{
    submit_rep_t rep;
    if (length < urb->length && (urb->flags & 1))
        status = -121; /* -EREMOTEIO */
    fill_submit_reply(&rep, urb, status, length);
    self->send(&rep, sizeof(rep));
    return 1;
}

int usb_init(usb_t *self)
{
    /* randomly generate a busid for the device */
    uint8_t id;
    size_t bytes;
    random(&id, 1, &bytes);
    id = id % 100;

    int busnum = id / 10;
    int devnum = id % 10;
    device_info.busnum = __htobe32(busnum);
    device_info.devnum = __htobe32(devnum);
    __memset(device_info.busid, 0, sizeof(device_info.busid));
    device_info.busid[0] = busnum + '0';
    device_info.busid[1] = '-';
    device_info.busid[2] = devnum + '0';

    self->attached = 0;
    return 1;
}
