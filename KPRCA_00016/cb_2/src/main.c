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
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "user.h"
#include "msg_queue.h"
#include "packet.h"
#include "request.h"
#include "response.h"
#include "message.h"
#include "util.h"
#include "coffee.h"

#define MAX_NUM_MSG 100

#define SFD_SERVER  3
#define SFD_CLIENT 4

user_t cur_user;

int is_alnum_str(char *str)
{
  int i, ret = 0;
  for (i = 0; i < __strlen(str); ++i)
    ret |= !__isalnum(str[i] & 0xFF);
  return !ret;
}

void quit()
{
  quit_req_t q;
  __fdprintf(STDOUT, "[INFO] Bye!\n");
  q.code = 0;
  send_request(&q, CMD_QUIT);
  __exit(0);
}

void help()
{
  __fdprintf(STDOUT, "---- Commands --------------------\n");
  __fdprintf(STDOUT, " 'h'elp - display this help menu\n");
  __fdprintf(STDOUT, " 'r'egister - register a user\n");
  __fdprintf(STDOUT, " 'l'ogin - login\n");
  __fdprintf(STDOUT, " 'o'ut - logout\n");
  __fdprintf(STDOUT, " 'e'numerate - list messages\n");
  __fdprintf(STDOUT, " 'v'iew - view a message\n");
  __fdprintf(STDOUT, " 's'end - send a message\n");
  __fdprintf(STDOUT, " 'd'elete - delete a message\n");
  __fdprintf(STDOUT, " 'q'uit - quit\n");
  __fdprintf(STDOUT, "----------------------------------\n");
}

void register_user()
{
  packet_t *p = NULL;
  response_t res;
  register_req_t reg;

  __fdprintf(STDOUT, "\n[[[ REGISTER ]]]\n");
  __fdprintf(STDOUT, "- username: ");
  if (read_until(STDIN, reg.username, MAX_USERNAME_LEN, '\n') < 0)
    goto fail;
  __fdprintf(STDOUT, "- password: ");
  if (read_until(STDIN, reg.password, MAX_PASSWORD_LEN, '\n') < 0)
    goto fail;
  __fdprintf(STDOUT, "- password confirm: ");
  if (read_until(STDIN, reg.password_confirm, MAX_PASSWORD_LEN, '\n') < 0)
    goto fail;

  send_request(&reg, CMD_REGISTER);
  __fdprintf(STDOUT, "\n");

  p = parse_packet(SFD_CLIENT);
  if (p == NULL)
      goto fail;
  if (p->body_len < 2 * sizeof(int))
      goto fail;
  __memcpy(&res, p->body, 2 * sizeof(int));
  if (res.data_len > 0)
      res.data = p->body + 2 * sizeof(int);
  handle_response(p->command, &res);

fail:
  if (p)
    __free(p);
}

void logout_user()
{
  if (cur_user.user_id == 0)
    __fdprintf(STDOUT, "[ERROR] Not logged in.\n");
  else
  {
    __memset(cur_user.username, 0, MAX_USERNAME_LEN);
    __memset(cur_user.password, 0, MAX_PASSWORD_LEN);
    cur_user.user_id = 0;
    cur_user.auth_code = 0;
    __fdprintf(STDOUT, "[INFO] Successfully logged out.\n");
  }
}

void login_user()
{
  packet_t *p = NULL;
  response_t res;
  login_req_t login;

  __fdprintf(STDOUT, "\n[[[ LOGIN ]]]\n");
  __fdprintf(STDOUT, "- username: ");
  if (read_until(STDIN, login.username, MAX_USERNAME_LEN, '\n') < 0)
    goto fail;
  __fdprintf(STDOUT, "- password: ");
  if (read_until(STDIN, login.password, MAX_PASSWORD_LEN, '\n') < 0)
    goto fail;

  send_request(&login, CMD_LOGIN);

  p = parse_packet(SFD_CLIENT);
  if (p == NULL)
      goto fail;
  if (p->body_len < 2 * sizeof(int))
      goto fail;
  __memcpy(&res, p->body, 2 * sizeof(int));
  if (res.data_len > 0)
      res.data = p->body + 2 * sizeof(int);
  if (handle_response(p->command, &res) == 0)
  {
    if (p->body_len - 2 * sizeof(int) >= res.data_len &&
        res.data_len >= sizeof(short) + sizeof(int))
    {
      if (cur_user.user_id != 0)
        logout_user();
      __strcpy(cur_user.username, login.username);
      __strcpy(cur_user.password, login.password);
      cur_user.user_id = *((short *) res.data);
      cur_user.auth_code = *((int *) &res.data[2]);
    }
  }

fail:
  if (p)
    __free(p);
}

int list_messages()
{
  short num_msg = 0, i;
  packet_t *p = NULL;
  response_t res;
  list_req_t list;

  if (cur_user.user_id == 0)
  {
    __fdprintf(STDOUT, "[ERROR] Login first, please.\n");
    goto fail;
  }

  list.user_id = cur_user.user_id;
  list.auth_code = cur_user.auth_code;
  send_request(&list, CMD_LIST);

  p = parse_packet(SFD_CLIENT);
  if (p == NULL)
      goto fail;
  if (p->body_len < 2 * sizeof(int))
      goto fail;
  __memcpy(&res, p->body, 2 * sizeof(int));
  if (res.data_len > 0)
      res.data = p->body + 2 * sizeof(int);
  if (handle_response(p->command, &res) == 0)
  {
    if (p->body_len - 2 * sizeof(int) >= res.data_len &&
        res.data_len >= sizeof(short))
    {
      num_msg = *((short *) res.data);
      if (res.data_len - sizeof(short) < num_msg * (4 * sizeof(short)))
        goto fail;
      int cnt = 0;
      char *dp = res.data + sizeof(short);
      __fdprintf(STDOUT, "\n[[[ MESSAGES ]]]\n");
      __fdprintf(STDOUT, "idx. <from_id, length, type>\n");
      __fdprintf(STDOUT, "----------------------------\n");
      for (i = 0, cnt = 0; cnt < num_msg; cnt++)
      {
        unsigned short type = *((unsigned short *) (dp + i));
        unsigned short from_id = *((unsigned short *) (dp + i + 4));
        unsigned short text_len = *((unsigned short *) (dp + i + 6));
        if (type == TYPE_PROTECTED)
          __fdprintf(STDOUT, "%d. <%d, N/A, Protected>\n", cnt + 1, from_id);
        else
          __fdprintf(STDOUT, "%d. <%d, %d bytes, Normal>\n", cnt + 1, from_id, text_len - 1);
        i += 4 * sizeof(short);
      }
      __fdprintf(STDOUT, "Total %d message(s).\n\n", num_msg);
    }
  }

fail:
  if (p)
    __free(p);

  return num_msg;
}

void display_message(message_t msg)
{
  char *msg_text = NULL;
  char *type_string = msg.type == TYPE_PROTECTED ? "Protected" : "Normal";
  __fdprintf(STDOUT, "From: USER_%d\n", msg.from_id);
  __fdprintf(STDOUT, "Type: %s\n", type_string);
  if (msg.type == TYPE_PROTECTED)
  {
    unsigned int key[4];
    char *unprotected = __malloc(MAX_TEXT_LEN + 1);
    if (unprotected == NULL)
    {
      __fdprintf(STDOUT, "Message: N/A (Error occurred)\n");
      return;
    }
    __memset(unprotected, 0, MAX_TEXT_LEN + 1);
    __strncpy(unprotected, msg.text, MAX_TEXT_LEN);
    key[0] = cur_user.auth_code;
    key[1] = cur_user.auth_code ^ cur_user.user_id;
    key[2] = cur_user.auth_code ^ cur_user.user_id;
    key[3] = cur_user.auth_code;
    unprotect_msg(key, unprotected, MAX_TEXT_LEN);
    msg_text = unprotected;
    __free(unprotected);
  }
  else
  {
    msg_text = msg.text;
  }

  if (msg_text)
  {
    __fdprintf(STDOUT, "Length: %d byte(s)\n", __strlen(msg_text));
    __fdprintf(STDOUT, "Message: %s\n", msg_text);
  }
  else
  {
    __fdprintf(STDOUT, "Length: N/A\n");
    __fdprintf(STDOUT, "Message: N/A (Error occurred)\n");
  }
}

void view_message()
{
  packet_t *p = NULL;
  response_t res;
  char *c, buf[32];
  int msg_id = -1;
  view_req_t view;

  if (cur_user.user_id == 0)
  {
    __fdprintf(STDOUT, "[ERROR] Login first, please.\n");
    return;
  }

  /* Print out messages */
  int num_msg = list_messages();
  if (num_msg == 0)
  {
    __fdprintf(STDOUT, "[INFO] You have no message.\n");
    goto fail;
  }

  __fdprintf(STDOUT, "Select idx > ");
  if (read_until(STDIN, buf, sizeof(buf), '\n') < 0)
    goto fail;
  msg_id = __strtol(buf, &c, 10);
  if (buf == c)
    goto fail;

  if (msg_id < 1 || msg_id > num_msg)
  {
    __fdprintf(STDOUT, "[ERROR] Invalid idx.\n");
    goto fail;
  }

  view.user_id = cur_user.user_id;
  view.auth_code = cur_user.auth_code;
  view.message_id = msg_id;
  send_request(&view, CMD_VIEW);
  __fdprintf(STDOUT, "\n");

  p = parse_packet(SFD_CLIENT);
  if (p == NULL)
      goto fail;
  if (p->body_len < 2 * sizeof(int))
      goto fail;
  __memcpy(&res, p->body, 2 * sizeof(int));
  if (res.data_len > 0)
      res.data = p->body + 2 * sizeof(int);
  if (handle_response(p->command, &res) == 0)
  {
    if (p->body_len - 2 * sizeof(int) >= res.data_len &&
        res.data_len >= sizeof(message_t) - sizeof(char *))
    {
      message_t msg;
      __memcpy(&msg, res.data, sizeof(message_t) - sizeof(char *));
      msg.text = res.data + sizeof(message_t) - sizeof(char *);
      display_message(msg);
    }
  }

fail:
  if (p)
    __free(p);
}

void send_message()
{
  packet_t *p = NULL;
  response_t res;
  int type = 0, msg_len = 0;
  char *c, buf[MAX_TEXT_LEN];
  message_t msg;
  send_req_t send;

  if (cur_user.user_id == 0)
  {
    __fdprintf(STDOUT, "[ERROR] Login first, please.\n");
    return;
  }

  __fdprintf(STDOUT, "\n[[[ SEND ]]]\n");
  __fdprintf(STDOUT, " - To: ");
  if (read_until(STDIN, send.username, MAX_USERNAME_LEN, '\n') < 0)
    goto fail;
  __fdprintf(STDOUT, " - Type (0: normal, 1: protected): ");
  if (read_until(STDIN, buf, sizeof(buf), '\n') < 0)
    goto fail;
  type = __strtol(buf, &c, 10);
  if (buf == c)
    goto fail;
  __memset(buf, '\0', sizeof(buf));
  __fdprintf(STDOUT, " - Message: ");
  if ((msg_len = read_until(STDIN, buf, MAX_TEXT_LEN, '\n')) < 0)
    goto fail;
  __fdprintf(STDOUT, "\n");

  msg.type = type == 1 ? TYPE_PROTECTED : TYPE_NORMAL;
  msg.to_id = -1;
  msg.from_id = cur_user.user_id;
  msg.text_len = msg_len;

  if (msg.type == TYPE_PROTECTED)
  {
    unsigned int key[4];
    key[0] = cur_user.auth_code;
    key[1] = cur_user.auth_code ^ cur_user.user_id;
    key[2] = cur_user.auth_code ^ cur_user.user_id;
    key[3] = cur_user.auth_code;
    protect_msg(key, buf, MAX_TEXT_LEN);
    msg.text_len = MAX_TEXT_LEN;
  }
  msg.text = buf;

  send.user_id = cur_user.user_id;
  send.auth_code = cur_user.auth_code;
  send.msg = &msg;
  send_request(&send, CMD_SEND);

  p = parse_packet(SFD_CLIENT);
  if (p == NULL)
      goto fail;
  if (p->body_len < 2 * sizeof(int))
      goto fail;
  __memcpy(&res, p->body, 2 * sizeof(int));
  if (res.data_len > 0)
      res.data = p->body + 2 * sizeof(int);
  handle_response(p->command, &res);

fail:
  if (p)
    __free(p);
}

void delete_message()
{
  packet_t *p = NULL;
  response_t res;
  char *c, buf[32];
  int msg_id = -1;
  delete_req_t delete;

  if (cur_user.user_id == 0)
  {
    __fdprintf(STDOUT, "[ERROR] Login first, please.\n");
    return;
  }

  /* Print out messages */
  int num_msg = list_messages();
  if (num_msg == 0)
  {
    __fdprintf(STDOUT, "[INFO] You have no message.\n");
    goto fail;
  }

  __fdprintf(STDOUT, "Select idx > ");
  if (read_until(STDIN, buf, sizeof(buf), '\n') < 0)
    goto fail;
  msg_id = __strtol(buf, &c, 10);
  if (buf == c)
    goto fail;

  delete.user_id = cur_user.user_id;
  delete.auth_code = cur_user.auth_code;
  delete.message_id = msg_id;
  send_request(&delete, CMD_DELETE);
  __fdprintf(STDOUT, "\n");

  p = parse_packet(SFD_CLIENT);
  if (p == NULL)
      goto fail;
  if (p->body_len < 2 * sizeof(int))
      goto fail;
  __memcpy(&res, p->body, 2 * sizeof(int));
  if (res.data_len > 0)
      res.data = p->body + 2 * sizeof(int);
  if (handle_response(p->command, &res) == 0)
  {
    if (p->body_len - 2 * sizeof(int) >= res.data_len &&
        res.data_len >= sizeof(int))
    {
      num_msg = *((int *) res.data);
      __fdprintf(STDOUT, "%d message(s) left.\n", num_msg);
    }
  }

fail:
  if (p)
    __free(p);
}

int main()
{
  char buf[4096];
  user_list_t users = NULL;
  msg_queue messages = NULL;
  packet_t *packet = NULL;

  cur_user.user_id = 0;

  __fdprintf(STDOUT, "[INFO] LulzChat Client v0.1\n");
  __fdprintf(STDOUT, "[INFO] Type 'h' for help menu\n");

  while (1)
  {
    /* Read input */
    __fdprintf(STDOUT, "> ");
    if (read_until(STDIN, buf, sizeof(buf),'\n') <= 0)
      quit();

    switch (buf[0])
    {
      case 'H':
      case 'h':
        help();
        break;
      case 'R':
      case 'r':
        register_user();
        break;
      case 'L':
      case 'l':
        login_user();
        break;
      case 'O':
      case 'o':
        logout_user();
        break;
      case 'E':
      case 'e':
        list_messages();
        break;
      case 'V':
      case 'v':
        view_message();
        break;
      case 'S':
      case 's':
        send_message();
        break;
      case 'D':
      case 'd':
        delete_message();
        break;
      case 'Q':
      case 'q':
        quit();
        break;
      default:
        __fdprintf(STDIN, "[ERROR] Unknown command.\n");
        break;
    }

  }

  return 0;
}
