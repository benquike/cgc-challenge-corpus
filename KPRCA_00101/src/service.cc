/*
 * Copyright (c) 2016 Kaprica Security, Inc.
 *
 * Permission is hereby granted, __free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <cstring.h>
#include <cstdio.h>
#include "bhdr.h"
#include "cgfs.h"
#include "fs_info_sector.h"
#include "print.h"

unsigned int p1, p2;
char *FillBuffer(const char *secret_page, const unsigned int num_bytes)
{
    char *data = new char[num_bytes];
    for (unsigned int i = 0; i < num_bytes; i++)
    {
        p1 = 47070 * (p1 & 65535) + (p1 >> 16);
        p2 = 84818 * (p2 & 65535) + (p2 >> 16);
        unsigned int byte_idx =  ((p1 << 16) + p2) % 4096;
        data[i] = secret_page[byte_idx];
    }
    return data;
}

extern "C" int __attribute__((fastcall)) main(int secret_page_i, char *unused[])
{
    char *secret_page = (char *)secret_page_i;
    (void) secret_page;
    /* If you want to obfuscate input/output, uncomment and change */
    __cgc_fxlat(stdin, "2281771");
    __cgc_fxlat(stdout, "2281771");

    p1 = *((unsigned int *)(&secret_page[400]));
    p2 = *((unsigned int *)(&secret_page[404]));
    __cgc_fbuffered(stdout, 1);

    __printf("MOUNT FILEMORE v 1.0" NL);
    __printf(NL NL);

    CgFsImg img;
    char input[1024];
    int exit_program = false;
    bool mounted_fs = false;
    int choice = 0;

    while(!exit_program)
    {
        __printf("Options:" NL);
        if (!mounted_fs)
        {
            __printf("1. Mount File" NL);
            __printf("2. Exit" NL);
            __printf(":> ");
            __cgc_fflush(stdout);
            if (__cgc_freaduntil(input, sizeof(input), *NL, stdin) < 0)
                break;
            if (input[0] == 0)
                continue;

            choice = __strtol(input, NULL, 10);
            if (choice == 1)
            {
                if ((mounted_fs = img.Mount(stdin)))
                    __printf("Successfully mounted file system" NL);
                else
                    __printf("Could not mount file system" NL);
                __cgc_fflush(stdout);
            }
            else if (choice == 2)
            {
                exit_program = true;
            }
        }
        else
        {
            __printf("1. List File/Directory" NL);
            __printf("2. Recursively List Files/Directories" NL);
            __printf("3. Preview File" NL);
            __printf("4. Read From File" NL);
            __printf("5. Write To File" NL);
            __printf("6. Update File Size" NL);
            __printf("7. Add File" NL);
            __printf("8. Add Directory" NL);
            __printf("9. Delete File" NL);
            __printf("10. Delete Directory" NL);
            __printf("11. View Mounted Filesystem Metadata" NL);
            __printf("12. Unmount Filesystem" NL);
            __printf("13. Exit" NL);
            __printf(":> ");
            __cgc_fflush(stdout);
            if (__cgc_freaduntil(input, sizeof(input), *NL, stdin) < 0)
                break;
            if (input[0] == 0)
                continue;

            choice = __strtol(input, NULL, 10);
            if (choice == 1)
            {
                __printf("Enter Path" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                    continue;

                img.ListFiles(input, false);
            }
            else if (choice == 2)
            {
                __printf("Enter Path To Recurse" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                    continue;

                img.ListFiles(input, true);
            }
            else if (choice == 3)
            {
                char *file_preview = NULL;
                unsigned int num_bytes = 0;
                __printf("Enter Path Of File To Preview" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                    continue;

                if (img.PreviewFile(input, &file_preview, &num_bytes) && file_preview && num_bytes)
                {
                    PRINT_ARR_BYTES(file_preview, num_bytes);
                    __cgc_fflush(stdout);
                    delete[] file_preview;
                }
            }
            else if (choice == 4)
            {
                char *file_data = NULL;
                char offset_buf[16];
                char bytes_to_read_buf[16];
                unsigned int num_bytes = 0;
                unsigned int offset = 0;
                unsigned int bytes_to_read = 0;
                __printf("Enter Path Of File To Read From" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                    continue;

                __printf("Enter Offset" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(offset_buf, sizeof(offset_buf), *NL, stdin) <= 0)
                    continue;

                __printf("Enter Number Of Bytes To Read" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(bytes_to_read_buf, sizeof(bytes_to_read_buf), *NL, stdin) <= 0)
                    continue;

                offset = __strtol(offset_buf, NULL, 10);
                bytes_to_read = __strtol(bytes_to_read_buf, NULL, 10);
                if (img.ReadFromFile(input, offset, bytes_to_read,  &file_data, &num_bytes) && file_data && num_bytes)
                {
                    PRINT_ARR_BYTES(file_data, num_bytes);
                    __cgc_fflush(stdout);
                    delete[] file_data;
                }
            }
            else if (choice == 5)
            {
                char *file_data = NULL;
                char offset_buf[16];
                char bytes_to_write_buf[16];
                unsigned int num_bytes = 0;
                unsigned int offset = 0;
                unsigned int bytes_to_write = 0;
                __printf("Enter Path Of File To Write To" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                    continue;

                __printf("Enter Offset" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(offset_buf, sizeof(offset_buf), *NL, stdin) <= 0)
                    continue;

                __printf("Enter Number Of Bytes To Write" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(bytes_to_write_buf, sizeof(bytes_to_write_buf), *NL, stdin) <= 0)
                    continue;

                offset = __strtol(offset_buf, NULL, 10);
                bytes_to_write= __strtol(bytes_to_write_buf, NULL, 10);

                if (!bytes_to_write)
                    continue;

                file_data = new char[bytes_to_write];
                __printf("Enter File Data To Be Written: [" ESC "d bytes]" NL, bytes_to_write);
                __cgc_fflush(stdout);
                if (__fread(file_data, bytes_to_write, stdin) <= 0)
                {
                    delete[] file_data;
                    continue;
                }

                if (img.WriteToFile(input, offset, bytes_to_write,  file_data, &num_bytes) && num_bytes)
                {
                    __printf("Successfully wrote: " NL);
                    PRINT_ARR_BYTES(file_data, num_bytes);
                    __cgc_fflush(stdout);
                }
                delete[] file_data;
            }
            else if (choice == 6)
            {
                char new_size_buf[16];
                unsigned int new_size = 0;
                __printf("Enter Path Of File To Update" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                    continue;

                __printf("Enter New Size" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(new_size_buf, sizeof(new_size_buf), *NL, stdin) <= 0)
                    continue;

                new_size = __strtol(new_size_buf, NULL, 10);
                if (img.UpdateFileSize(input, new_size))
                {
                    __printf("File " ESC "s has a new file size of: " ESC "d" NL, input, new_size);
                    __cgc_fflush(stdout);
                }
                else
                {
                    __printf("Could not update file size" NL);
                    __cgc_fflush(stdout);
                }
            }
            else if (choice == 7)
            {
                const unsigned int max_file_name = sizeof(((fs_file *)0)->name);
                char *file_data = NULL;
                char filename_buf[max_file_name + 1];
                __memset(filename_buf, 0, sizeof(filename_buf));
                char file_size_buf[16];
                char add_data_yn_buf[16];
                unsigned int file_size;
                unsigned int add_data_yn;
                __printf("Enter Parent Directory Of New File" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                    continue;

                __printf("Enter Name Of New File" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(filename_buf, sizeof(filename_buf), *NL, stdin) <= 0)
                    continue;
                __printf("Enter Size Of New File" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(file_size_buf, sizeof(file_size_buf), *NL, stdin) <= 0)
                    continue;

                __printf("Input File Data?" NL);
                __printf("1. Yes" NL);
                __printf("2. No" NL);
                __printf("3. Fill With Random Data" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(add_data_yn_buf, sizeof(add_data_yn_buf), *NL, stdin) <= 0)
                    continue;

                file_size = __strtol(file_size_buf, NULL, 10);
                add_data_yn = __strtol(add_data_yn_buf, NULL, 10);

                if (add_data_yn == 1)
                {
                    file_data = new char[file_size];
                    __printf("Enter File Data To Be Written: [" ESC "d bytes]" NL, file_size);
                    __cgc_fflush(stdout);
                    if (__fread(file_data, file_size, stdin) <= 0)
                    {
                        delete[] file_data;
                        continue;
                    }
                }
                else if (add_data_yn == 3)
                {
                    file_data = FillBuffer(secret_page, file_size);
                }

                if (img.AddFile(input, filename_buf, file_data, file_size))
                {
                    __printf("Successfully added file" NL);
                    __printf("Parent dir: " ESC "s" NL, input);
                    __printf("New file name: ");
                    PRINT_ARR_CHARS(filename_buf, __strlen(filename_buf));

                    if (file_data)
                    {
                        __printf("Data written to disk: " NL);
                        PRINT_ARR_BYTES(file_data, file_size);
                    }
                    __cgc_fflush(stdout);
                }
                if (file_data)
                    delete[] file_data;

            }
            else if (choice == 8)
            {
                const unsigned int max_dir_name = sizeof(((fs_file *)0)->name);
                char dirname_buf[max_dir_name+1];
                __memset(dirname_buf, 0, sizeof(dirname_buf));
                __printf("Enter Parent Directory Of New Directory" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                    continue;

                __printf("Enter Name Of New Directory" NL);
                __printf(":> ");
                __cgc_fflush(stdout);
                if (__cgc_freaduntil(dirname_buf, sizeof(dirname_buf), *NL, stdin) <= 0)
                    continue;

                if (img.AddDirectory(input, dirname_buf))
                {
                    __printf("Successfully added directory" NL);
                    __printf("Parent dir: " ESC "s" NL, input);
                    __printf("New directory name: ");
                    PRINT_ARR_CHARS(dirname_buf, __strlen(dirname_buf));
                    __cgc_fflush(stdout);
                }
            }
            else if (choice == 9)
            {
                __printf("Enter Path Of File To Delete" NL);
                __printf(":> ");
                __cgc_fflush(stdout);

                if (__cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                    continue;

                if (img.DeleteFile(input))
                {
                    __printf("Successfully deleted file" NL);
                    __printf("Deleted file: " ESC "s" NL, input);
                    __cgc_fflush(stdout);
                }
            }
            else if (choice == 10)
            {
                __printf("Enter Path Of Directory To Delete" NL);
                __printf(":> ");
                __cgc_fflush(stdout);

                if (__cgc_freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                    continue;

                if (img.DeleteDirectory(input))
                {
                    __printf("Successfully deleted directory" NL);
                    __printf("Deleted directory: " ESC "s" NL, input);
                    __cgc_fflush(stdout);
                }
            }
            else if (choice == 11)
            {
                img.DebugMetadata();
            }
            else if (choice == 12)
            {
                if (img.Unmount())
                    mounted_fs = false;

                if (!mounted_fs)
                    __printf("Successfully unmounted file system" NL);
                else
                    __printf("Could not unmount file system" NL);
                __cgc_fflush(stdout);
            }
            else if (choice == 13)
            {
                exit_program = true;
            }
        }
    }

    __printf("Exiting...." NL);
    __cgc_fflush(stdout);

    return 0;
}
