#ifndef CTYPE_H_
#define CTYPE_H_

int digittoint(int c);
int isascii(int c);
int __isdigit(int c);
int isprint(int c);
int __isalnum(int c);
int __isalpha(int c);
int __islower(int c);
int __isupper(int c);
int __isspace(int c);
int ispunct(int c);
int iscntrl(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c);

#endif
