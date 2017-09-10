#ifndef CTYPE_H_
#define CTYPE_H_

int __isdigit(int c);
int __isascii(int c);
int __cgc_isprint(int c);
int __isalnum(int c);
int __isalpha(int c);
int __islower(int c);
int __isupper(int c);
int __isspace(int c);
int __ispunct(int c);
int tolower(int c);
int toupper(int c);

#endif /* !CTYPE_H_ */
