#include<libcgc.h>

#define BUG(addr) do {				\
    *((int *)addr) = 0xdeadbeaf;		\
      } while (0)

#define dummy() do {				\
    if (x > 0xdeadbeef) {			\
      if (y <= 0xdeadbeef) {			\
	x = x + y + 1;				\
      } else {					\
	y = x + y + 1;				\
      }						\
    } else {					\
      if (y > 0xdeadbeef) {			\
	x = x - y + 1;				\
      } else {					\
	y = x - y + 1;				\
      }						\
    }						\
 } while(0)

#define ABCD (0x41424344)
#define BCDE (0x42434445)
#define CDEF (0x43444546)
#define DEFG (0x44454647)
#define EFGH (0x45464748)
#define FGHI (0x56574849)

int __attribute__((fastcall)) main(int secret_page_i, char *unused[]) {
    int x;
    size_t size;
    int ret;
    int y;

    receive(STDIN, &x, sizeof(x), &size);
    receive(STDIN, &y, sizeof(y), &size);

    switch(x) {

    case ABCD:
        ret = 1234;

        switch (y) {
	case DEFG:
	  dummy();
	  BUG(DEFG);
	  ret = ret + 2;
	  break;
	case EFGH:
	  dummy();
	  BUG(EFGH);
	  ret = ret + 3;
	  break;
	case FGHI:
	  dummy();
	  BUG(FGHI);
	  ret = ret + 6;
	  break;
	default:
	  ret = ret + 9;
        }

        break;
    case BCDE:
      ret = 2345;
      switch (y) {
      case DEFG:
	dummy();
	BUG(DEFG);
	ret = ret + 3;
	break;
      case EFGH:
	dummy();
	BUG(EFGH);
	ret = ret + 4;
	break;
      case FGHI:
	dummy();
	BUG(FGHI);
	ret = ret + 7;
	break;
      default:
	ret = ret + 10;
      }

      break;
    case CDEF:
      ret = 3456;
      switch (y) {
      case DEFG:
	dummy();
	BUG(DEFG);
	ret = ret + 5;
	break;
      case EFGH:
	dummy();
	BUG(EFGH);
	ret = ret + 8;
	break;
      case FGHI:
	dummy();
	BUG(FGHI);
	ret = ret + 2;
	break;
      default:
	ret = ret + 1;
      }
        break;
    default:
      ret = 33;
      break;
    }

    ret = ret + 1;
    return ret;
}
