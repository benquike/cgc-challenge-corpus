/*

Author: Joe Rogers <joe@cromulence.com>

Copyright (c) 2014 Cromulence LLC

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
#include "stdlib.h"
#include "fs.h"
#include "cmd.h"

char *ARGV[MAX_ARGS];
uint32_t ARGC;
extern ShellCmds cmds[];
char CWD[MAX_CMD];

int FreeArgs() {
	int i = 0;

	while (ARGV[i]) {
		deallocate(ARGV[i], __strlen(ARGV[i]));
		ARGV[i] = NULL;
		i++;
	}

	return(0);
}

int ParseArgs(char *cmd) {
	char *tok;
	char *t;
	int new_len;
	int open_quote = 0;

	// start with a clean slate
	__bzero(ARGV, MAX_ARGS*sizeof(char*));
	ARGC = 0;

	// look for spaces
	if ((tok = __strtok(cmd, " ")) == NULL) {
		ARGC = 0;
		return(0);
	}
	ARGV[ARGC++] = __strdup(tok);

	while ((tok = __strtok(NULL, " ")) != NULL && ARGC < MAX_ARGS-1) {
		// handle closing quote made up of multiple " "-separated tokens
		if (open_quote && tok[__strlen(tok)-1] == '"') {
			new_len = __strlen(ARGV[ARGC]) + __strlen(tok) + 1;
			if (allocate(new_len, 0, (void *)&t)) {
				FreeArgs();
				ARGC = 0;
				return(0);
			}
			__strcpy(t, ARGV[ARGC]+1);
			__strcat(t, " ");
			tok[__strlen(tok)-1] = '\0';
			__strcat(t, tok);
			deallocate(ARGV[ARGC], __strlen(ARGV[ARGC]));
			ARGV[ARGC++] = t;
			open_quote = 0;

		// handle middle tokens for quoted string
		} else if (open_quote) {
			new_len = __strlen(ARGV[ARGC]) + __strlen(tok) + 2;
			if (allocate(new_len, 0, (void *)&t)) {
				FreeArgs();
				ARGC = 0;
				return(0);
			}
			__strcpy(t, ARGV[ARGC]);
			__strcat(t, " ");
			__strcat(t, tok);
			deallocate(ARGV[ARGC], __strlen(ARGV[ARGC]));
			ARGV[ARGC] = t;

		// handle token delimited by quotes
		} else if (tok[0] == '"' & tok[__strlen(tok)-1] == '"') {
			tok[__strlen(tok)-1] = '\0';
			ARGV[ARGC++] = __strdup(tok+1);

		// handle starting quote 
		} else if (tok[0] == '"') {
			open_quote = 1;
			ARGV[ARGC] = __strdup(tok);

		// not a quoted token
		} else {
			ARGV[ARGC++] = __strdup(tok);
		}
	}

	// found a starting, but no ending quote, abort processing
	if (open_quote) {
		__puts("missing quote");
		FreeArgs();
		ARGC = 0;
		return(0);
	}

	// too many args, abort processing
	if (ARGC == MAX_ARGS-1) {
		__puts("too many args");
		FreeArgs();
		ARGC = 0;
		return(0);
	}

	return(ARGC);

}

int main(void) {
	char cmd[MAX_CMD];
	ShellCmds *c;

	__bzero(CWD, MAX_CMD);
	__strcpy(CWD, "/");

	if (InitFS(512*1024)) {
		__puts("Failed to initialize the RAM file system\n");
		return(-1);
	}

	__puts("Welcome to the cgcfs shell!");
	__puts("Type help for a list of available commands.");
	__printf("% ");
	__bzero(cmd, MAX_CMD);
	while (readUntil(cmd, MAX_CMD-1, '\n') != -1) {
		ParseArgs(cmd);

		// parse failure
		if (ARGC == 0) {
			FreeArgs();
			__bzero(cmd, MAX_CMD);
			__printf("% ");
			continue;
		}

		// find the command being requested
		if (ARGV[0] != NULL) {
			c = cmds;
			while (c->command != NULL) {
				if (!__strcmp(c->command, ARGV[0])) {
					// run the command
					c->handler();

					break;
				}
				c++;
			}
		}
		if (c == NULL) {
			__puts("Invalid command");
		}
		if (c->command == NULL) {
			__puts("Invalid command");
		}

		FreeArgs();
		__bzero(cmd, MAX_CMD);
		__printf("% ");
	}

	if (DestroyFS()) {
		__puts("Failed to destroy the RAM file system\n");
		return(-1); 
	}

	return(0);

}
