#include "stdio_private.h"

__cgc_FILE _stdin = {
    .fd = STDIN,
    .rw = F_READ,
    .idx = 0,
    .length = 0
};
__cgc_FILE _stdout = {
    .fd = STDOUT,
    .rw = F_WRITE,
    .idx = -1,
    .length = 0
};
__cgc_FILE _stderr = {
    .fd = STDERR,
    .rw = F_WRITE,
    .idx = -1,
    .length = 0
};

__cgc_FILE * const stdin = &_stdin;
__cgc_FILE * const stdout = &_stdout;
__cgc_FILE * const stderr = &_stderr;
