/*

Author: Debbie Nuttall <debbie@cromulence.com>

Copyright (c) 2015 Cromulence LLC

Permission is hereby granted, __free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#include <libcgc.h>
#include "libc.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "shell.h"

// Runs a shell command
void runshellcommand(char *cmd) {

  if (cmd == NULL) return;
  if (*cmd == '\0') return;
  if (!__strcmp(cmd, "ls")) {
    __printf("one two three\n");
  }
  else if (!__strcmp(cmd, "pwd")) {
    __printf(".\n");
  }
  else if (!strncmp(cmd, "dump ", 5)) {
    int i = __atoi(cmd + 5);
    send_all((char *)i, 4);
  }
  else if (!__strcmp(cmd, "help")) {
    __printf("Here is no help\n");
  }
  else if (!__strcmp(cmd, "info")) {
    __printf("Here is no info\n");
  }
  else if (!__strcmp(cmd, "exec")) {
    // type 1 pov?
  }
}

void shell() {
  // Mimic a shell
  while (1) {
    __printf(">");
  // Read a command
    int bytes_received;
    char input[1024];
    bytes_received = __receive_until(input, sizeof(input), '\n');
  // Execute command
    if (__strcmp(input, "__exit\n") == 0) {
      return;
    }
    runshellcommand(input);
  }
}