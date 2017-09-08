#include <libcgc.h>
#include <stdlib.h>

void sort_command( char *pszLine )
{
    int idx;
    int sortList[64];
    char szTempBuf[128];

    if ( pszLine == NULL )
    {
        __printf( "Empty command.\n" );
        return;
    }

    // BUG:: Overflow possible here
#ifdef PATCHED
    unsigned char itemCount = __atoi( pszLine );
#else
    char itemCount = __atoi( pszLine );
#endif

    if ( itemCount == 0 )
    {
        __printf( "0 items sorted.\n" );
        return;
    }
    else if ( itemCount <= 64 )
        __printf( "Enter @d items to sort:\n", itemCount );
    else
    {
        __printf( "Maximum of 64 items allowed to sort.\n" );

        return;
    }

    unsigned int itemTotal = (itemCount & 0xFF);

    for ( idx = 0; idx < itemTotal; idx++ )
    {
        getline( szTempBuf, 128 );

        sortList[idx] = __atoi( szTempBuf );
    }

    // Begin sorting
    // Naive bubble sort method

    for ( int i = 0; i < (itemTotal-1); i++ )
    {
        int bSwapped = 0;
        for ( int j = 0; j < (itemTotal-i)-1; j++ )
        {
            if ( sortList[j] > sortList[j+1] )
            {
                int temp = sortList[j];
                sortList[j] = sortList[j+1];
                sortList[j+1] = temp;

                bSwapped = 1;
            }
        }

        if ( !bSwapped )
            break;
    }

    __printf( "@d items sorted.\n", itemTotal );

    for ( idx = 0; idx < itemTotal; idx++ )
    {
        __printf( "@d\n", sortList[idx] );
    }

    // Exit
    return;
}
