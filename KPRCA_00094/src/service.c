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
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "user.h"
#include "ctf.h"

unsigned int r;
unsigned int prng()
{
    r = ((((r >> 31) ^ (r >> 30) ^ (r >> 10) ^ (r >> 0)) & 1) << 31) | (r >> 1);
    return r;
}

void random_string(char *buf, size_t len)
{
    int i;
    int r;
    char str[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (i = 0; i < len; ++i)
    {
        r = prng();
        buf[i] = str[r % (sizeof(str) - 1)];
    }
}

void print_main_menu()
{
    __printf("\n=== Menu ===================\n");
    __printf("1. Register User\n");
    __printf("2. Login User\n");
    __printf("3. Quit\n");
    __printf("> ");
}

int is_alphanum(const char *s)
{
    int i;
    for (i = 0; i < __strlen(s) - 1; ++i)
    {
        if (!__isalnum(s[i]))
            return 0;
    }
    return 1;
}

void handle_register_user(ctf_t *ctf)
{
    char shout[128], buf[256];
    user_t *user = NULL;
    team_t *team = NULL;
    int new_team = 0;
    error_t err;
    if ((err = user_new(&user)) != ERR_OK)
        goto fail;
    __printf("\n=== User Creation ===================\n");
    __printf("Nickname: ");
    err = ERR_INVALID_VALUE;
    fflush(stdout);
    if (freaduntil(buf, sizeof(buf), '\n', stdin) <= 0)
        goto fail;
    if (!is_alphanum(buf))
        goto fail;
    if ((err = user_set_nick(user, buf)) != ERR_OK)
        goto fail;
    __printf("Password: ");
    err = ERR_INVALID_VALUE;
    fflush(stdout);
    if (freaduntil(buf, sizeof(buf), '\n', stdin) <= 0)
        goto fail;
    if (!is_alphanum(buf))
        goto fail;
    if ((err = user_set_pass(user, buf)) != ERR_OK)
        goto fail;
    __printf("Team token: ");
    fflush(stdout);
    if (freaduntil(buf, sizeof(buf), '\n', stdin) < 0)
        goto fail;
    if (__strcmp(buf, "") == 0)
    {
        if ((err = team_new(&team)) != ERR_OK)
            goto fail;
        __printf("\n=== Team Creation ===================\n");
        __printf("Team name: ");
        err = ERR_INVALID_VALUE;
        fflush(stdout);
        if (freaduntil(buf, sizeof(buf), '\n', stdin) <= 0)
            goto fail;
        if ((err = team_change_name(team, buf)) != ERR_OK)
            goto fail;
        __printf("Shoutout: ");
        err = ERR_INVALID_VALUE;
        fflush(stdout);
#ifdef PATCHED_1
        if (freaduntil(shout, sizeof(shout), '\n', stdin) <= 0)
#else
        if (freaduntil(shout, sizeof(buf), '\n', stdin) <= 0)
#endif
            goto fail;
        if ((err = team_set_shoutout(team, shout)) != ERR_OK)
            goto fail;
        random_string(team->code, sizeof(team->code) - 1);
        team->code[sizeof(team->code) - 1] = '\0';
        new_team = 1;
    }
    else
    {
        int i;
        for (i = 0; i < ctf->num_teams; ++i)
        {
            if (memcmp(ctf->teams[i]->code, buf, 32) == 0)
            {
                team = ctf->teams[i];
                break;
            }
        }
        if (i == ctf->num_teams)
        {
            err = ERR_NO_SUCH_TEAM;
            goto fail;
        }
    }
    if ((err = ctf_add_user(ctf, user)) != ERR_OK)
        goto fail;
    if (new_team && (err = ctf_add_team(ctf, team)) != ERR_OK)
        goto fail;
    if ((err = team_add_member(team, user)) != ERR_OK)
        goto fail;
    if ((err = user_set_team(user, team, NULL)) != ERR_OK)
        goto fail;
    __printf("[INFO] Successfully registered.\n");
    if (new_team)
        __printf("[INFO] Team code: ;s\n", team->code);
    return;

fail:
    __printf("[ERROR] ;s\n", error_to_string(err));
    if (user)
        __free(user);
    if (new_team && team)
        __free(team);
    return;
}

void handle_login_user(ctf_t *ctf)
{
    error_t err;
    char nick[64], pass[64];
    __printf("\n=== Login ===================\n");
    __printf("Nick: ");
    err = ERR_INVALID_VALUE;
    fflush(stdout);
    if (freaduntil(nick, sizeof(nick), '\n', stdin) <= 0)
        goto fail;
    if (!is_alphanum(nick))
        goto fail;
    __printf("Password: ");
    fflush(stdout);
    if (freaduntil(pass, sizeof(pass), '\n', stdin) <= 0)
        goto fail;
    if (!is_alphanum(pass))
        goto fail;
    if ((err = ctf_auth_user(ctf, &ctf->logged_in, nick, pass)) != ERR_OK)
        goto fail;
    __printf("[INFO] Successfully logged in as ;s.\n", nick);
    return;

fail:
    __printf("[ERROR] ;s\n", error_to_string(err));
}

void print_ctf_ticker(ctf_t *ctf)
{
    team_t *t = ctf->logged_in->team;
    size_t n = 1;
    team_t **tmp = NULL;
    ctf_get_ranks(ctf, &tmp, &n);
    __free(tmp);
    __printf("\n================================\n");
    __printf("| [;s] - ;d\n", t->name, t->score);
    __printf("================================\n");
    int i;
    flag_t **solves = NULL;
    n = 5;
    ctf_get_solves(ctf, &solves, &n);
    if (n == 0)
        __printf("No solves yet.\n");
    for (i = 0; i < n; ++i)
        __printf("| ;s solved ;s (;d pts)\n", solves[i]->team->name, solves[i]->chal->name, solves[i]->chal->points);
    if (solves)
        __free(solves);
    __printf("================================\n");
}

void print_ctf_menu(ctf_t *ctf)
{
    print_ctf_ticker(ctf);
    __printf("\n=== CTF Menu ===================\n");
    __printf("1. View challenge list\n");
    __printf("2. View challenge detail\n");
    __printf("3. View ranking\n");
    __printf("4. View team profile\n");
    __printf("5. Submit flag\n");
    __printf("6. Logout\n");
    __printf("> ");
}

void print_challenge(ctf_t *ctf, chal_t *chal, int detail)
{
    const char *cat = chal_cat_to_string(chal->cat);
    chal_stat_t stat = chal->status;
    if (flg_team_did_solve(&ctf->flg, ctf->logged_in->team, chal))
        stat = CSTAT_SOLVED_U;
    const char *status = chal_status_to_string(stat);
    if (!detail)
        __printf("[;s] [;d pts] ;s - ;s\n", cat, chal->points, chal->name, status);
    else
    {
        __printf("[;s] [;d pts] ;s - ;s\n", cat, chal->points, chal->name, status);
        if (chal->status == CSTAT_LOCKED)
            __printf("Hidden.\n");
        else
            __printf(";s\n", chal->desc);
        //__printf("flag: ;s\n", chal->flag);
        flag_t **f = NULL;
        size_t n = 3;
        flg_get_solves(&ctf->flg, &f, chal, &n);
        if (n > 0)
        {
            __printf("\nTop Solvers\n");
            __printf("===========\n");
            int i;
            for(i = 0; i< n; ++i)
                __printf(";d. ;s\n", i+1, f[i]->team->name);
            __free(f);
        }
    }
}

void handle_view_challenge_list(ctf_t *ctf)
{
    int i;
    if (ctf->num_chals == 0)
    {
        __printf("[INFO] No challenges are available.\n");
        return;
    }
    __printf("\n=== Challenge list ===================\n");
    for (i = 0; i < ctf->num_chals; ++i)
    {
        __printf(";d. ", i);
        print_challenge(ctf, ctf->chals[i], 0);
    }
}

void handle_view_challenge_detail(ctf_t *ctf)
{
    char buf[32];
    handle_view_challenge_list(ctf);
    if (ctf->num_chals == 0)
        return;
    __printf("\nChoose idx: ");
    fflush(stdout);
    if (freaduntil(buf, sizeof(buf), '\n', stdin) < 0)
        goto fail;
    error_t err;
    chal_t *chal = NULL;
    if ((err = ctf_get_chal(ctf, &chal, __strtoul(buf, NULL, 10))) != ERR_OK)
        goto fail;
    __printf("\n=== Challenge detail ===================\n");
    print_challenge(ctf, chal, 1);
    return;

fail:
    __printf("[ERROR] ;s\n", error_to_string(err));
}

void view_ranking_page(ctf_t *ctf, size_t page)
{
    int i;
    size_t n = (5 < ctf->num_teams - (page - 1) * 5) ? 5 : (ctf->num_teams - (page - 1) * 5);
    team_t **ranks = NULL;
    ctf_get_ranks(ctf, &ranks, &page);
    for (i = 0; i < n; ++i)
    {
        int rank = i + page * 5 + 1;
        __printf(";d. ;s (;d pts)\n", rank, ranks[i]->name, ranks[i]->score);
    }
    __free(ranks);
}

void handle_view_ranking(ctf_t *ctf)
{
    char buf[32];
    error_t err = ERR_INVALID_VALUE;
    __printf("\n=== Ranking ===================\n");
    size_t page = 1;
    size_t num_pages = (ctf->num_teams / 5) + ((ctf->num_teams % 5) == 0 ? 0 : 1);
    view_ranking_page(ctf, 1);
    __printf("\n=== 1 / ;d ===================\n", num_pages);
    while (1)
    {
        __printf("\nPage: ");
        fflush(stdout);
        if (freaduntil(buf, sizeof(buf), '\n', stdin) < 0)
            goto fail;
        if (__strcmp(buf, "q") == 0)
            return;
        page = __strtoul(buf, NULL, 10);
        if (page < 1 || page > num_pages)
            goto fail;
        view_ranking_page(ctf, page);
        __printf("\n=== ;d / ;d ===================\n", page, num_pages);
    }
    return;

fail:
    __printf("[ERROR] ;s\n", error_to_string(err));
}

void handle_view_team_profile(ctf_t *ctf)
{
    char buf[64];
    error_t err = ERR_NO_SUCH_TEAM;
    __printf("\nTeam name: ");
    fflush(stdout);
    if (freaduntil(buf, sizeof(buf), '\n', stdin) <= 0)
        goto fail;
    int i;
    team_t *t = NULL;
    for (i = 0; i < ctf->num_teams; ++i)
    {
        if (__strcmp(ctf->teams[i]->name, buf) == 0)
        {
            t = ctf->teams[i];
            break;
        }
    }
    if (!t)
        goto fail;
    __printf("\n=== Team profile ===================\n");
    __printf(";s\n", t->name);
    __printf("Leader: ;s\n", t->leader->nick);
    __printf("Members (;d)\n - ", t->num_members);
    for (i = 0; i < t->num_members; ++i)
    {
        __printf(";s", t->members[i]->nick);
        if (i != t->num_members - 1)
            __printf(", ");
    }
    __printf("\nShoutout: ;s\n", t->shout);
    return;

fail:
    __printf("[ERROR] ;s\n", error_to_string(err));
}

void handle_submit_flag(ctf_t *ctf)
{
    char buf[512];
    error_t err = ERR_INVALID_FLAG;
    __printf("\n=== Submit flag ===================\n");
    __printf("Flag: ");
    fflush(stdout);
    if (freaduntil(buf, sizeof(buf), '\n', stdin) <= 0)
        goto fail;
    if ((err = ctf_submit_flag(ctf, ctf->logged_in->team, buf)) != ERR_OK)
        goto fail;
    chal_t *chal = NULL;
    flg_find_chal(&ctf->flg, &chal, buf);
    __printf("[INFO] Congrats! You scored ;d points!\n", chal->points);
    return;
fail:
    __printf("[ERROR] ;s\n", error_to_string(err));
}

void handle_logout_user(ctf_t *ctf)
{
    __printf("[INFO] Successfully logged out.\n");
    ctf->logged_in = NULL;
}

void handle_quit()
{
    __printf("Bye.\n");
    fflush(stdout);
    __exit(0);
}

void do_random_action(ctf_t *ctf)
{
    ctf->status = CTF_STAT_LIVE;
    switch (prng() % 10)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            /* Create a random challenge */
            {
                chal_t *chal = (chal_t *) __malloc(sizeof(chal_t));
                random_string(chal->name, 32);
                chal->name[32] = '\0';
                random_string(chal->desc, 64);
                chal->desc[64] = '\0';
                chal->points = prng() % 600;
                if (chal->points == 0)
                    chal->points = 1;
                chal->status = CSTAT_LOCKED;
                chal->cat = prng() % 7;
                chal->flag = __malloc(64);
                random_string(chal->flag, 32);
                chal->flag[32] = '\0';
                ctf_add_chal(ctf, chal);
            }
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            /* Open a random challenge */
            {
                int i;
                for (i = 0; i < ctf->num_chals; ++i)
                {
                    chal_t *c = ctf->chals[i];
                    if (c->status == CSTAT_LOCKED)
                    {
                        if (prng() % 100 < 50)
                        {
                            ctf_open_chal(ctf, i);
                            break;
                        }
                    }
                }
            }
            break;
        case 8:
            /* Make CTF down */
            ctf->status = CTF_STAT_DOWN;
            break;
        case 9:
            break;
    }
}

int __attribute__((fastcall)) main(int secret_page_i, char *unused[]) {
    void *secret_page = (void *)secret_page_i;
    char buf[256];

    r = *(unsigned int *)secret_page ^ *(unsigned int *)&secret_page[20];

    fbuffered(stdin, 1);
    fbuffered(stdout, 1);

    fxlat(stdin, "7943");
    fxlat(stdout, "7943");

    ctf_t ctf;
    ctf_init(&ctf);

    __printf("Shout CTF CLI v0.1\n");
    __printf("=================\n");
    fflush(stdout);
    while (1)
    {
        do_random_action(&ctf);
        if (!ctf.logged_in)
        {
            print_main_menu();
            fflush(stdout);
            if (freaduntil(buf, sizeof(buf), '\n', stdin) < 0)
                break;
            if (ctf.status == CTF_STAT_DOWN)
            {
                __printf("CTF server is down! Try again later :(\n");
                continue;
            }
            if (__strcmp(buf, "1") == 0)
                handle_register_user(&ctf);
            else if (__strcmp(buf, "2") == 0)
                handle_login_user(&ctf);
            else if (__strcmp(buf, "3") == 0)
                handle_quit();
            else
                __printf("Try again.\n");
        }
        else
        {
            print_ctf_menu(&ctf);
            fflush(stdout);
            if (freaduntil(buf, sizeof(buf), '\n', stdin) < 0)
                break;
            if (ctf.status == CTF_STAT_DOWN)
            {
                __printf("CTF server is down! Try again later :(\n");
                continue;
            }
            if (__strcmp(buf, "1") == 0)
                handle_view_challenge_list(&ctf);
            else if (__strcmp(buf, "2") == 0)
                handle_view_challenge_detail(&ctf);
            else if (__strcmp(buf, "3") == 0)
                handle_view_ranking(&ctf);
            else if (__strcmp(buf, "4") == 0)
                handle_view_team_profile(&ctf);
            else if (__strcmp(buf, "5") == 0)
                handle_submit_flag(&ctf);
            else if (__strcmp(buf, "6") == 0)
                handle_logout_user(&ctf);
            else
                __printf("Try again.\n");
        }
    }

    fflush(stdout);
    return 0;
}
