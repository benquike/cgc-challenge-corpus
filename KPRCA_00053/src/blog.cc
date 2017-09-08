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
#include "blog.h"
#include "io.h"
#include "list.h"

extern "C" {
#include <string.h>
};

static List<User *> g_reg_users;

User *Blog::get_user(char *username)
{
    for (int i=0; i < g_reg_users.length(); i++) {
        if(strcasecmp(username, g_reg_users[i]->get_username()) == 0)
            return g_reg_users[i];
    }

    return nullptr;
}

void Blog::register_user()
{
    char username[16];
    char pw[16];
    char friend_code[16];
    User *user;

    __printf("Enter username: ");
    if (IO::readline()) {
        if (__strlen(IO::buf()) > 2 && __strlen(IO::buf()) < 16) {
            __strcpy(username, IO::buf());
        } else {
            __printf("Error: Username must be between 3 and 15 characters\n");
            return;
        }
    } else {
        __printf("Bad Input\n");
        return;
    }
    if (get_user(username) != nullptr) {
        __printf("Error: Username already exists. Try a different username\n");
        return;
    }

    while(true) {
        __printf("Enter password: ");
        if (IO::readline()) {
            if (__strlen(IO::buf()) > 4 && __strlen(IO::buf()) < 16) {
                __strcpy(pw, IO::buf());
                break;
            } else {
                __printf("Bad Password\n");
            }
        } else {
            __printf("Bad Input\n");
        }
    }

    while(true) {
        __printf("Friend code: ");
        if (IO::readline()) {
            if (__strlen(IO::buf()) > 4 && __strlen(IO::buf()) < 16) {
                __strcpy(friend_code, IO::buf());
                break;
            } else {
                __printf("Bad Friend Code\n");
            }
        } else {
            __printf("Bad Input\n");
        }
    }

    user = new User(username, pw, friend_code);
    g_reg_users.add(user);
}

User *Blog::log_in()
{
    char username[16];
    char pw[16];
    User *user = nullptr;
    __printf("Username: ");
    if (IO::readline()) {
        if (__strlen(IO::buf()) > 4 && __strlen(IO::buf()) < 16)
            __strcpy(username, IO::buf());
    } else {
        __printf("Bad input\n");
        return nullptr;
    }

    __printf("Password: ");
    if (IO::readline()) {
        if (__strlen(IO::buf()) > 4 && __strlen(IO::buf()) < 16)
            __strcpy(pw, IO::buf());
    } else {
        __printf("Bad input\n");
        return nullptr;
    }

    user = get_user(username);
    if (user && user->check_password(pw)) {
        __printf("Authenticated\n");
        return user;
    } else {
        __printf("Bad username/password combination\n");
        return nullptr;
    }
}

void Blog::add_friend(User *user)
{
    char username[16];
    char friend_code[16];
    User *new_friend = nullptr;

    if (!user) {
        __printf("Bad user\n");
        return;
    }

    __printf("Enter friend username: ");
    if (IO::readline()) {
        if (__strlen(IO::buf()) > 2 && __strlen(IO::buf()) < 16) {
            __strcpy(username, IO::buf());
            new_friend = get_user(username);
        } else {
            __printf("Error: Username must be between 3 and 15 characters\n");
            return;
        }
    }

    if (new_friend == nullptr) {
        __printf("Error: Couldn't get friend\n");
        return;
    }

    while(true) {
        __printf("Friend code: ");
        if (IO::readline()) {
            if (__strlen(IO::buf()) > 4 && __strlen(IO::buf()) < 16) {
                __strcpy(friend_code, IO::buf());
                break;
            } else {
                __printf("Bad Friend Code\n");
            }
        } else {
            __printf("Bad Input\n");
        }
    }

    if (user->add_friend(new_friend, friend_code))
        __printf("Added friend, %s!\n", username);
    else
        __printf("Couldn't add as friend.\n");
}


void Blog::view_user_profile(User *user)
{
    if (!user) {
        __printf("Enter username: ");
        if (IO::readline()) {
            if (__strlen(IO::buf()) > 2 && __strlen(IO::buf()) < 16) {
                user = get_user(IO::buf());
            }
        }

        if (!user) {
            __printf("Bad Username\n");
            return;
        }
    }

    user->print_profile();
}

void Blog::view_user_posts(User *reg_user, User *user, int post_selection)
{
    if (!user) {
        __printf("Enter username: ");
        if (IO::readline()) {
            if (__strlen(IO::buf()) > 2 && __strlen(IO::buf()) < 16) {
                user = get_user(IO::buf());
            }
        }

        if (!user) {
            __printf("Bad Username\n");
            return;
        }
    }

    if (!post_selection) {
        __printf("Select a post id to view\n");
        __printf("0. Go Back\n");
        user->list_posts();
        __printf("Selection: ");
        post_selection = IO::readnum();
        if (post_selection == 0) {
            __printf("Returning to main menu\n");
            return;
        }
    }

    if( post_selection > user->num_posts()) {
        __printf("Bad Post Selection\n");
        return;
    }

    user->print_post(post_selection);
    if (reg_user && reg_user != user) {
        int like_selection = 0;
        __printf("Like Post?\n");
        __printf("1. Yes\n");
        __printf("2. No\n");
        __printf("Selection: ");
        like_selection = IO::readnum();
        if (like_selection == 1) {
            user->like_post(reg_user, post_selection);
        }
    }
}
