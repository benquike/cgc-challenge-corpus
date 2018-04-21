#include<libcgc.h>

int __attribute__((fastcall)) main(int secret_page_i, char *unused[]) {
    int x, y;
    size_t read;

    receive(STDIN, &x, sizeof(x), &read);
    receive(STDIN, &y, sizeof(y), &read);

    if (x == 0x12345678) {

	if (y == 0xdeadbeaf) {

#ifndef PATCHED
	  *((int *) 0xdeadbeaf) = 0;
#endif

	}
    }

    return 0;
}
