#include "recv.h"

int recv( char *dest, int length )
{
	int total = 0;
	size_t __read = 0;
	int left = length;

	if ( dest == NULL ) {
		return total;
	}

	while ( total < length ) {
		if ( receive( 0, dest+total, left, &__read) != 0 ) {
			__printf("[ERROR] Failed to receive data\n");
			return 0;
		}

        if (__read == 0) {
			__printf("[ERROR] Failed to receive data\n");
			return 0;
        }

		total += __read;
		left -= __read;
	}

	return total;
}
