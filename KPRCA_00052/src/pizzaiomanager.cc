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
#include "pizzaiomanager.h"
#include "pizzapie.h"
#include "pizzabowl.h"
#include "pizzasub.h"
#include "sauce.h"
#include "cheese.h"
#include "meat.h"
#include "veggie.h"

PizzaIoManager::PizzaIoManager(Pizza *_pizza)
{
    pizza = _pizza;
}

PizzaIoManager::~PizzaIoManager()
{
  pizza = (Pizza *)NULL;
}

bool PizzaIoManager::create_pizza_pie()
{
    int choice;
    while (true) {
        __printf("Select Size\n");
        __printf("1. Small\n");
        __printf("2. Medium\n");
        __printf("3. Large\n");
        __printf("Choice: ");

        choice = readnum();
        if (choice < 1 || choice > 3) {
            __printf("Bad Selection\n");
            continue;
        }
        break;
    }

    pizza = new PizzaPie(choice);
    if (!pizza) {
        __printf("Error creating a pizza right now. Please try again later\n");
        return false;
    }
    __printf("Successfully added a new Pizza Pie!\n");
    return true;
}

bool PizzaIoManager::create_pizza_sub()
{
    int sub_size, bread_type, choice;
    while (true) {
        __printf("Select Size\n");
        __printf("1. 6 inch\n");
        __printf("2. 12 inch\n");
        __printf("Choice: ");

        choice = readnum();
        if (choice < 1 || choice > 2) {
            __printf("Bad Selection\n");
            continue;
        }
        sub_size = choice;
        break;
    }

    while (true) {
        __printf("Select Bread Type\n");
        __printf("1. White\n");
        __printf("2. Wheat\n");
        __printf("Choice: ");

        choice = readnum();
        if (choice < 1 || choice > 2) {
            __printf("Bad Selection\n");
            continue;
        }
        bread_type = choice;
        break;
    }

    pizza = new PizzaSub(sub_size, bread_type);
    if (!pizza) {
        __printf("Error creating a pizza right now. Please try again later\n");
        return false;
    }
    __printf("Successfully added a new Pizza Sub!\n");
    return true;
}

bool PizzaIoManager::create_pizza_bowl()
{
    int choice;
    bool side_of_bread;
    while (true) {
        __printf("Side of bread?\n");
        __printf("1. Yes\n");
        __printf("2. No\n");
        __printf("Choice: ");

        choice = readnum();
        switch (choice) {
        case 1:
            side_of_bread = true; break;
        case 2:
            side_of_bread = false; break;
        default:
            __printf("Bad Selection\n");
            continue;
        }
        break;
    }

    pizza = new PizzaBowl(side_of_bread);
    if (!pizza) {
        __printf("Error creating a pizza right now. Please try again later\n");
        return false;
    }
    __printf("Successfully added a new Pizza Bowl!\n");
    return true;
}

void PizzaIoManager::add_toppings()
{
    Topping *topping = (Topping *)NULL;
    int choice;
    bool done_adding_toppings = false;

    while(!done_adding_toppings) {
        __printf("Select topping type:\n");
        __printf("1. Cheese\n");
        __printf("2. Meat\n");
        __printf("3. Veggies\n");
        __printf("4. No More Toppings\n");
        __printf("Choice: ");

        choice = readnum();
        switch(choice) {
        case 1:
            Cheese::list_options();
            __printf("Enter topping name: ");
            if(readline())
                topping = Cheese::select_cheese(get_last_input());
            else
                __printf("Failed reading input\n");
            break;
        case 2:
            Meat::list_options();
            __printf("Enter topping name: ");
            if(readline())
                topping = Meat::add_meat(get_last_input());
            else
                __printf("Failed reading input\n");
            break;
        case 3:
            Veggie::list_options();
            __printf("Enter topping name: ");
            if(readline())
                topping = Veggie::pick_veggie(get_last_input());
            else
                __printf("Failed reading input\n");
            break;
        case 4:
            done_adding_toppings = true;
            continue;
        default:
            __printf("Bad Choice\n");
            continue;
        }

        if (topping) {
            pizza->add_topping(topping);
            __printf("Added topping\n");
        } else {
            __printf("Bad topping name\n");
        }
    }
}

void PizzaIoManager::remove_toppings()
{
    int choice;
    bool done_removing_toppings = false;

    while (!done_removing_toppings) {
        if (!pizza->get_num_toppings()) {
            __printf("No Toppings to remove\n");
            return;
        }

        __printf("Toppings Added\n");
        __printf("\t0. Cancel\n");
        pizza->print_toppings();
        __printf("Choice: ");
        choice = readnum();

        if (choice == 0) {
            __printf("Finished removing toppings\n");
            return;
        } else if (choice > pizza->get_num_toppings()) {
            __printf("Bad Selection\n");
        } else {
            __printf("Removed Topping\n");
            pizza->remove_topping(--choice);
        }
    }
}

void PizzaIoManager::add_sauces()
{
    Sauce *sauce = (Sauce *)NULL;
    int choice;
    bool done_adding_sauces = false;

    while(!done_adding_sauces) {
        __printf("Select an option:\n");
        __printf("1. Add Sauce\n");
        __printf("2. No More Sauces\n");
        __printf("Choice: ");

        choice = readnum();
        switch(choice) {
        case 1:
            Sauce::list_options();
            __printf("Enter sauce name: ");
            if(readline())
                sauce = Sauce::pour_sauce(get_last_input());
            else
                __printf("Failed reading input\n");
            break;
        case 2:
            done_adding_sauces = true; continue;
        default:
            __printf("Bad Choice\n");
            continue;
        }

        if (sauce) {
            pizza->add_sauce(sauce);
            __printf("Added sauce\n");
        } else {
            __printf("Bad sauce name\n");
        }
    }
}

void PizzaIoManager::remove_sauces()
{
    int choice;
    bool done_removing_sauces = false;

    while (!done_removing_sauces) {
        if (!pizza->get_num_sauces()) {
            __printf("No sauces to remove\n");
            return;
        }

        __printf("Sauces on the side\n");
        __printf("\t0. Cancel\n");
        pizza->print_sauces();
        __printf("Choice: ");
        choice = readnum();

        if (choice == 0) {
            __printf("Finished removing sauces\n");
            return;
        } else if (choice > pizza->get_num_sauces()) {
            __printf("Bad Selection\n");
        } else {
            __printf("Removed sauce\n");
            pizza->remove_sauce(--choice);
        }
    }
}

bool PizzaIoManager::new_pizza()
{
    int choice;

    while (true) {
        __printf("Choose what the kind of pizza\n");
        __printf("1. Pizza Pie - The classic!\n");
        __printf("2. Pizza Sub - All the fun, on a bun\n");
        __printf("3. Pizza Bowl - Our own twist\n");
        __printf("Choice: ");
        choice = readnum();
        switch(choice) {
        case 1:
            create_pizza_pie(); break;
        case 2:
            create_pizza_sub(); break;
        case 3:
            create_pizza_bowl(); break;
        default:
            __printf("Bad Choice\n");
            continue;
        }

        break;
    }

    if (edit_pizza()) {
        __printf("Successfully added pizza!\n");
        return true;
    } else {
        __printf("Error: Try again later\n");
        return false;
    }
}

bool PizzaIoManager::edit_pizza(Pizza *_pizza)
{
    if (_pizza)
        pizza = _pizza;

    if (!pizza) {
        __printf("No Pizza to edit\n");
        return false;
    }

    int choice;
    bool done_editing = false;
    while (!done_editing) {
        __printf("Select an option:\n");
        __printf("1. Add Toppings\n");
        __printf("2. Remove Toppings\n");
        __printf("3. Add Sauce\n");
        __printf("4. Remove Sauce\n");
        __printf("5. Finished With Pizza\n");
        __printf("Choice: ");
        choice = readnum();
        switch(choice) {
        case 1:
            add_toppings(); break;
        case 2:
            remove_toppings(); break;
        case 3:
            add_sauces(); break;
        case 4:
            remove_sauces(); break;
        case 5:
            done_editing = true; break;
        default:
            __printf("Bad Choice\n");
            continue;
        }
    }

    return true;
}

Pizza *PizzaIoManager::get_pizza()
{
    Pizza *ppizza = pizza;
    pizza = (Pizza *)NULL;
    return ppizza;
}
