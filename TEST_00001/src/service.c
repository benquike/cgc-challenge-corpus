#include<libcgc.h>

size_t printf(const char *format, ...);

size_t receive_line(int fd, char *buf, size_t size);

size_t receive_line(int fd, char *buf, size_t size) {
    char c;
    size_t total = 0;
    size_t read = 0;

    while (1) {
        if (receive(fd, &c, 1, &read) != 0) {
            _terminate(1);
        }

        if (read == 0 || c == '\n') {
            break;
        }

        if (total >= size) {
            return 0;
        }

        buf[total++] = c;
    }

    return total;
}


int __attribute__((fastcall)) main(int secret_page_i, char *unused[]) {
    char buf[1024];
    size_t size;


    size = receive_line(STDIN, buf, sizeof(buf) - 1);
    if (size == 0)
        _terminate(1);

    buf[size] = 0;

    if (buf[0] == 'A' && buf[1] == 'B') {
#ifndef PATCHED
      *((int *) 0xdeadbeaf) = 0;
#endif
    }

    return 0;
}
