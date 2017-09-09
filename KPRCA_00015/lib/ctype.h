#ifndef CTYPE_H_
#define CTYPE_H_

int __cgc_isascii(int c);
int isprint(int c);
int __isalnum(int c);
int __isalpha(int c);
int __islower(int c);
int __isupper(int c);
int __isspace(int c);
int __cgc_ispunct(int c);
int tolower(int c);
int toupper(int c);

#endif /* !CTYPE_H_ */
