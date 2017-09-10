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

#include <cstdio.h>
#include "print.h"

void debug_bhdr(const bhdr &header)
{
    __printf("****Bios Header Information****\n");
    __printf("Bytes per sector: " ESC "d\n", header.bytes_per_sector);
    __printf("Sectors per cluster: " ESC "d\n", header.sectors_per_cluster);
    __printf("Num reserved sectors: " ESC "d\n", header.num_reserved_sectors);
    __printf("Num CgFs tables: " ESC "d\n", header.num_cgfst);
    __printf("Num root directories: " ESC "d\n", header.num_root_dirs);
    __printf("Total Sectors (<65535): " ESC "d\n", header.total_sectors_small);
    __printf("Media Info: " ESC "d\n", header.media_info);
    __printf("unused: " ESC "d\n", header.unused);
    __printf("unused track info: " ESC "d\n", header.unused_track_info);
    __printf("unused partition info: " ESC "d\n", header.unused_partition_info);
    __printf("Total Sectors (>65535): " ESC "d\n", header.total_sectors_large);

    __printf("\n**Extended CgFs Info**\n");
    __printf("Sectors per CgFs Table: " ESC "d\n", header.sectors_per_cgfst);
    __printf("Drive Description: " ESC "d\n", header.drive_desc);
    __printf("Version: " ESC "d\n", header.version);
    __printf("Root Cluster Idx: " ESC "d\n", header.root_cluster_idx);
    __printf("FS Info Sector Idx: " ESC "d\n", header.fs_info_sector_idx);
    __printf("Boot Copy Sector Idx: " ESC "d\n", header.boot_copy_sector_idx);
    __printf("--12 Bytes Reserved--\n");
    __printf("--16 Bytes Reserved--\n");
    __printf("--FS TYPE--\n");
    PRINT_ARR_CHARS(header.fs_type, sizeof(header.fs_type));

    __printf("\n**Boot code and end sequence**\n");
    __printf("--Boot Code (419 bytes)--\n");
    __printf("End of Header Signature: \\x" ESC "02x\\0x" ESC "02x\n", header.end_signature[0], header.end_signature[1]);
    __printf("Total header size: " ESC "d\n", sizeof(header));
    __printf("****End Bios Header****\n");
    __cgc_fflush(stdout);
}

void debug_fs_info(const fs_info_sector &fs_info)
{
    __printf("****FS Info Sector****\n");
    __printf("--FS Signature 1--\n");
    PRINT_ARR_CHARS(fs_info.signature, sizeof(fs_info.signature));
    __printf("--Reserved Space (480 Bytes)--\n");
    __printf("--FS Signature 2--\n");
    PRINT_ARR_CHARS(fs_info.signature2, sizeof(fs_info.signature2));
    __printf("Num Free Clusters: " ESC "d\n", fs_info.num_free_clusters);
    __printf("Most Recent Cluster Allocation: " ESC "d\n", fs_info.most_recent_cluster_alloc);
    __printf("--12 Bytes Reserved--\n");
    __printf("End of Header Signature: \\x" ESC "02x\\0x" ESC "02x\\x" ESC "02x\\0x" ESC "02x\n", fs_info.end_signature[0], fs_info.end_signature[1],
                                                                        fs_info.end_signature[2], fs_info.end_signature[3]);
    __printf("****End FS Info****\n");
    __cgc_fflush(stdout);

}

void debug_file_metadata(const fs_file *file_metadata)
{
    __printf("****File Metadata****\n");
    __printf("--File Name--\n");
    PRINT_ARR_CHARS(file_metadata->name, sizeof(file_metadata->name));
    __printf("--Attributes--\n");
    PRINT_ARR_BYTES(&file_metadata->attrib, sizeof(file_metadata->attrib));
    __printf("Starting Cluster Index: " ESC "d\n", file_metadata->starting_cluster);
    __printf("File size = " ESC "d\n", file_metadata->size);
    __printf("****End File Metadata****\n");
    __cgc_fflush(stdout);
}

void debug_file(Array<cluster_data> file_clusters)
{
    __printf("****File Hex View****\n");
    for (int i = 0; i < file_clusters.length(); i++) {
        PRINT_ARR_HEXDUMP(file_clusters[i].data, file_clusters[i].cluster_size);
    }
    __printf("****End File Hex View****\n");
    __cgc_fflush(stdout);
}
