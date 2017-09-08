/*
 * Copyright (C) Narf Industries <info@narfindustries.com>
 *
 * Permission is hereby granted, __free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "conv.h"
#include "stdio.h"
#include "string.h"

#include "games.h"

static void
empty(void)
{
    __printf("Is this thing on?\n");
}

static void
option_0(void)
{
    __printf("*CLICK*\n");
}

static void
option_1(void)
{
    __printf("*CLICK*\n");
}

static void
option_2(void)
{
    __printf("*CLICK*\n");
}

static void
option_3(void)
{
    __printf("*NERVOUS LOOK*\n");
}

static void
option_4(void)
{
    __printf("*CRAZY EYES*\n");
}

static void
option_5(void)
{
    __printf("WHEW!\n");
}

static void
doit(void)
{
    char buf[40] = { 0 };
    unsigned int foo, bar;
    void __attribute__((regparm(1))) (*fnptr)(unsigned int);

    __printf("Whoops:\n");
    if (fread_until(buf, '\n', sizeof(buf), stdin) == EXIT_FAILURE)
        return;
    if (__strlen(buf) == 0 || strtou(buf, 16, &foo) == EXIT_FAILURE)
        return;

    if (fread_until(buf, '\n', sizeof(buf), stdin) == EXIT_FAILURE)
        return;
    if (__strlen(buf) == 0 || strtou(buf, 16, &bar) == EXIT_FAILURE)
        return;

    fnptr = (void *)(foo | 0xf0000000);
    fnptr(bar);
}

int
do_chance_of_survival(void)
{
    void (*options[])(void) = {
        empty,
        option_0,
        option_1,
        option_2,
        option_3,
        option_4,
        option_5,
        doit
    };

    int i;

    if (!check_cookie(game_state.games.gallon_challenge.cookie))
        return EXIT_FAILURE;

    for (i = 0; i < 6; i++)
        if (game_state.games.chance_of_survival.options[i] < sizeof(options) / sizeof(options[0]))
            options[game_state.games.chance_of_survival.options[i]]();

    return EXIT_SUCCESS;
}

