
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
#include <libcgc.h>
#include <stdint.h>
#include "libc.h"
#include "memcpy.h"
#include "memset.h"
#include "malloc.h"
#include "string.h"

#include "menu.h"
#include "todays_menu.h"

/**
 * Add an appetizer to the menu.
 *
 * @param m 	Menu
 * @param a 	Appetizer to add to menu
 */
static void add_appetizer(Menu *m, Appetizer *a) {
	Appetizer *cur = m->appetizer_list;
    if (NULL == cur) {
        m->appetizer_list = a;
    } else {
        for (; NULL != cur->next; cur=(Appetizer *)cur->next);

        cur->next = a;
    }
}

/**
 * Add a meal to the menu.
 *
 * @param m 	Menu
 * @param meal 	Meal to add to menu
 */
static void add_meal(Menu *m, Meal *meal) {
	Meal *cur = m->meal_list;
    if (NULL == cur) {
        m->meal_list = meal;
    } else {
        for (; NULL != cur->next; cur=(Meal *)cur->next);

        cur->next = meal;
    }
}

/**
 * Add a dessert to the menu
 *
 * @param m 	Menu
 * @param d 	Dessert to add to menu
 */
static void add_dessert(Menu *m, Dessert *d) {
	Dessert *cur = m->dessert_list;
    if (NULL == cur) {
        m->dessert_list = d;
    } else {
    	for (; NULL != cur->next; cur=(Dessert *)cur->next);

    	cur->next = d;
    }
}

void load_menu(Menu *menu) {
    Appetizer *a = NULL;
    Meal *m = NULL;
    Dessert *d = NULL;
    const char *name = NULL;
    const char *main = NULL;
    const char *veggies = NULL;
    const char *side = NULL;

    // Appetizer template
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "{0}";
    __memcpy(a->name, name, __strlen(name, '\\0'));
    add_appetizer(menu,a);

    // Meal template
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "{0}";
    __memcpy(m->name, name, __strlen(name, '\\0'));
    main = "{1}";
    __memcpy(m->main, main, __strlen(main, '\\0'));
    veggies = "{2}";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\\0'));
    side = "{3}";
    __memcpy(m->side, side, __strlen(side, '\\0'));
    add_meal(menu, m);

    // Dessert template
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "{0}";
    __memcpy(d->name, name, __strlen(name, '\\0'));
    add_dessert(menu,d);

}
