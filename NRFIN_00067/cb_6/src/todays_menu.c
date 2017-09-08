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
 * @param m     Menu
 * @param a     Appetizer to add to menu
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
 * @param m     Menu
 * @param meal  Meal to add to menu
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
 * @param m     Menu
 * @param d     Dessert to add to menu
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

    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "ZbWWNoNM";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "eDsELei";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "WSpBFJFOZgwszDffv";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "OjZiYi";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "AnGOULFOGC";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "cSOLkHLjNiSr";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "dNlwfQlb";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "Jdubqqhp";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "ypDpTEmYTJNbgwuAAuk";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "PxSfXOLhtqroMQEYuI";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "KGLHaywX";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "wTopgFWuWqFm";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "exxhtKZQ";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "JxQkslh";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "OKtn";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "sZlmZGiQPtIAh";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "KvBPV";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "YMiRPnuTZ";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "vSyFUX";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "koaVbUY";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "vGlLNtRWMBPrSQIGBl";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "VsdLokjwQPOmqX";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "uMcUfcpDItFDs";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "bdQ";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "Snt";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "BfJB";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "eJpMHmjqGbdtZTaC";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "BN";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "OhPbcBnJFNBsfKxc";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "MAEwIQ";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "FbZsPlRYOfBimieMN";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "tMyziPnmnv";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "ENGodOMkSCcdp";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "oW";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "jqBfeUjRYCpwb";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "I";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "ZTRhTQBKSyHSvnA";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "AAYAcqLF";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "GdO";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "OJGbM";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "CvqK";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "irsRWLUz";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "apwd";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "HbTGG";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "giSPcyLgGK";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "gBXDWHZEILFekw";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "XLWfktopVr";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "JhDmBevCueHq";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "tve";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    a = __malloc(sizeof(Appetizer));
    MALLOC_OK(a);
    __memset(a, '\0', sizeof(Appetizer));
    a->ftype = APP_TYPE;
    name = "aUWYXBp";
    __memcpy(a->name, name, __strlen(name, '\0'));
    add_appetizer(menu,a);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "Zj";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "elHXoEkGJHTfXydBj";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "BkngOfUoTmqtHOMAsSC";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "XjMM";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "eZlRse";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "NTOvguiPgjPlEc";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "RDyNWXZu";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "gPRF";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "XLmGtNwx";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "DRgdveNnWklqvg";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "peSjRFwkVUTWaBqkUZc";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "vEicQun";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "mEOcRZWoWATCb";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "A";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "uX";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "C";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "qk";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "DImoNaZRWzYbjunIA";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "HZQqWOurDrFhZYi";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "jfKqdlVACidsVYrKT";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "vhKkoZ";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "QluJoiBdiPSjTREbfCV";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "RGqRxjI";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "jdoZXCoKrCHis";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "KhVzoF";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "TvAMOyYghpritkQKt";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "YlkFYBvSqtQxU";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "ossUKKxCsEPwAQuRc";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "cGsWHxAeFkByvPuAnb";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "u";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "UYBeUiccXeIdacmo";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "skLRiJW";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "Dzs";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "hUN";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "RrvWQClGLc";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "E";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "LPWgXP";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "ZgQkrAsglrUFknMGX";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "MOoJtdbGKwI";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "CzfnjxBK";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "mD";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "Es";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "uqiFNVKzPFeoMm";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "GoAPHgyqAHYxl";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "DvCF";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "pHIEymqeAycJijjFFk";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "e";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "IyWZnR";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "QblFGrOfyuS";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "T";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "BZzN";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "GtrPWzKW";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "FXEHUTFUF";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "McSUxQepzTIeu";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "VIfO";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "qg";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "hqWsFPYCvuQ";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "fPIeUkgPhYG";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "mWWjU";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "NBmQapgAdNh";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "hEftYexRZsXkUncI";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "oWQHyVMOfSyaKEib";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "PHdBEJSkmyKM";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "WuHadHwnipvH";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "gbK";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "r";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "g";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "CHSfCaUqj";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "FyYzVa";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "X";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "ImwYuQymbIIGAb";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "vZmZhVnBnTwEUnuNKAh";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "QjLlpYhdEwffGsyObxr";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "mIUZITpftBvcMifY";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "EqSfGGewaUntOuFNlRQ";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "zWOFfCwKkwDTnIFX";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "E";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "AtjGgIxYASTCOC";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "aOVAVDAvqbpKPQzs";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "GTzDwFtOZBGsbbM";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "lZhdeTslbTBK";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "pwqEZDoh";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "gmpwZynj";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "cr";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "Hcb";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "Tqzl";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "VoiXIgGAtoNHMq";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "mROScjgKBz";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "caloY";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "KYlwUCwgkhh";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "MLxIEz";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "SxzeyKTHsODuUk";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "cwoMLL";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "sKykXwqm";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "NXaG";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "qryLVWWEwEx";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "hoPyZCWLbzlEdS";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "nJNOemZLRA";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "QqpfnCul";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "hKxFhOFanfnrmMMo";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "HBjxBFrofPKcLh";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "HQXkYe";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "HVtZCfTbqD";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "TrkhFsCcoSRkwftoT";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "tsiK";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "bYqcAspvb";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "zuFxOQEXKMHzJCHmTX";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "urVZe";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "hBfkvxWNYNKAWOzmasc";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "gxqlPTpMapFtlDNcN";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "svzMcxufKVy";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "oLvwcIJsXjzJYrnK";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "wuXVqAIWyTbaeTq";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "ucayTRX";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "jLIxECdyrhWfwuhZ";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "OFpbDRbVfx";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "zkbsnHsQYNpgrMw";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "TgHn";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "PrzysxmuxvWGic";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "LLEdnN";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "n";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "KoXHZsrHnUtQi";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "FriJyufIKULUYxS";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "KkUZp";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "kcIlkHphxKeZWiyh";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "wAkD";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "oNKz";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "QlpxCsSSsfAsT";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "IPZfoyxxrvYzwNmcndJ";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "HwOSnePDOXk";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "BNqrexKmxzwYqR";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "ZzTFYA";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "xGimYTMbTGurv";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "yg";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "fVAqrTHHLKaZimlJqjI";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "VbDaKZELJ";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "UmXu";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "ormjxbdGdKYIQejskBV";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "pSBIfScXYfA";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "OZIGfmINguZcpfi";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "vZgXTmEfJrxpukXLiy";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "iAQFY";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "iCw";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "ipsiSSCBtPZzgLWkRby";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "GatLliocExXqUDhGGg";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "BdCRYrsIW";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "UvmPPZRWNmeKuh";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "BUIfblmlpKYpnXzK";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "mxCEajsFOx";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "jMHek";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "oqBIFUSsfGLwuqp";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "tpQdTrVfwEbVqCGC";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "Ie";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "yvwHDjpJGkOO";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "eQgFSsFgTQtQT";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "VhbNDL";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "MW";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "pVAbrgTmfsf";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "NDkwNvbbHkmg";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "eqlYvUouwSFcet";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "PZVEoFhIc";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "tCfePusRu";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "d";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "IyghjY";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "AsY";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "pfvxYeTqc";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "ZTxaQoNQYaIBBY";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "YRbgJKV";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "k";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "rwUiJ";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "RrNSfpsriLJXJAn";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "YtC";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "EBzXJGYjGffCyBiPp";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "QSILluzPFyZZbEsE";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "wtddywMhBwD";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "mLyAdUlsMGFhP";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "vbWrs";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "xWBdzcvZQTf";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "QdjRhFJ";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "UhIQDYCNZ";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "mCObWa";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "An";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "AFygdlesQryFQJPbBiq";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "gWvmtTG";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "JssjKtCzNVAeX";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "IXrWDMTGKTAPatTSl";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "CtvtzAZHCYnxfk";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "TeWfgjhdilcO";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "HakKrdXeRCpQJ";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "FKnGclTRQFvjz";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "vJpUnutIkgpm";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "bLsFqgMw";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "VdI";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "rDnCfN";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "qe";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "EijGXvaPpbk";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    m = __malloc(sizeof(Meal));
    MALLOC_OK(m);
    __memset(m, '\0', sizeof(Meal));
    m->ftype = MEAL_TYPE;
    name = "wePMzfShzImZDOTgLc";
    __memcpy(m->name, name, __strlen(name, '\0'));
    main = "Zxpb";
    __memcpy(m->main, main, __strlen(main, '\0'));
    veggies = "YsiGeamwZgFEp";
    __memcpy(m->veggies, veggies, __strlen(veggies, '\0'));
    side = "HDeBa";
    __memcpy(m->side, side, __strlen(side, '\0'));
    add_meal(menu, m);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "AKPReWjkKZRnRsGvRDU";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "xvvfIjwJxxoX";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "EcVatQDxgHwEwd";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "xL";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "CGabmQUFBiTiyg";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "OLgmcumpKx";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "ZOrnojF";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "ejxWijyBhfjzOi";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "wZyEJHaU";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "SQGmFKASiGI";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "tmdsFsKDge";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "cOkXtVctEuoeCFHQGzT";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "FZSdmjxmSVL";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "dKRAFyzUnzoKLKD";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "fegevMNqQD";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "VCmTydTBVQCeLQ";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "loyAbN";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "JnOZlrmpv";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "YZdBWVBsuGQ";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "suwkMLdoZTwWHsivcVs";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "nvoEWCpENpxDVuUGh";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "kDnXpp";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "jsffGu";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "VSJGQCLjpWi";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "yaSAoGMePbTvXYszZpd";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "YQOfPYMomTxSMKIZJPj";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "pesQByVw";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "Y";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "jqkl";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "CrBjW";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "TWYFRqe";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "UalYcxsnHdUOwB";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "GRBycFmDIEcoTP";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "oleZre";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "kXwjlceRFOuSLDl";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "qWyXwZXJFkDH";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "GbjNxstDxKmrM";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "NMLxnTqga";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "PneNgwQjz";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "yQyOP";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "ozox";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "AcdZXwJxQVoZbt";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "smmGG";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "XR";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "Amb";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "hBKpQJTqUVKJ";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "gBuAyZnrOXNVwInMU";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "m";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "cJVIJ";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);
    //
    d = __malloc(sizeof(Dessert));
    MALLOC_OK(d);
    __memset(d, '\0', sizeof(Dessert));
    d->ftype = DES_TYPE;
    name = "Jhu";
    __memcpy(d->name, name, __strlen(name, '\0'));
    add_dessert(menu,d);

}

