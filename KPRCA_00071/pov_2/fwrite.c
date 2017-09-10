#include <libpov.h>
#include "stdio_private.h"

ssize_t __cgc_fwrite(const void *ptr, size_t size, __cgc_FILE *stream)
{
    const char *buf = ptr;
    size_t idx = 0, tx;

    if (stream->idx == INVALID_IDX)
    {
unbuffered:
        if (transmit(stream->fd, ptr, size, &tx) != 0 || tx == 0)
            return -1;
        return tx;
    }
    else
    {
        if (size >= sizeof(stream->buffer))
        {
            __cgc_fflush(stream);
            goto unbuffered;
        }

        if (stream->length + size >= sizeof(stream->buffer))
        {
            tx = sizeof(stream->buffer) - stream->length;
            __libpov_memcpy(stream->buffer + stream->length, buf, tx);
            stream->length += tx;
            idx += tx;

            __cgc_fflush(stream);
        }

        if (idx < size)
        {
            tx = size - idx;
            __libpov_memcpy(stream->buffer + stream->length, buf + idx, tx);
            stream->length += tx;
            idx += tx;
        }

        return idx;
    }
}
