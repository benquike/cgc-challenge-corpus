/*

Author: Dustin Fraze <df@cromulence.co>

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

#include "ltypes.h"
#include "trie.h"
#include "stack.h"

trie *root;

stack *operStack;
stack *funcStack;

void add();
void sub();
void mul();
void div();
void mod();
void xor();
void and();
void not();
void terminate();
void type();
void _int();
void equals();
void len();
void process(char *input);

int main() {
    char line[256];
    char result[256];

    root = initTrie();
    operStack = initStack();
    funcStack = initStack();
    insertInTrie(root, "add", lfunc("add", &add));
    insertInTrie(root, "sub", lfunc("sub", &sub));
    insertInTrie(root, "mul", lfunc("mul", &mul));
    insertInTrie(root, "div", lfunc("div", &div));
    insertInTrie(root, "mod", lfunc("mod", &mod));
    insertInTrie(root, "not", lfunc("not", &not));
    insertInTrie(root, "int", lfunc("int", &_int));
    insertInTrie(root, "terminate", lfunc("terminate", &terminate));
    insertInTrie(root, "equals", lfunc("equals", &equals));
    insertInTrie(root, "type", lfunc("type", &type));
    insertInTrie(root, "len", lfunc("len", &len));
 
    while(1) {
        __memset(line, 0, 256);

        transmit(STDOUT, "> ", 2, NULL);
        if(__receive_until(line, '\n', 255) == 0)
            _terminate(0);
        process(line);
    }

}

void process(char *input) {
    char *tmp;
    int i = 0;
    ltype* foo;
    char *save;
    tmp = __strtok(input, " ");
    while(tmp) {
        if(__isdigit(tmp[0]) || (tmp[0] == '-' && __strlen(tmp) > 1)) {
            //variables can't start with number.  Make sure the rest is a number.
            for(i=1;i<__strlen(tmp);i++)
                if(!__isdigit(tmp[i]))
                    break;
            if(i < __strlen(tmp))
                __puts("NOT A NUMBER");
            else {
                foo = lint("", tmp);
                push(operStack, foo);
            }
        } else if(tmp[0] == '"') {
            char *match;
            tmp++;
            match = __strchr(tmp, '"');
            if(match) {
                *match = 0;
                push(operStack, lstring("", tmp));
            } else {
                tmp--;
                __printf("Unterminated string constant: @s\n", tmp);
            }
        } else {
            //starts with alpha.  Is either a function or a variable.

            if(__strcmp(tmp, "var") == 0)
            {
                //variable declaration.
                //lets assume it's an int and fix it later.
                char *varName = __strtok(NULL, " ");
                char *equals = __strtok(NULL, " ");
#ifdef PATCHED
		if ( equals == NULL ) {
			break;
		}
#endif
                if(equals && *equals == '=') {
                    //declaration and assignment at once.
                    char *tmp;
                    tmp = __strtok(NULL, " ");
                    if(tmp == NULL) {
                        __printf("Malformed variable declaration\n");
                        break;
                    }
                    if(tmp[0] == '"') {
                        char *match;
                        tmp++;
                        match = __strchr(tmp, '"');
                        if(match) {
                            *match = 0;
                            insertInTrie(root, varName, (char *)lstring(varName, tmp));
                        }
                    } else {
                        if(__strcmp(tmp, "False") == 0)
                            insertInTrie(root, varName, (char *)lbool(varName, tmp));
                        else if (__strcmp(tmp, "True") == 0)
                            insertInTrie(root, varName, (char *)lbool(varName, tmp));
                        else
                            insertInTrie(root, varName, (char *)lint(varName, tmp));
                    }
                } else {
                    //just a declaration.  Set it to 0 for now.
                    insertInTrie(root, varName, (char *)lint(varName, "0"));
                }
                tmp = __strtok(NULL, " ");
                continue;
            }
            if(__strcmp(tmp, "True") == 0)
            {
                push(operStack, lbool("", "True"));
                tmp = __strtok(NULL, " ");
                continue;
            }
            if(__strcmp(tmp, "False") == 0)
            {
                push(operStack, lbool("", "False"));
                tmp = __strtok(NULL, " ");
                continue;
            }
            if(__strcmp(tmp, "+") == 0)
            {
                ltype *out;
                out = (ltype *)findInTrie(root, "add")->value;
                #ifndef PATCHED
                push(funcStack, out);
                #else
                if(__strcmp(out->type, "Function") == 0)
                    push(funcStack, out);
                else
                    push(operStack, out);
                #endif
                tmp = __strtok(NULL, " ");
                continue;
            }
            if(__strcmp(tmp, "-") == 0)
            {
                ltype *out;
                out = (ltype *)findInTrie(root, "sub")->value;
                #ifndef PATCHED
                push(funcStack, out);
                #else
                if(__strcmp(out->type, "Function") == 0)
                    push(funcStack, out);
                else
                    push(operStack, out);
                #endif
                tmp = __strtok(NULL, " ");
                continue;
            }
            if(__strcmp(tmp, "*") == 0)
            {
                ltype *out;
                out = (ltype *)findInTrie(root, "mul")->value;
                #ifndef PATCHED
                push(funcStack, out);
                #else
                if(__strcmp(out->type, "Function") == 0)
                    push(funcStack, out);
                else
                    push(operStack, out);
                #endif
                tmp = __strtok(NULL, " ");
                continue;
            }
            if(__strcmp(tmp, "/") == 0)
            {
                ltype *out;
                out = (ltype *)findInTrie(root, "div")->value;
                #ifndef PATCHED
                push(funcStack, out);
                #else
                if(__strcmp(out->type, "Function") == 0)
                    push(funcStack, out);
                else
                    push(operStack, out);
                #endif
                tmp = __strtok(NULL, " ");
                continue;
            }
            if(__strcmp(tmp, "%") == 0)
            {
                ltype *out;
                out = (ltype *)findInTrie(root, "mod")->value;
                #ifndef PATCHED
                push(funcStack, out);
                #else
                if(__strcmp(out->type, "Function") == 0)
                    push(funcStack, out);
                else
                    push(operStack, out);
                #endif
                tmp = __strtok(NULL, " ");
                continue;
            }
            if(__strcmp(tmp, "==") == 0)
            {
                ltype *out;
                out = (ltype *)findInTrie(root, "equals")->value;
                #ifndef PATCHED
                push(funcStack, out);
                #else
                if(__strcmp(out->type, "Function") == 0)
                    push(funcStack, out);
                else
                    push(operStack, out);
                #endif
                tmp = __strtok(NULL, " ");
                continue;
            }

            if(findInTrie(root, tmp)) {
                ltype *out;
                out = (ltype *)findInTrie(root, tmp)->value;
                if(__strcmp(out->type, "Function") == 0) {
                    push(funcStack, out);
                } else {
                    push(operStack, out);
                }
            } else {
                __printf("Undeclared identifier: @s\n", tmp);
            }
        }
        tmp = __strtok(NULL, " ");
    }
    while(!isEmpty(funcStack)) {
        ltype *func = pop(funcStack);
        ((void (*)())func->value)();
    }
    if(!(isEmpty(operStack))) {
        ltype *oper = pop(operStack);
        if(__strcmp(oper->type, "Integer") == 0)
            __printf("@d\n", oper->value);
        else if(__strcmp(oper->type, "String") == 0)
            __printf("@s\n", oper->value);
        else if(__strcmp(oper->type, "Boolean") == 0)
            __printf("@s\n", (int)oper->value == 1 ? "True" : "False");
    }
}

void add() {
    ltype *oper1 = NULL;
    ltype *oper2 = NULL;

    if(!(isEmpty(operStack)))
        oper1 = pop(operStack);
    else {
        __puts("Not enough operands for add");
        return;
    }

    if(!(isEmpty(operStack)))
        oper2 = pop(operStack);
    else {
        __puts("Not enough operands for add");
        return;
    }

    if((__strcmp(oper1->type, "Integer") == 0) && (__strcmp(oper2->type, "Integer") == 0))
    {
        char result[32];
        __bzero(result, 32);
        int_to_str((int)oper1->value + (int)oper2->value, result);
        push(operStack, lint("", result));
    } else if((__strcmp(oper1->type, "String") == 0) && (__strcmp(oper2->type, "String") == 0)) {
        char *tmp;
        tmp = __calloc(oper1->len + oper2->len + 1, 1);
        if(tmp) {
            __memcpy(tmp, oper2->value, oper2->len);
            __memcpy(tmp+(oper2->len), oper1->value, oper1->len);
            push(operStack, lstring("", tmp));
            __free(tmp);
        } else {
            __puts("Critical memory error.  Exiting.");
            _terminate(-1);
        }
    } else if((__strcmp(oper2->type, "String") == 0) && (__strcmp(oper1->type, "Integer") == 0)) {
        char *tmp;
        char result[32];
        __bzero(result, 32);
        int_to_str((int)oper1->value, result);
        tmp = __calloc(oper2->len + oper1->len + 1, 1);
        if(tmp) {
            __memcpy(tmp, oper2->value, oper2->len);
            __memcpy(tmp+oper2->len, result, __strlen(result));
            push(operStack, lstring("", tmp));
            __free(tmp);
        } else {
            __puts("Critical memory error.  Exiting.");
            _terminate(-1);
        }
    } else {
        __printf("Add doesn't make sense on @s and @s\n", oper2->type, oper1->type);
    }

}

void _int() {
    ltype *oper = NULL;
    int i;
    char *tmp;

    if(!(isEmpty(operStack)))
        oper = pop(operStack);

    if(oper) {
        if(__strcmp(oper->type, "Integer") == 0)
            push(operStack, oper);
        else if(__strcmp(oper->type, "String") == 0)
        {
            tmp = oper->value;
            for(i=0;i<__strlen(tmp);i++)
            {
                if(!__isdigit(tmp[i]))
                {
                    __puts("Could not convert argument to int");
                    return;
                }
            }
            push(operStack, lint("", oper->value));
        } else if(__strcmp(oper->type, "Boolean") == 0) {
            if(oper->value)
                push(operStack, lint("", "1"));
            else
                push(operStack, lint("", "0"));
        }
    } else {
        __puts("Not enough operands for int");
    }
}

void mul() {
    ltype *oper1 = NULL;
    ltype *oper2 = NULL;

    if(!(isEmpty(operStack)))
        oper1 = pop(operStack);

    if(!(isEmpty(operStack)))
        oper2 = pop(operStack);

    if(oper1 && oper2) {
        if(__strcmp(oper2->type, "Integer") == 0 && __strcmp(oper1->type, "Integer") == 0) {
            char tmp[32];
            __bzero(tmp, 32);
            int_to_str((int)oper2->value * (int)oper1->value, tmp);
            push(operStack, lint("", tmp));
        } else if(__strcmp(oper2->type, "String") == 0 && __strcmp(oper1->type, "Integer") == 0) {
            char *tmp;
            int size;
            int times;
            char *orig;
            #ifdef PATCHED
            if ((int)oper1->value >= (0x7fffffff / oper2->len)) {
                return;
            }
            if ((int)oper1->value <= 0) {
                return;
            }
            #endif
            times = (int)oper1->value;
            size = (oper2->len * (int)oper1->value) + 1;
            tmp = __calloc(size, 1);
            orig = tmp;
            if(tmp)
            {
                while(times--) {
                    __memcpy(tmp, oper2->value, oper2->len);
                    tmp+=oper2->len;
                }
                push(operStack, lstring("", orig));
            } else {
                __puts("Critical memory error.  Terminating.");
                _terminate(-1);
            }
        } else {
            __printf("Mul does not make sense with @s and @s.\n", oper2->type, oper1->type);
        }
    } else
        __puts("Not enough operands for multiplication");
}

void sub() {
    ltype *oper1 = NULL;
    ltype *oper2 = NULL;

    if(!(isEmpty(operStack)))
        oper1 = pop(operStack);
    else {
        __puts("Not enough operands for sub");
        return;
    }

    if(!(isEmpty(operStack)))
        oper2 = pop(operStack);
    else {
        __puts("Not enough operands for sub");
        return;
    }

    if((__strcmp(oper1->type, "Integer") == 0) && (__strcmp(oper2->type, "Integer") == 0))
    {
        char result[32];
        __bzero(result, 32);
        int_to_str((int)oper2->value - (int)oper1->value, result);
        push(operStack, lint("", result));
    } else {
        __printf("Sub does not make sense on @s and @s\n", oper2->type, oper1->type);
    }
}

void div() {
    ltype *oper1 = NULL;
    ltype *oper2 = NULL;
    
    if(!(isEmpty(operStack)))
        oper1 = pop(operStack);

    if(!(isEmpty(operStack)))
        oper2 = pop(operStack);

    if(oper1 && oper2) {
        if((__strcmp(oper1->type, "Integer") == 0) && (__strcmp(oper2->type, "Integer") == 0)) {
            char result[32];
            __bzero(result, 32);
            if(oper1->value != 0) {
                int_to_str((int)oper2->value / (int)oper1->value, result);
                push(operStack, lint("", result));
            } else
                __puts("Cannot divide by zero.");
        } else
            __printf("Division does not make sense on @s and @s\n", oper2->type, oper1->type);
    } else {
        __puts("Not enough operands for division.");
    }
}

void mod() {
    ltype *oper1 = NULL;
    ltype *oper2 = NULL;
    
    if(!(isEmpty(operStack)))
        oper1 = pop(operStack);

    if(!(isEmpty(operStack)))
        oper2 = pop(operStack);

    if(oper1 && oper2) {
        if((__strcmp(oper1->type, "Integer") == 0) && (__strcmp(oper2->type, "Integer") == 0)) {
            char result[32];
            __bzero(result, 32);
            if(oper1->value != 0) {
                int_to_str((int)oper2->value % (int)oper1->value, result);
                push(operStack, lint("", result));
            } else
                __puts("Cannot mod by zero.");
        } else
            __printf("Mod does not make sense on @s and @s\n", oper2->type, oper1->type);
    } else {
        __puts("Not enough operands for mod.");
    }
}

void not() {
    ltype *oper1 = NULL;

    if(!(isEmpty(operStack)))
        oper1 = pop(operStack);
    else {
        __puts("Not enough operands for not.");
        return;
    }
    //this is mostly a stupid joke...because it's not the string passed in.
    if(__strcmp(oper1->type, "String") == 0) {
        if(__strcmp(oper1->value, "OMG"))
            push(operStack, lstring("", "OMG"));
        else
            push(operStack, lstring("", "BBQ"));
    } else if(__strcmp(oper1->type, "Integer") == 0) {
        char result[32];
        __bzero(result, 32);
        int_to_str(~(int)oper1->value, result);
        push(operStack, lint("", result));
    } else {
        (int)oper1->value == 1 ? push(operStack, lbool("", "False")) : push(operStack, lbool("", "True"));
    }
}

void len() {
    ltype *oper = NULL;

    if(!(isEmpty(operStack)))
        oper = pop(operStack);
    else {
        __puts("Not enough operands for len.");
        return;
    }
    char tmp[32];
    __bzero(tmp, 32);
    int_to_str(oper->len, tmp);
    push(operStack, lint("", tmp));
}

void equals() {
    ltype *oper1 = NULL;
    ltype *oper2 = NULL;

    if(!(isEmpty(operStack)))
        oper1 = pop(operStack);

    if(!(isEmpty(operStack)))
        oper2 = pop(operStack);

    if(oper1 && oper2)
    {
        if((__strcmp(oper1->type, "Integer") == 0 && __strcmp(oper2->type, "Integer") == 0) || (__strcmp(oper1->type, "Boolean") == 0 && __strcmp(oper2->type, "Boolean") == 0)) {
            if(oper1->value == oper2->value)
                push(operStack, lbool("", "True"));
            else
                push(operStack, lbool("", "False"));
        } else if (__strcmp(oper1->type, "String") == 0 && __strcmp(oper2->type, "String") == 0) {
            if(__strcmp(oper1->value, oper2->value) == 0)
                push(operStack, lbool("", "True"));
            else
                push(operStack, lbool("", "False"));
        } else
            push(operStack, lbool("", "False"));
    } else {
        __puts("Not enough operands for equality");
    }
}

void type() {
    ltype *oper1 = NULL;

    if(!(isEmpty(operStack))) {
        oper1 = pop(operStack);
    }

    if(oper1 != NULL)
        push(operStack, lstring("", oper1->type));
}

void terminate() {
    ltype *oper1 = NULL;

    if(!(isEmpty(operStack))) {
        oper1 = pop(operStack);
    }
    //don't worry if the stack is empty.  We're going to terminate anyway.
    if(oper1 && (__strcmp(oper1->type, "Integer") == 0))
        _terminate((int)oper1->value);
    else 
        _terminate(0);
}
