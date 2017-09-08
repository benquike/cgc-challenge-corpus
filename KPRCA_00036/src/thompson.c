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
#include "constants.h"

#define OPERATOR "|*+?()"
#define TOKEN "|*+?()[]\\"

static int normalize_infix(unsigned char **pinfix, size_t size)
{
    unsigned char *new_str = *pinfix;
    unsigned char unions[200], i = 0;
    unsigned char *infix = *pinfix, *s, *e, *t = *pinfix;
    int error;
    for (; *infix ; infix++ ) {
        if ( !isprint(*infix) ) {
            error = BAD_CHARACTER;
            goto failed;
        }

        if (__strchr(OPERATOR, *infix) != NULL) {
            *infix += CHAR_OFFSET;
        } else if (*infix == '\\') {
            if (infix[1] != '\0' && __strchr(TOKEN, infix[1]) != NULL) {
                *infix++ = SKIP;
            } else {
                error = BAD_RE;
                goto failed;
            }
        } else if (*infix == '[') {
            int found = FALSE;
            i = 0;
            s = infix;
            unions[i++] = LPAREN;
            while (!found && *++infix) {
                if (*infix != ']') {
                    if (i > 196 ) {
                        error = BAD_RE;
                        goto failed;
                    }
                    unions[i++] = *infix;
                    unions[i++] = UNION;
                } else {
                    if (i <= 1 ) {
                        error = BAD_RE;
                        goto failed;
                    }
                    e = ++infix;
                    found = TRUE;
                }
            }
            if (!found) {
                error = BAD_RE;
                goto failed;
            }

            unions[i] = '\0';
            unions[--i] = RPAREN;
            new_str = __malloc(__strlen(t) + __strlen(unions) + 1);
            __memcpy(new_str, t, s - t);
            __memcpy(new_str + (s - t), unions, __strlen(unions));
            __memcpy(new_str + ((s - t) + __strlen(unions)), e, __strlen(e) + 1);
            infix = new_str + ((s - t) + __strlen(unions)) - 1;
            __free(t);
            t = new_str;
        }
    }

    *pinfix = new_str;
    return 0;

failed:
    return BAD_RE;
}

static unsigned char peek(unsigned char *list) {
    return *--list;
}

static int is_nonconcat_char(unsigned char c)
{
    return (c == UNION || c == STAR || c == PLUS || c == QMARK || c == RPAREN || c == SKIP);
}

void debug_print_re(unsigned char* re)
{
    unsigned char *temp = __malloc(__strlen(re) + 1);
    unsigned char *ptemp = temp, *pre = re;
    for( ; *re; re++)
        *temp++ = *re > CHAR_OFFSET ? *re - CHAR_OFFSET : *re;
    temp = '\0';

    __printf("Normalized re = %s\n", ptemp);
    __printf("Original re = %s\n", pre);
    __free(temp);
}

int retorpn(unsigned char *infix, size_t size, unsigned char **rpn)
{
    *rpn = NULL;
    if(__strlen(infix) == 0)
        return BAD_RE;
    else if(__strlen(infix) > MAX_RE_SIZE)
        return RE_TOO_LONG;
    else if(__strlen(infix) > size)
        return BUF_OVERFLOW;

    unsigned char *output = __calloc(1, (__strlen(infix) * 2) + 1); // Factor in all the concats
    unsigned char *operators = __calloc(1, __strlen(infix) + 1);
    unsigned char *out_iter = output;
    unsigned char *op_iter = operators;
    unsigned char *ninfix = __malloc(size);

    int error = 0;
    int bad_paren = 0;
    unsigned char c;

    __strcpy(ninfix, infix);
    infix = ninfix;
    error = normalize_infix(&infix, size);
    if (error != 0)
        goto failed;

    for (; *infix ; infix++ ) {
        switch(*infix) {
        case SKIP:
            break;
        case LPAREN:
            if (infix[1] == '\0' || is_nonconcat_char(infix[1])) {
                error = BAD_RE;
                goto failed;
            }

            PUSH(op_iter, *infix);
            continue;
        case RPAREN:
            bad_paren = 1;
            while(op_iter != operators) {
                if(peek(op_iter) != LPAREN) {
                    POP(op_iter, c);
                    PUSH(out_iter, c);
                } else {
                    POP(op_iter, c);
                    bad_paren = 0;
                    break;
                }
            }
            break;
        case UNION:
            if (out_iter == output || infix[1] == '\0' || is_nonconcat_char(infix[1])) {
                error = BAD_RE;
                goto failed;
            }

            while(op_iter != operators) {
                if(peek(op_iter) != LPAREN && peek(op_iter) != UNION) {
                    POP(op_iter, c);
                    PUSH(out_iter, c);
                } else {
                    break;
                }
            }
            PUSH(op_iter, *infix);
            break;
        case STAR:
        case PLUS:
        case QMARK:
            if (out_iter == output) {
                error = BAD_RE;
                goto failed;
            }
            while(op_iter != operators) {
                c = peek(op_iter);
                if(c == STAR || c == PLUS || c == QMARK) {
                    POP(op_iter, c);
                    PUSH(out_iter, c);
                } else {
                    break;
                }
            }
            PUSH(op_iter, *infix);
            break;
        default:
            PUSH(out_iter, *infix);
            break;
        }

        if (*infix != UNION && infix[1] != '\0' && !is_nonconcat_char(infix[1])) {
            while(op_iter != operators) {
                c = peek(op_iter);
                if(c == STAR || c == PLUS || c == QMARK || c == CONCAT) {
                    POP(op_iter, c);
                    PUSH(out_iter, c);
                } else {
                    break;
                }
            }
            PUSH(op_iter, CONCAT);
        }

        if (bad_paren) {
            error = BAD_RE;
            goto failed;
        }
    }

    while(op_iter != operators) {
        POP(op_iter, c);
        PUSH(out_iter, c);
    }

    *rpn = output;
    __free(ninfix);
    __free(operators);
    return 0;

failed:
    __free(ninfix);
    __free(output);
    __free(operators);
    return error;
}

