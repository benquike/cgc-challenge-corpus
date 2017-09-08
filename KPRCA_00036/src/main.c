/*
 * Copyright (c) 2014 Kaprica Security, Inc.
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

#include <libcgc.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "thompson.h"
#include "trex.h"

#define TEXT_END_SEQ "\n\n\n"
#define MAX_TEXT_SIZE 500000
#define MAX_SEARCH_STR_SIZE 500

//Put the bug here -- Get it so there is a case where resizing the buffer doesn't set it to null
static int resize_buf(void **buf, size_t old_size)
{
    if (MAX_TEXT_SIZE / 2 < old_size)
        return 0;

    int new_size = old_size * 2;
    char *new_buf = __malloc(new_size);
    if (*buf) {
        __memcpy(new_buf, *buf, old_size);
        __free(*buf);
    }
    *buf = new_buf;
    return new_size;
}

static unsigned char readopt(int fd) {
    unsigned char c = 0, d = 0;
    size_t rx;

    if ((receive(fd, &c, 1, &rx) != 0 || rx == 0) || (receive(fd, &d, 1, &rx) != 0 || rx == 0))
        return -1;

    if (d != '\n') {
        while(d != '\n')
            if (receive(fd, &d, 1, &rx) != 0 || rx == 0)
                return -1;
        return -1;
    }

    return c;
}

static int readtrex(int fd, unsigned char *trex, size_t size)
{
    size_t i, rx;
    unsigned char d = 0;

    for (i = 0; i < size; i++) {
        if (receive(fd, trex, 1, &rx) != 0 || rx == 0)
            return -1;
        if (*trex == '\n')
            break;
        trex++;
    }

    if (*trex != '\n') {
        while(d != '\n')
            if (receive(fd, &d, 1, &rx) != 0 || rx == 0)
                return -1;
        return -1;
    } else {
        *trex = '\0';
    }

    return i;
}

static int create_text(int fd, unsigned char **text)
{
    size_t len = 0, buf_size = 16, rx;
    unsigned char *pend_seq = TEXT_END_SEQ;
    unsigned char *buf;

    buf_size = resize_buf(text, buf_size);
    while(buf_size) {
        buf = *text;
        if (receive(fd, &buf[len], 1, &rx) != 0 || rx == 0)
            goto bad_text;
        if (buf[len] == *pend_seq) {
            if (*++pend_seq == '\0')
                break;
        } else {
            pend_seq = TEXT_END_SEQ;
        }
        if (++len == buf_size)
            buf_size = resize_buf(text, buf_size);
    }
    if(len < 3 || !buf_size)
        goto bad_text;

    (*text)[len - 2] = '\0';
    return len - 2;
bad_text:
    if (buf_size)
        __free(*text);
    *text = NULL;
    return 0;
}

void print_menu(match_type_e match_type, unsigned char *text, unsigned char *trex)
{
    unsigned char temp;
    __printf("Text Searcher: Main Menu\n");
    __printf("1. Enter Text\n");
    __printf("2. Enter Search String\n");
    __printf("3. Line Search - Partial Matches\n");
    __printf("4. Line Search - Exact Matches\n");
    __printf("5. Print Text\n");
    __printf("6. Print Search String\n");
    __printf("7. Search Text\n");
    __printf("8. Quit\n\n");
    if (text) {
        if (__strlen(text) > 15) {
            temp = text[15];
            text[15] = '\0';
            __printf("[Text=%s...]\n", text);
            text[15] = temp;
        } else {
            __printf("[Text=%s]\n", text);
        }
    }
    if (__strlen(trex)) {
        if (__strlen(trex) > 15) {
            temp = trex[15];
            trex[15] = '\0';
            __printf("[SearchString=%s...]\n", trex);
            trex[15] = temp;
        } else {
            __printf("[SearchString=%s]\n", trex);
        }
    }

    if (match_type == PARTIAL)
        __printf("[LineMatching=Partial]\n");
    if (match_type == ALL)
        __printf("[LineMatching=Exact]\n");
    __printf("Select: ");
}

static void test()
{
    state_t *nfa;
    unsigned char *rpn = NULL;
    unsigned char *test_string = "aaaaaaaa";
    unsigned char *trex = "a";
    int retcode = retorpn(trex, __strlen(trex) + 1, &rpn);
    retcode = retorpn(trex, __strlen(trex) + 1, &rpn);
    if (retcode >= 0)
        nfa = evalrpn(rpn);
    else
        __printf("Invalid search string\n");
    if (nfa == NULL)
        __printf("Bad rpn\n");
    debug_state(nfa);
    __printf("trex = %s\n", trex);
    __printf("rpn = %s\n", rpn);
    __printf("Test string = %s\n", test_string);

    __printf("Partial Matching:\n");
    match(nfa, test_string, PARTIAL);
    __printf("Full Line Matching:\n");
    match(nfa, test_string, ALL);

    __exit(0);
}

int main(void)
{
    int quit = FALSE;
    unsigned char *text = NULL, *rpn = NULL;
    unsigned char trex[MAX_SEARCH_STR_SIZE] = {'\0'};
    unsigned char selection = 0;
    int match_len = 0, text_len = 0, trex_len = 0, retcode;
    match_type_e match_type = PARTIAL;
    state_t *nfa = NULL;
    init_trex();

    do {
        __printf("\n");
        print_menu(match_type, text, trex);
        selection = readopt(STDIN);
        //fdprintf(2, "Selection = %c\n", selection);
        switch(selection) {
            case '1':
                __printf("Enter text to search\n");
                if(text) {
                    __free(text);
                    text = NULL;
                }
                text_len = create_text(STDIN, &text);
                break;
            case '2':
                __printf("Enter search string\n");
                trex_len = readtrex(STDIN, trex, MAX_SEARCH_STR_SIZE);
                if (trex_len < 0)
                {
                    trex_len = 0;
                    *trex = 0;
                }
                break;
            case '3':
                match_type = PARTIAL;
                break;
            case '4':
                match_type = ALL;
                break;
            case '5':
                if (text)
                    __printf("--Text--\n%s\n", text);
                else
                    __printf("--Text--\n\n", text);
                break;
            case '6':
                if (__strlen(trex))
                    __printf("--Search String--\n%s\n", trex);
                else
                    __printf("--Search String--\n\n", trex);
                break;
            case '7':
                if (!text_len) {
                    __printf("Input text has not been received\n");
                    break;
                } else if(!trex_len) {
                    __printf("Search text has not been received\n");
                    break;
                }
                retcode = retorpn(trex, __strlen(trex) + 1, &rpn);
                if (retcode >= 0) {
                    nfa = evalrpn(rpn);
                } else {
                    __printf("Invalid search string\n");
                    break;
                }
                if (nfa == NULL) {
                    __printf("Bad rpn\n");
                    break;
                }
                match(nfa, text, match_type);
                break;
            case '8':
                break;
            default:
                __printf("Bad Selection\n");
        }
    } while (selection != '8');

    __printf("Exiting...\n");
    return 0;
}
