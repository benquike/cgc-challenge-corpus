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
extern "C" {
#include <libcgc.h>
#include <stdlib.h>
#include <string.h>
};

#include "orderiomanager.h"


#define BUF_SIZE 512

void print_banner()
{
    __printf("Welcome to the CGC Pizzeria order management system.\n");
}

int main()
{
    OrderIoManager oim;
    IoManager pwio(BUF_SIZE);
    bool authenticated = false, shutdown = false;
    int choice;
    int pw_attempts;

    while (!shutdown) {
        pw_attempts = 10;
        while (!authenticated) {
            __printf("Enter system password: ");
            if(!pwio.readline()) {
                __printf("Incorrect Password!\n");
            } else if (__strcmp("pizzapass", pwio.get_last_input()) == 0) {
            //else if (__strcmp("pp", pwio.get_last_input()) == 0) {
                authenticated = true;
                __printf("\n");
                print_banner();
            }  else if (__strcmp("quit", pwio.get_last_input()) == 0) {
                __printf("Received termination string\n");
                shutdown = true;
                break;
            } else {
                __printf("Incorrect Password!\n");
                if (!(--pw_attempts)) {
                    __printf("Too many bad attempts\n");
                    shutdown = true;
                    break;
                }
            }
        }

        while (authenticated) {
            __printf("1. Input Order\n");
            __printf("2. Update Order\n");
            __printf("3. View One Orders\n");
            __printf("4. View All Orders\n");
            __printf("5. Delete Order\n");
            __printf("6. Clear All Orders\n");
            __printf("7. Logout\n");
            __printf("Choice: ");

            choice = oim.readnum();
            switch(choice) {
            case 1:
                if(!oim.input_order(-1))
                    __printf("Wow. that failed pretty bad");
                break;
            case 2:
                oim.update_order();
                break;
            case 3:
                oim.view_single_order();
                break;
            case 4:
                oim.print_orders();
                break;
            case 5:
                if (oim.delete_order())
                    __printf("Succesfully removed order\n");
                break;
            case 6:
                oim.clear_all_orders();
                __printf("Cleared all previous orders\n");
                break;
            case 7:
                oim.clear_all_orders(true);
                __printf("Logging out\n");
                authenticated = false;
                break;
            default:
                __printf("Bad Selection\n");
            }
        }
    }
}
