#include<libcgc.h>


int __attribute__((fastcall)) main(int secret_page_i, char *unused[]) {
    int x;
    size_t size;
    int ret;

    receive(STDIN, &x, sizeof(x), &size);

    switch(x) {

    case 0xdeadbeaf:
        ret = 1234;
        break;
    case 2:
        ret = 2345;
        break;
    case 3:
        ret = 3456;
        break;
    default:
        ret = 33;
        break;
    }

    ret = ret + 1;
    return ret;
}
