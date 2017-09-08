/*

Author: Joe Rogers <joe@cromulence.com>

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
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "shell.h"
#include "fs.h"
#include "screen.h"

// global shell environment
environment ENV;

void PrintPrompt(void) {
	__printf("$s@CB> ", ENV.User);
}

int32_t ParseCli(char *buf, Command *pCmd) {
	char *tok;
	uint8_t FoundQuote = 0;

	if (!buf || !pCmd) {
		return(0);
	}

	pCmd->argc = 0;
	while ((tok = __strtok(buf, " ")) && pCmd->argc <= MAX_ARGS) {
		buf = NULL;
		if (FoundQuote) {
			if (tok[__strlen(tok)-1] == '"') {
				// found the close quote at the end of tok
				// copy this token onto the argv and move on
				tok[__strlen(tok)-1] = '\0';
				if (__strlen(pCmd->argv[pCmd->argc]) + __strlen(tok) + 1 > MAX_ARGLEN) {
					// parse error
					return(1);
				}
				__strcat(pCmd->argv[pCmd->argc], " ");
				__strcat(pCmd->argv[pCmd->argc++], tok);
				FoundQuote = 0;
				continue;
			}

			if (tok[0] == '"' && tok[1] == '\0') {
				// quote is the only thing in tok, so done with this argv
				pCmd->argc++;
				FoundQuote = 0;
				continue;
			}

			if (__strchr(tok, '"') == NULL) {
				// add this tok to the argv
				if (__strlen(pCmd->argv[pCmd->argc]) + __strlen(tok) + 1 > MAX_ARGLEN) {
					// parse error
					return(1);
				}
				__strcat(pCmd->argv[pCmd->argc], " ");
				__strcat(pCmd->argv[pCmd->argc], tok);
				continue;
			} else {
				// found the close quote in the middle of the tok
				// parse error
				return(1);
			}

		}
		if (tok[0] == '\"') {
			// found an open quote
			if (tok[__strlen(tok)-1] == '"') {
				// found the end quote
				tok[__strlen(tok)-1] = '\0';
				// copy everything between the quotes into the argv
				__strncpy(pCmd->argv[pCmd->argc++], tok+1, MAX_ARGLEN);
			} else {
				// this token doesn't end with a quote, so check that there are no mid-token quotes
				if (__strchr(tok+1, '"') != NULL) {
					// parse error
					return(1);
				}

				// make note that we're in a quoted argument
				FoundQuote = 1;

				// but copy what we have into the first part of the argv
				__strncpy(pCmd->argv[pCmd->argc], tok+1, MAX_ARGLEN);
			}
			continue;
		}

		// no quotes, just copy to an argv
		__strncpy(pCmd->argv[pCmd->argc++], tok, MAX_ARGLEN);
	}

	return(2);
}

int32_t HandleListFiles(Command *pCmd) {

	if (!pCmd) {
		return(0);
	}

	if (pCmd->argc != 1) {
		__puts("Invalid arguments");
		return(0);
	}

	ListFiles();
	return(1);
}	

int32_t HandleExit(Command *pCmd) {

	if (!pCmd) {
		return(0);
	}

	if (pCmd->argc != 1) {
		__puts("Invalid arguments");
		return(0);
	}

	__puts("logout");

	_terminate(0);

}

int32_t HandleHelp(Command *pCmd) {

	if (!pCmd) {
		return(0);
	}

	if (pCmd->argc == 1) {
		__puts("Available commands:");
		__puts("chuser");
		__puts("chpw");
		__puts("exit");
		__puts("id");
		__puts("help");
		__puts("?");
		__puts("newuser");
		__puts("deluser");
		__puts("dump");
		__puts("print");
	} else if (pCmd->argc == 2) {
		if (!__strcmp(pCmd->argv[1], "chuser")) {
			__puts("Usage: chuser <username>");
		} else if (!__strcmp(pCmd->argv[1], "passwd")) {
			if (!__strcmp(ENV.User, "root")) {
				__puts("Usage: passwd <username>");
			} else {
				__puts("Usage: passwd");
			}
		} else if (!__strcmp(pCmd->argv[1], "exit")) {
			__puts("Usage: exit");
		} else if (!__strcmp(pCmd->argv[1], "id")) {
			__puts("Usage: id");
		} else if (!__strcmp(pCmd->argv[1], "help")) {
			__puts("Usage: help <command>");
		} else if (!__strcmp(pCmd->argv[1], "?")) {
			__puts("Usage: ? <command>");
		} else if (!__strcmp(pCmd->argv[1], "newuser")) {
			__puts("Usage: newuser <username> <group>");
		} else if (!__strcmp(pCmd->argv[1], "deluser")) {
			__puts("Usage: deluser <username>");
		} else if (!__strcmp(pCmd->argv[1], "cat")) {
			__puts("Usage: dump <filename>");
		} else if (!__strcmp(pCmd->argv[1], "echo")) {
			__puts("Usage: print <text> [> file]");
		} else {
			__printf("Unknown command: $s\n", pCmd->argv[1]);
		}
	}

	return(1);
	
}

uint8_t HandleDump(Command *pCmd) {
	__FILE *stream;

	if (!pCmd) {
		return(0);
	}

	if (pCmd->argc != 2) {
		__puts("Input error");
		__puts("Usage: dump <filename>");
		return(0);
	}

	return(Dump(pCmd->argv[1]));

}	

uint8_t HandlePrint(Command *pCmd) {
	__FILE *stream;
	char *RedirectFile = NULL;
	uint32_t i;

	if (!pCmd) {
		return(0);
	}

	if (pCmd->argc < 2) {
		__puts("Input error");
		__puts("Usage: print \"<text>\" [> file]");
		return(0);
	}

	// run through the args to see if we have a file redirect
	for (i = 2; i < pCmd->argc; i++) {
		if (pCmd->argv[i][0] == '>' && pCmd->argv[i][1] == '\0') {
			// found a file redirect...and there should only be one more arg
			if (i+1 != pCmd->argc-1)  {
				__puts("Input error");
				__puts("Usage: print \"<text>\" [> file]");
				return(0);
			}
			RedirectFile = pCmd->argv[i+1];
			break;
		} 
	}

	if (RedirectFile) {
		if ((stream = __fopen(RedirectFile, "w", 0)) == NULL) {
			__printf("Unable to open file '$s'\n\r", RedirectFile);
			return(0);
		}
		if (__fwrite(pCmd->argv[1], __strlen(pCmd->argv[1]), 1, stream) != __strlen(pCmd->argv[1])) {
			__fclose(stream);
			return(0);
		}
		__fclose(stream);
	} else {
		// output to stdout
		__printf("$s", pCmd->argv[1]);
		for (i = 2; i < pCmd->argc; i++) {
			__printf(" $s", pCmd->argv[i]);
		}
		__puts("");
	}

	return(1);

}

void PrependCommandHistory(char *buf) {
	uint8_t i;

	ENV.NumCommandHistory = 0;
#ifdef PATCHED_2
	for (i = MAX_CMD_HISTORY-1; i > 0; i--) {
#else
	for (i = MAX_CMD_HISTORY; i > 0; i--) {
#endif
		if (ENV.CommandHistory[i-1][0] != '\0') {
			__strcpy(ENV.CommandHistory[i], ENV.CommandHistory[i-1]);
			if (ENV.NumCommandHistory == 0) {
				ENV.NumCommandHistory = i;
			}
		}
	}
	__strcpy(ENV.CommandHistory[0], buf);
	ENV.NumCommandHistory++;
}
