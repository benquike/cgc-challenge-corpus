#ifndef CTYPE_H_
#define CTYPE_H_

int __cgc_digittoint(int c);
int __cgc_isascii(int c);
int __isdigit(int c);
int isprint(int c);
int __isalnum(int c);
int __isalpha(int c);
int __islower(int c);
int __isupper(int c);
int __isspace(int c);
int __cgc_ispunct(int c);
int __cgc_iscntrl(int c);
int __cgc_isxdigit(int c);
int tolower(int c);
int toupper(int c);

#endif
