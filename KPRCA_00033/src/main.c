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
#include <string.h>
#include <ctype.h>
#include "readuntil.h"

#include "lsimp.h"

#define CURRENT_VER ((char) 103)
#define MAX_BUF_LEN 2048

lsimp_msg_t *head = NULL, *tail = NULL;

enum op_t {
  OP_QUEUE = 0,
  OP_PROCESS = 1,
  OP_QUIT = 2
};

typedef struct data_node {
  lsimp_data_t *data;
  struct data_node *next;
} data_node_t;

data_node_t* sorted(data_node_t *list)
{
  if (list)
  {
    data_node_t *s = NULL, *next = list, *cur = NULL, *c = NULL;;
    if (list->next == NULL)
      return list;

    while (next)
    {
      cur = next;
      next = cur->next;
      if (s == NULL || cur->data->seq < s->data->seq)
      {
        cur->next = s;
        s = cur;
      }
      else
      {
        c = s;
        while (c)
        {
          if (c->next == NULL || cur->data->seq < c->data->seq)
          {
            cur->next = c->next;
            c->next = cur;
            break;
          }
          c = c->next;
        }
      }
    }
    return s;
  }
  return NULL;
}

void queue_msg(lsimp_msg_t *msg)
{
  if (msg == NULL)
    return;

  if (head == NULL)
    head = tail = msg;
  else
  {
    tail->next = msg;
    tail = msg;
  }
  __printf("QUEUED\n");
}

void clear_queue()
{
  if (head)
  {
    lsimp_msg_t *msg = head, *old = msg;
    while (msg)
    {
      if (msg->type == LMT_KEYX)
      {
        if (msg->keyx.key)
          __free(msg->keyx.key);
      }
      else if (msg->type == LMT_DATA)
      {
        if (msg->data.data)
          __free(msg->data.data);
      }
      else if (msg->type == LMT_TEXT)
      {
        if (msg->text.msg)
          __free(msg->text.msg);
      }
      old = msg;
      msg = msg->next;
      __free(old);
    }
  }
  __printf("QUEUE CLEARED\n");
}

void process()
{
  int ttl = 0;
  lsimp_keyx_t *keyx = NULL;
  lsimp_helo_t *helo = NULL;
  data_node_t *dchain = NULL, *dtail = NULL;
  lsimp_msg_t *msg = head;

  if (head)
  {
    while (msg)
    {
      if (helo && ttl > helo->ttl)
        break;
      if (msg->type == LMT_HELO)
      {
        if (helo == NULL)
        {
          if (msg->helo.version == CURRENT_VER)
          {
            helo = &msg->helo;
            __printf("HELO [VERSION %d] [SECURE %s] [TTL %d]\n",
                helo->version, helo->secure_mode ? "on" : "off", helo->ttl);
          }
          else
            __printf("INVALID VERSION\n");
        }
      }
      else if (msg->type == LMT_KEYX)
      {
        if (helo == NULL)
          break;
        if (keyx == NULL)
        {
          if (!helo->secure_mode)
          {
            __printf("KEYX IN NON-SECURE\n");
            break;
          }
          if (msg->keyx.key_len == 0)
          {
            __printf("NO KEY\n");
            break;
          }

          keyx = &msg->keyx;

          __printf("KEYX ");
          __printf("[OPTION ");
          if ((keyx->option & 0x0F) == 0x07)
            __printf("prepend | ");
          else
            __printf("append | ");
          if ((keyx->option & 0xF0) == 0x30)
          {
            __printf("inverted] ");
            int i;
            for (i = 0; i < keyx->key_len; ++i)
              keyx->key[i] = ~keyx->key[i];
          }
          else
            __printf("as-is] ");
          __printf("[LEN %d]\n", keyx->key_len);
        }
      }
      else if (msg->type == LMT_DATA)
      {
        if (helo == NULL)
          break;
        if (!helo->secure_mode)
        {
          __printf("DATA IN NON-SECURE\n");
          break;
        }
        if (keyx == NULL)
        {
          __printf("DATA BEFORE KEYX\n");
          break;
        }
        __printf("DATA [SEQ %d] [LEN %d]\n", msg->data.seq, msg->data.data_len);
        if (dchain == NULL)
        {
          if ((dchain = (data_node_t *)__malloc(sizeof(data_node_t))) != NULL)
          {
            dchain->data = &msg->data;
            dchain->next = NULL;
            dtail = dchain;
          }
        }
        else
        {
          if ((dtail->next = (data_node_t *)__malloc(sizeof(data_node_t))) != NULL)
          {
            dtail = dtail->next;
            dtail->data = &msg->data;
            dtail->next = NULL;
          }
        }
      }
      else if (msg->type == LMT_TEXT)
      {
        if (helo == NULL)
          break;
        if (helo->secure_mode)
        {
          __printf("TEXT IN SECURE\n");
          break;
        }
        if (msg->text.msg_len == 0)
        {
          __printf("NO TEXT MSG\n");
          break;
        }
        __printf("TEXT [LEN %d] [MSG %s]\n", msg->text.msg_len, msg->text.msg);
      }
      msg = msg->next;
      ttl++;
    }
  }

  if (helo == NULL)
    __printf("HELO MISSING\n");

  if (dchain)
  {
    // dtail is wrong after this, but it's okay
    data_node_t *p = sorted(dchain);
    __printf("SECURE MESSAGE:\n");
    int seq = 1, old = 0;
    while (p)
    {
      if (old == p->data->seq)
      {
        __printf("(SEQ #%d DUP)", old);
        p = p->next;
        continue;
      }
      if (seq != p->data->seq)
        __printf("(SEQ #%d MISSING)", seq);
      else
      {
        if (decode_data(keyx, p->data))
          __printf("%s", p->data->data);
        p = p->next;
      }
      old = seq++;
    }
    __printf("\n");
  }

  __printf("PROCESS DONE\n");
  clear_queue();
  head = tail = NULL;
}

void quit()
{
  __printf("QUIT\n");
  __exit(0);
}

int main()
{
  unsigned int len;
  char buf[MAX_BUF_LEN];
  lsimp_msg_t *msg;

  while (1)
  {
    if (read_n(STDIN, (char *)&len, sizeof(len)) <= 0)
      return -1;
    if (len > MAX_BUF_LEN || len < sizeof(int))
      return -1;
    if (read_n(STDIN, (char *)&buf, len) <= 0)
      return -1;

    switch (*(int *)buf)
    {
      case OP_QUEUE:
        msg = parse_msg(buf + sizeof(int), len - sizeof(int));
        if (msg != NULL)
          queue_msg(msg);
        else
          __printf("FAILED TO QUEUE\n");
        break;
      case OP_PROCESS:
        process();
        break;
      case OP_QUIT:
        quit();
        break;
      default:
        return -1;
    }
  }

  return 0;
}
