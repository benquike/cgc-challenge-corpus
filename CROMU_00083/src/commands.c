/*

Author: Steve Wood <swood@cromulence.com>

Copyright (c) 2016 Cromulence LLC

Permission is hereby granted, __cgc_free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include <libcgc.h>
#include "stdlib.h"
#include "service.h"
#include "filesystem.h"
#include "stdio.h"
#include "string.h"
#include "input.h"
#include "malloc.h"

extern securityIdType securityID;

int makeFile(int argcount, char **args) {

int retcode;

    if (argcount < 2) {

        __cgc_printf("invalid usage\n");
        return -1;
    }

    retcode = createFile(args[1], REGULAR, securityID);

    if ( retcode != 0 ) {

        __cgc_printf("error\n");
        return retcode;
    }

    return 0;

}


int makeMemFile(int argcount, char **args) {

int retcode;
int memsize;
void *memptr;

    if (argcount < 3) {

        __cgc_printf("invalid usage\n");
        return -1;
    }

    memsize = __cgc_atoi(args[2]);

    if (memsize < 0 || memsize > 8192*10) {

        return -1;

    }

    memptr = __cgc_malloc(memsize);

    if ( memptr < 0 ) {

        return -1;

    }

    __cgc_bzero(memptr, memsize);

    retcode = makeMemoryFile(args[1], (unsigned int)memptr, memsize,  0,  securityID );
    if ( retcode != 0 ) {

        __cgc_printf("error\n");
        return retcode;
    }

    return 0;

}

int eraseFile(int argcount, char **args) {

int retcode;
fileHandleType fh;

    if (argcount < 2) {

        __cgc_printf("invalid usage\n");
        return -1;
    }

    fh = openFile(args[1], securityID);

    switch (fh) {

        case ERROR_NO_PERMISSION:

            __cgc_printf("permission denied\n");
            break;

        case ERROR_NOT_FOUND:

            __cgc_printf("file $s not found\n", args[1]);
            break;

        case ERROR_BAD_TYPE:

            __cgc_printf("bad type for erase\n");
            break;

    }

    if ( fh < 0 ) {

        return (fh);

    }

    retcode = deleteFile(fh, securityID);

    if ( retcode < 0 ) {

        __cgc_printf("error erasing $s\n", args[1]);

        closeFile(fh);
        return (retcode);

    }

    return 0;

}

int listFiles(int argcount, char **args) {

int retcode;
findFileHandleType *fileInfoHandle;
char *otherperms [] = { "--", "-W", "R-", "RW" };
char *filetype [] = { "", "DIR", "REG", "MEM", "ROM" };
char ownerName[10];

    fileInfoHandle = 0;

    if ( argcount > 1 ) {

        retcode = findFiles(args[1], &fileInfoHandle);
    
    }
    else {

        retcode = findFiles("", &fileInfoHandle);

    }


    __cgc_printf("FILENAME                   SIZE        OWNER     O-PERMS    TYPE\n");
    __cgc_printf("----------------------------------------------------------------\n");
    while ( retcode == 0 ) {

        if (lookupName(ownerName, fileInfoHandle->currentFile->securityID) == -1)
            __cgc_strcpy(ownerName, "       ");

        __cgc_printf("$-25s  $10d  $-8s  $2s    $3s\n", fileInfoHandle->currentFile->name, 
                                    fileInfoHandle->currentFile->fileSize,
                                    ownerName, 
                                    otherperms[fileInfoHandle->currentFile->othersPermissions], 
                                    filetype[fileInfoHandle->currentFile->fileType]);

        retcode = findNextFile(fileInfoHandle);

    }
    __cgc_printf("\n");

    return 0;

}

int overwriteFile( int argcount, char **args) {

int retcode;
fileHandleType fh;
char text[1024];

    if (argcount < 2) {

        __cgc_printf("invalid usage\n");
        return -1;
    }

    fh = openFile(args[1], securityID);

    if ( fh <  0 ) {

        switch (fh) {

            case ERROR_NO_PERMISSION:

                __cgc_printf("permission denied\n");
                break;

            case ERROR_NOT_FOUND:

                __cgc_printf("file $s not found\n", args[1]);
                break;

            case ERROR_BAD_TYPE:

                __cgc_printf("unable to __cgc_write files of this type\n");
                break;

            default:
        
                __cgc_printf("Error opening $s\n", args[1]);
        
        }
        
        return (fh);
    }

    retcode = truncateFile(fh, securityID);

    if ( retcode <  0 ) {

        switch (retcode) {

            case ERROR_NO_PERMISSION:

                __cgc_printf("permission denied for __cgc_write\n");
                break;

            default:
        
                __cgc_printf("Error writing to $s\n", args[1]);
        
        }

        closeFile(fh);
        return (retcode);
    }

    while (1) {

        __cgc_receive_until(text, '\n', sizeof(text));

        if ( __cgc_strlen(text) == 0 ) {

            break;
        }

        retcode = writeFile(fh, text, __cgc_strlen(text), securityID);

        if ( retcode < 0 ) {

            __cgc_printf("Error writing to $s\n", args[1]);
            closeFile(fh);
            return (retcode);

        }

    }

    closeFile(fh);

    return 0;

}


int dumpFile(int argcount, char **args) {

int retcode;
fileHandleType fh;
char buffer[1024];
unsigned int readcount;

    if (argcount < 2) {

        __cgc_printf("invalid usage\n");
        return -1;
    }

    fh = openFile(args[1], securityID);

    if ( fh <  0 ) {

        switch (fh) {

            case ERROR_NO_PERMISSION:

                __cgc_printf("permission denied\n");
                break;

            case ERROR_NOT_FOUND:

                __cgc_printf("file $s not found\n", args[1]);
                break;
                
            case ERROR_BAD_TYPE:

                __cgc_printf("unable to display files of this type\n");
                break;

            default:
        
                __cgc_printf("Error opening $s\n", args[1]);
        
        }
        
        return (fh);
    }

    __cgc_bzero(buffer, sizeof(buffer));

    retcode = readFile(fh, buffer, sizeof(buffer), 0, &readcount, securityID);

    if ( retcode <  0 && retcode != ERROR_EOF ) {

        switch (retcode) {

            case ERROR_NO_PERMISSION:

                __cgc_printf("permission denied for __cgc_read\n");
                break;

            default:
        
                __cgc_printf("Error reading from $s\n", args[1]);
        
        }

        closeFile(fh);
        return (retcode);
    }

    retcode = 0;

    while ( retcode == 0 ) {

        __cgc_write(STDOUT, buffer, readcount);
        __cgc_bzero(buffer, sizeof(buffer));
        retcode = readFile(fh, buffer, sizeof(buffer), 0, &readcount, securityID);
    
    }

    closeFile(fh);

    __cgc_printf("\n");

    if (retcode == ERROR_EOF ) {

        return 0;

    }
    else {

        __cgc_printf("Error reading file\n");
        return -1;

    }


}

int readFromEnd(int argcount, char **args) {

int retcode;
fileHandleType fh;
char buffer[1024];
unsigned int readcount;
fileInfoType fileinfo;

    if (argcount < 3) {

        __cgc_printf("invalid usage\n");
        return -1;
    }

    if ( statusFile( args[1],  &fileinfo) != 0 ) {

        __cgc_printf("unable to stat file $s\n", args[1]);
        return -1;

    }

    fh = openFile(args[1], securityID);

    if ( fh <  0 ) {

        switch (fh) {

            case ERROR_NO_PERMISSION:

                __cgc_printf("permission denied\n");
                break;

            case ERROR_NOT_FOUND:

                __cgc_printf("file $s not found\n", args[1]);
                break;
                
            default:
        
                __cgc_printf("Error opening $s\n", args[1]);
        
        }
        
        return (fh);
    }

    fileReadPosition(fh, fileinfo.size - __cgc_atoi(args[2]));

    __cgc_bzero(buffer, sizeof(buffer));

    retcode = readFile(fh, buffer, sizeof(buffer), 0, &readcount, securityID);

    if ( retcode <  0 && retcode != ERROR_EOF ) {

        switch (retcode) {

            case ERROR_NO_PERMISSION:

                __cgc_printf("permission denied for __cgc_read\n");
                break;

            default:
        
                __cgc_printf("Error reading from $s\n", args[1]);
                __cgc_printf("errno is $d\n", retcode);
        
        }

        closeFile(fh);
        return (retcode);
    }

    retcode = 0;

    if (readcount > 0 ) {

        __cgc_write(STDOUT, buffer, readcount);
    }

    while ( retcode == 0 ) {

        __cgc_bzero(buffer, sizeof(buffer));
        retcode = readFile(fh, buffer, sizeof(buffer), 0, &readcount, securityID);
        __cgc_write(STDOUT, buffer, readcount);
    
    }

    closeFile(fh);

    __cgc_printf("\n");

    if (retcode == ERROR_EOF ) {

        return 0;

    }
    else {

        __cgc_printf("Error reading file\n");
        return -1;

    }


}


int readFirstN(int argcount, char **args) {

int retcode;
fileHandleType fh;
char buffer[1024];
unsigned int readCount;
unsigned int readTermCount;
unsigned int totalRead;

    if (argcount < 3) {

        __cgc_printf("invalid usage\n");
        return -1;
    }

    fh = openFile(args[1], securityID);

    if ( fh <  0 ) {

        switch (fh) {

            case ERROR_NO_PERMISSION:

                __cgc_printf("permission denied\n");
                break;

            case ERROR_NOT_FOUND:

                __cgc_printf("file $s not found\n", args[1]);
                break;
                
            default:
        
                __cgc_printf("Error opening $s\n", args[1]);
        
        }
        
        return (fh);
    }

    readTermCount = __cgc_atoi(args[2]);

    if ( readTermCount == 0 ) {

        return 0;
    }

    totalRead = 0;
    __cgc_bzero(buffer, sizeof(buffer));

    retcode = readFile(fh, buffer, __cgc_minimum(sizeof(buffer), readTermCount - totalRead), 0, &readCount, securityID);

    if ( retcode <  0 && retcode != ERROR_EOF ) {

        switch (retcode) {

            case ERROR_NO_PERMISSION:

                __cgc_printf("permission denied for __cgc_read\n");
                break;

            default:
        
                __cgc_printf("Error reading from $s\n", args[1]);
                __cgc_printf("errno is $d\n", retcode);
        
        }

        closeFile(fh);
        return (retcode);
    }

    retcode = 0;

    if (readCount > 0 ) {

        totalRead += readCount;
        __cgc_write(STDOUT, buffer, readCount);
    }

    while ( retcode == 0 && totalRead < readTermCount ) {

        __cgc_bzero(buffer, sizeof(buffer));
        retcode = readFile(fh, buffer, __cgc_minimum(sizeof(buffer), readTermCount - totalRead), 0, &readCount, securityID);
        __cgc_write(STDOUT, buffer, readCount);
        totalRead += readCount;
    
    }

    closeFile(fh);

    __cgc_printf("\n");

    if (retcode == ERROR_EOF || retcode == NO_ERROR ) {

        return 0;

    }
    else {

        __cgc_printf("Error reading file\n");
        return -1;

    }

}


int copyFile(int argcount, char **args) {

int retcode;
fileHandleType fh;
fileHandleType fh2;
char buffer[1024];
unsigned int readcount;
fileInfoType fileinfo;

    if (argcount < 3) {

        __cgc_printf("invalid usage\n");
        return -1;
    }

    fh = openFile(args[1], securityID);

    if ( fh <  0 ) {

        switch (fh) {

            case ERROR_NO_PERMISSION:

                __cgc_printf("permission denied\n");
                break;

            case ERROR_NOT_FOUND:

                __cgc_printf("file $s not found\n", args[1]);
                break;
                
            default:
        
                __cgc_printf("error opening $s\n", args[1]);
        
        }
        
        return (fh);
    }

    if ( statusFile( args[2],  &fileinfo) != 0 ) {

        retcode = createFile(args[2], REGULAR, securityID);

        if ( retcode != 0 ) {

            __cgc_printf("error creating $s\n", args[2]);
            return -1;
        }

    }
    else {

        __cgc_printf("file $s already exists\n", args[2]);
        closeFile(fh);
        return -1;

    }

    fh2 = openFile(args[2], securityID);

    if ( fh2 < 0 ) {

        __cgc_printf("unable to open $s for writing\n", args[2]);

        closeFile(fh);

        return -1;

    }

    retcode = 0;

    while ( retcode == 0 ) {

        __cgc_bzero(buffer, sizeof(buffer));

        retcode = readFile(fh, buffer, sizeof(buffer), 0, &readcount, securityID);

        if (readcount > 0 ) {

            writeFile( fh2, buffer, readcount, securityID);
        }

    }

    closeFile(fh);

    if (retcode == ERROR_EOF ) {


        closeFile(fh2);
        return 0;

    }
    else if (retcode == ERROR_NO_PERMISSION) {

        __cgc_printf("permission denied for __cgc_read\n");
        retcode = deleteFile(fh2, securityID);
        closeFile(fh2);
        return -1;
    }
    else {

        retcode = deleteFile(fh2, securityID);
        __cgc_printf("error reading file\n");
        closeFile(fh2);
        return -1;

    }


}

int setPermissions(int argcount, char **args) {

int retcode;
otherPermsType operms;
fileHandleType fh;

    if (argcount < 3) {

        __cgc_printf("invalid usage\n");
        return -1;
    }

    operms = __cgc_atoi(args[2]);

    if ( operms > 3 ) {

        __cgc_printf("invalid usage\n");
        return -1;

    }

    fh = openFile(args[1], securityID);

    if ( fh < 0 ) {

        switch (fh) {

            case ERROR_NOT_FOUND:

                __cgc_printf("file $s not found\n", args[1]);
                return -1;
                break;

            case ERROR_NO_PERMISSION:

                __cgc_printf("permission denied\n");
                return fh;
                break;

        } // switch

    } // if ( fh < 0 )

    retcode = setPerms(fh, operms, securityID);

    if ( retcode != 0 ) {

        closeFile(fh);
        __cgc_printf("permission denied\n");
        return -1;

    }

    closeFile(fh);

    return 0;

}


