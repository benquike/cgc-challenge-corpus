#include<libcgc.h>

int __attribute__((fastcall)) main(int secret_page_i, char *unused[]) {
  int x;
  size_t read;

  receive(STDIN, &x, sizeof(x), &read);

  if (x == 0xdeadbeaf) {

#ifndef PATCHED
    *((int *) 0xdeadbeaf) = 0;
#endif

  }

  return 0;
}
