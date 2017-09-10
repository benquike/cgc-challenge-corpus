/*
 * Copyright (c) 2015 Kaprica Security, Inc.
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

#include <libcgc.h>
#include <stdio.h>
#include <string.h>

#include "sc.h"

const char *secret = (const char*) 0x4347C000;

void print_menu()
{
    __printf("1. Compress\n");
    __printf("2. Decompress\n");
    __printf("3. Quit\n");
}

void handle_compress()
{
    int i;
    unsigned char buf[95], *out = NULL;
    unsigned char data[MAX_DATA_SIZE];
    size_t outlen = 0;
    sc_obj_t *sc = NULL;

    __printf("Key?\n");
    if (__fread(buf, sizeof(buf), stdin) != sizeof(buf))
        goto fail;

    __memset(data, 0, sizeof(data));
    for (i = 0; i < sizeof(buf); ++i)
    {
        if (buf[i] < 32 || buf[i] > 126)
            goto fail;
        if (data[buf[i]]++ > 0)
            goto fail;
    }

    __printf("Data?\n");
    __memset(data, 0, sizeof(data));
    __cgc_fflush(stdout);
    if (__cgc_freaduntil((char *) data, sizeof(data), '\0', stdin) < 0)
        goto fail;

    sc = sc_new(buf);
    sc->data = data;
    sc->data_len = __strlen((char *)data);
    if (sc_scompress(sc, &out, &outlen) < 0)
        goto fail;
    __printf("Original Size: %d\n", sc->data_len);
    __printf("Compressed Size: %d (%d%%)\n", outlen, ((int) ((outlen / (sc->data_len * 1.0)) * 100)));
    __printf("Compressed Data: ");
    for (i = 0; i < outlen && i < 32; ++i)
        __printf("%02X", out[i]);
    __printf("\n");

    goto done;

fail:
    __printf("error.\n");
done:
    if (sc)
        __free(sc);
    if (out)
        __free(out);
}

void handle_decompress()
{
    int i;
    unsigned char buf[95], *out = NULL;
    unsigned char data[MAX_DATA_SIZE];
    size_t outlen = 0, len = 0;
    sc_obj_t *sc = NULL;

    __printf("Key?\n");
    if (__fread(buf, sizeof(buf), stdin) != sizeof(buf))
        goto fail;

    __memset(data, 0, sizeof(data));
    for (i = 0; i < sizeof(buf); ++i)
    {
        if (buf[i] < 32 || buf[i] > 126)
            goto fail;
        if (data[buf[i]]++ > 0)
            goto fail;
    }

    __printf("Length?\n");
    __memset(data, 0, sizeof(data));
    __cgc_fflush(stdout);
    if (__cgc_freaduntil((char *) data, sizeof(data), '\n', stdin) < 0)
        goto fail;
    len = __strtoul((char *) data, NULL, 10);
    if (len > MAX_DATA_SIZE)
        goto fail;

    __printf("Data?\n");
    __memset(data, 0, sizeof(data));
    if (__fread((char *) data, len, stdin) < 0)
        goto fail;

    sc = sc_new(buf);
    sc->data = data;
    sc->data_len = len;
    if (sc_sdecompress(sc, &out, &outlen) < 0)
        goto fail;
    __printf("Compressed Size: %d\n", sc->data_len);
    __printf("Original Size: %d\n", __strlen((char *) out));
    __printf("Original Data: ");
    __fwrite(out, outlen, stdout);
    __fwrite("\n", 1, stdout);
    goto done;

fail:
    __printf("error.\n");
done:
    if (sc)
        __free(sc);
    if (out)
        __free(out);
}

int main()
{
    char buf[8];
    int i;
    __cgc_fbuffered(stdin, 1);
    for (i = 0; i < 8; i += 2)
        __printf("%02X", secret[i] & 0xFF);
    __printf("\n");
    while(1)
    {
        print_menu();
        __cgc_fflush(stdout);
        if (__cgc_freaduntil(buf, sizeof(buf), '\n', stdin) < 0)
            return -1;
        switch(__strtoul(buf, NULL, 10))
        {
            case 1:
                handle_compress();
                break;
            case 2:
                handle_decompress();
                break;
            case 3:
                __printf("Bye.\n");
                __cgc_fflush(stdout);
                __exit(0);
                break;
            default:
                __printf("Invalid menu.\n");
                break;
        }
    }
    return 0;
}
