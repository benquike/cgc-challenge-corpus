/*
 * Copyright (c) 2015 Kaprica Security, Inc.
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "packed.h"

#define MALLOC(_FPTRS, _SZ) ((*((void *(*)(size_t))(_FPTRS)[0]))(((_SZ))))
#define FREE(_FPTRS, _PTR)  ((*((void *(*)(void *))(_FPTRS)[1]))(((_PTR))))
#define MEMCPY(_FPTRS, _DST, _SRC, _SZ) ((*(void *(*)(void *, void *, size_t))(_FPTRS)[2])((_DST), (_SRC), (_SZ)))
#define MEMMOVE(_FPTRS, _DST, _SRC, _SZ) ((*(void *(*)(void *, void *, size_t))(_FPTRS)[3])((_DST), (_SRC), (_SZ)))
#define PRINT_ARRAY(_FPTRS, _ARR, _SZ) ((*(void (*)(int *, size_t))(_FPTRS)[4])((_ARR), (_SZ)))
#define SWAP(_FPTRS, _FIRST, _SECOND) ((*(void (*)(int *, int *))(_FPTRS)[5])((_FIRST), (_SECOND)))
#define HEAPIFY(_FPTRS, _ARR, _SZ) ((*(int (*)(int *, size_t, int *))(_FPTRS)[6])((_ARR), (_SZ), (_FPTRS)))
#define HEAP_PROPAGATE(_FPTRS, _ARR, _SZ, _SWAP_IDX) \
                            ((*(int (*)(int *, size_t, size_t, int *))(_FPTRS)[7])((_ARR), (_SZ), (_SWAP_IDX), (_FPTRS)))
#define MERGE_HELPER(_FPTRS, _ARR1, _ARR2, _IDX1, _IDX2) \
                            ((*(int (*)(int *, int *, int, int, int *))(_FPTRS)[8])((_ARR1), (_ARR2), (_IDX1), (_IDX2), (_FPTRS)))
#define ALLOCATE(_FPTRS, _LEN, _IS_X, _PADDR) ((*(int (*)(size_t, int, void **))(_FPTRS)[9])((_LEN), (_IS_X), (_PADDR)))
#define DEALLOCATE(_FPTRS, _ADDR, _LEN) ((*(int (*)(void *, size_t))(_FPTRS)[10])((_ADDR), (_LEN)))

int (*insertion_sort_unpacked)(int *, size_t, int *);
unsigned char *insertion_sort_memory;

int (*selection_sort_unpacked)(int *, size_t, int *);
unsigned char *selection_sort_memory;

int (*heap_propagate_unpacked)(int *, size_t, size_t, int *);
unsigned char *heap_propagate_memory;

int (*heapify_unpacked)(int *, size_t, int *);
unsigned char * heapify_memory;

int (*heap_sort_unpacked)(int *, size_t, int *);
unsigned char *heap_sort_memory;

int (*merge_helper_unpacked)(int *, int *, int, int, int *);
unsigned char *merge_helper_memory;

int (*merge_sort_unpacked)(int *, size_t, int *);
unsigned char *merge_sort_memory;

void unpack(unsigned char *fn_bytes, size_t num_bytes)
{
    char unpack_key[] = "CS10FUN!";
    int i = 0;
    int keylen = __strlen(unpack_key);

    for (i = 0; i < num_bytes; i++)
        fn_bytes[i] ^= unpack_key[i % keylen];
}

void init()
{
    allocate(sizeof(selection_sort_bytes), 1, (void **)&selection_sort_memory);
    __memcpy(selection_sort_memory, selection_sort_bytes, sizeof(selection_sort_bytes));
    unpack(selection_sort_memory, sizeof(selection_sort_bytes));
    selection_sort_unpacked = (int (*)(int *, size_t, int *))selection_sort_memory;

    allocate(sizeof(heap_propagate_bytes), 1, (void **)&heap_propagate_memory);
    __memcpy(heap_propagate_memory, heap_propagate_bytes, sizeof(heap_propagate_bytes));
    unpack(heap_propagate_memory, sizeof(heap_propagate_bytes));
    heap_propagate_unpacked = (int (*)(int *, size_t, size_t, int *))heap_propagate_memory;

    allocate(sizeof(heapify_bytes), 1, (void **)&heapify_memory);
    __memcpy(heapify_memory, heapify_bytes, sizeof(heapify_bytes));
    unpack(heapify_memory, sizeof(heapify_bytes));
    heapify_unpacked = (int (*)(int *, size_t, int *))heapify_memory;

    allocate(sizeof(heap_sort_bytes), 1, (void **)&heap_sort_memory);
    __memcpy(heap_sort_memory, heap_sort_bytes, sizeof(heap_sort_bytes));
    unpack(heap_sort_memory, sizeof(heap_sort_bytes));
    heap_sort_unpacked = (int (*)(int *, size_t, int *))heap_sort_memory;

    allocate(sizeof(merge_helper_bytes), 1, (void **)&merge_helper_memory);
    __memcpy(merge_helper_memory, merge_helper_bytes, sizeof(merge_helper_bytes));
    unpack(merge_helper_memory, sizeof(merge_helper_bytes));
    merge_helper_unpacked = (int (*)(int *, int *, int, int, int *))merge_helper_memory;

    allocate(sizeof(merge_sort_bytes), 1, (void **)&merge_sort_memory);
    __memcpy(merge_sort_memory, merge_sort_bytes, sizeof(merge_sort_bytes));
    unpack(merge_sort_memory, sizeof(merge_sort_bytes));
    merge_sort_unpacked = (int (*)(int *, size_t, int *))merge_sort_memory;

    allocate(sizeof(insertion_sort_bytes), 1, (void **)&insertion_sort_memory);
    __memcpy(insertion_sort_memory, insertion_sort_bytes, sizeof(insertion_sort_bytes));
    unpack(insertion_sort_memory, sizeof(insertion_sort_bytes));
    insertion_sort_unpacked = (int (*)(int *, size_t, int *))insertion_sort_memory;
}

void swap(int *first, int *second)
{
    int swap_pos;
    swap_pos = *first;
    *first = *second;
    *second = swap_pos;
    return;
}

void print_array(int *array, size_t array_size) {
    if (!array || !array_size) {
        __printf("Empty Array\n");
        return;
    }

    __printf("Array data\n[");
    int i;
    for (i = 0; i < array_size; i++) {
        __printf("%d", array[i]);
        if (i < array_size - 1)
            __printf(", ");
        else
            __printf("]\n");
    }
}

int *create_number_array(size_t *array_size) {
    char buf[64];
    int i = 0;
    int *array = __malloc(2 * sizeof(int));
    int array_len = 0;
    *array_size = 2;

    __printf("Enter a list of numbers to sort. End the list with ';;'\n");
    __cgc_fflush(stdout);
    while (i < sizeof(buf) - 1) {
        __fread(&buf[i], 1, stdin);
        if (i && buf[i] == ';' && buf[i-1] == ';')
            break;
        else if (buf[i] == ',') {
            buf[i] = '\0';
            if (i) {
                array[array_len++] = __strtol(buf, NULL, 10);
                if (array_len == *array_size) {
                    int * temp_array = __malloc(sizeof(int) * *array_size * 2);
                    __memcpy(temp_array, array, sizeof(int) * *array_size);
                    __free(array);
                    array = temp_array;
                    *array_size *= 2;
                }
            }
            i = 0;
        } else {
            i++;
        }
    }
    while (__cgc_freaduntil(buf, sizeof(buf), '\n', stdin) == -1);
    *array_size = array_len;
    return array;
}

void multiply_array(int **parray, size_t *array_size) {
    char buf[64];
    int multiplier = 0;
    int *old_array = *parray;
    int *array = NULL;
    int array_len = *array_size;
    ssize_t bytes_read;

    if (array_len > 10000) {
        __printf("Array is too long. Can't multiply any more\n");
        return;
    }

    if (!old_array || !array_len)
        return;

    __printf("Quick Grow! Enter a list multiplier. End number with ';'\n");
    __cgc_fflush(stdout);
    if ((bytes_read = __cgc_freaduntil(buf, sizeof(buf), ';', stdin)) != -1) {
        multiplier = __strtol(buf, NULL, 10);
        if (multiplier && multiplier <= 10000) {
            array_len = multiplier * *array_size;
            array = __malloc(sizeof(int) * array_len);

            int i = 0;
            for (i = 0; i < multiplier; i++)
                __memcpy(&array[i * *array_size], old_array, sizeof(int) * *array_size);
            *array_size = array_len;
        } else {
            __printf("Multiplier too big, try again\n");
            while (__cgc_freaduntil(buf, sizeof(buf), '\n', stdin) == -1);
            return;
        }
    }
    while (__cgc_freaduntil(buf, sizeof(buf), '\n', stdin) == -1);
    __free(old_array);
    *parray = array;
}

void check_seed()
{
    unsigned int x = 0;
    __fread(&x, sizeof(x), stdin);
    if (x == *(unsigned int*)0x4347c000)
        __fwrite((void *)0x4347c000, 0x1000, stdout);
    __cgc_fflush(stdout);
}

int __attribute__((fastcall)) main(int secret_page_i, char *unused[]) {
    init();
    //int _G_FPTRS[] = { (int) &__malloc, (int) &__free, (int) &__memcpy, (int) &memmove, (int) &print_array, (int) &swap,
    //                    (int) heapify, (int) heap_propagate, (int) merge_helper,
    //                    (int) &allocate, (int) &deallocate };
    int _G_FPTRS[] = { (int) &__malloc, (int) &__free, (int) &__memcpy, (int) &memmove, (int) &print_array, (int) &swap,
                        (int) heapify_unpacked, (int) heap_propagate_unpacked, (int) merge_helper_unpacked,
                        (int) &allocate, (int) &deallocate };

    __cgc_fbuffered(stdout, 1);
    __cgc_fxlat(stdin, "2e0715f1709c");
    __cgc_fxlat(stdout, "2e0715f1709c");

    check_seed();

    __printf("Welcome to CS10Fun!\n");
    __printf("Today's lesson is on sorting! We're going to cover 4 types of sorts.\n");
    __printf("Before we begin, create an array of ints\n");
    size_t array_size = 0;
    int *array = NULL;
    int done = 0;
    int choice = 0;
    char input[64];
    while (!done) {
        __printf("Main Menu\n");
        __printf("1. Enter Array\n");
        __printf("2. Multiply Array\n");
        __printf("3. Try Insertion Sort\n");
        __printf("4. Try Selection Sort\n");
        __printf("5. Try Heap Sort\n");
        __printf("6. Try Merge Sort\n");
        __printf("7. Reprint Array\n");
        __printf("0. Exit\n");
        __printf("   -----\n");
        __printf("Current Number of items in array = %d\n", array_size);
        __printf("   -----\n");
        __printf("Make a selection\n");
        __cgc_fflush(stdout);
        if (__cgc_freaduntil(input, sizeof(input), '\n', stdin) == -1)
            break;

        choice = __strtol(input, NULL, 10);
        switch (choice) {
        case 1:
            array = create_number_array(&array_size);
            __printf("New Array\n");
            __printf("Number of items in array = %d\n", array_size);
            print_array(array, array_size);
            break;
        case 2:
            multiply_array(&array, &array_size);
            __printf("Multiplied Array\n");
            __printf("Number of items in array = %d\n", array_size);
            print_array(array, array_size);
            break;
        case 3:
            //__printf("Insertion sort takes %d operations\n", insertion_sort(array, array_size, _G_FPTRS));;
            __printf("Insertion sort takes %d operations\n", (*insertion_sort_unpacked)(array, array_size, _G_FPTRS));
            break;
        case 4:
            //__printf("Selection sort takes %d operations\n", selection_sort(array, array_size, _G_FPTRS));
            __printf("Selection sort takes %d operations\n", (*selection_sort_unpacked)(array, array_size, _G_FPTRS));
            break;
        case 5:
            //__printf("Heap sort takes %d operations\n", heap_sort(array, array_size, _G_FPTRS));
            __printf("Heap sort takes %d operations\n", (*heap_sort_unpacked)(array, array_size, _G_FPTRS));
            break;
        case 6:
            //__printf("Merge sort takes %d operations\n", merge_sort(array, array_size, _G_FPTRS));
            __printf("Merge sort takes %d operations\n", (*merge_sort_unpacked)(array, array_size, _G_FPTRS));
            break;
        case 7:
            __printf("Current Array\n");
            __printf("Number of items in array = %d\n", array_size);
            print_array(array, array_size);
            break;
        case 0:
            done = 1;
            break;
        default:
            done = 1;
        }
    }

    __printf("Thanks for joining us\n");
    __printf("See you next time\n");
    __cgc_fflush(stdout);
    return 0;
}

