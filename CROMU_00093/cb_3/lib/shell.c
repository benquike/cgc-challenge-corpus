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
  if (!__strcmp(cmd, "list")) {
    __printf("one two three key\n");
  }
  else if (!__strcmp(cmd, "pwd")) {
    __printf(".\n");
  }
  else if (!strncmp(cmd, "dump ", 5)) {
    int i = __atoi(cmd + 5);
#ifdef PATCHED_2
    if ((i >= FLAG_PAGE)&&(i <= FLAG_PAGE + PAGE_SIZE)) {
      return;
    }
#endif
    send_all((char *)i, 4);
  }
  else if (!__strcmp(cmd, "help")) {
    __printf("Here is no help\n");
  }
  else if (!__strcmp(cmd, "info")) {
    __printf("Here is no info\n");
  }
  else if (!__strcmp(cmd, "exec")) {
    __printf("Command not supported\n");
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