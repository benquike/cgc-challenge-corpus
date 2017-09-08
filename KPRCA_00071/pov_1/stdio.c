#include "stdio_private.h"

__FILE _stdin = {
    .fd = STDIN,
    .rw = F_READ,
    .idx = 0,
    .length = 0
};
__FILE _stdout = {
    .fd = STDOUT,
    .rw = F_WRITE,
    .idx = -1,
    .length = 0
};
__FILE _stderr = {
    .fd = STDERR,
    .rw = F_WRITE,
    .idx = -1,
    .length = 0
};

__FILE * const stdin = &_stdin;
__FILE * const stdout = &_stdout;
__FILE * const stderr = &_stderr;
