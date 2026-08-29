#ifndef TOKEN_H
#define TOKEN_H

#define OP 0
#define IDENTIFIER 1
#define NUMBER 2
#define STRING 3
#define CHARACTER 4

#define TICK 5
#define TILDE 6
#define EXCLAMATION 7
#define AT 8
#define HASH 9
#define DOLLAR 10
#define PERCENT 11
#define CIRCUMFLEX 12
#define AMPER 13
#define STAR 14
#define LPAR 15
#define RPAR 16
#define MINUS 17
#define EQ 18
#define PLUS 19
#define LBRACE 20
#define RBRACE 21
#define LSQB 22
#define RSQB 23
#define BSLASH 24
#define VBAR 25
#define SEMI 26
#define COLON 27
#define COMMA 28
#define LT 29
#define DOT 30
#define GT 31
#define SLASH 32
#define QUESTION 33

#define TILDEEQUAL 34
#define NE 35
#define ATEQUAL 36
#define HASHEQUAL 37
#define DOLLAREQUAL 38
#define PERCENTEQUAL 39
#define CIRCUMFLEXEQUAL 40
#define AMPEREQUAL 41
#define STAREQUAL 42
#define MINUSEQUAL 43
#define DOUBLEEQUAL 44
#define PLUSEQUAL 45
#define VBAREQUAL 46
#define BSLASHEQUAL 47
#define COLONEQUAL 48
#define DOUBLEDOT 49
#define LE 50
#define GE 51
#define SLASHEQUAL 52
#define QUESTIONEQUAL 53
#define DOUBLEAMPER 54
#define DOUBLESTAR 55
#define DOUBLEMINUS 56
#define DOUBLEPLUS 57
#define DOUBLEVBAR 58
#define RARROW 59
#define RETTYPE 60
#define LSHIFT 61
#define RSHIFT 62

#define DOUBLESTAREQUAL 63
#define ELLIPSIS 64
#define LSHIFTEQUAL 65
#define RSHIFTEQUAL 66

#define KEYWORD 67
#define SPACE 68
#define NEWLINE 69

#include <stddef.h>

extern char *TOKEN_LEXEMES[];
extern const size_t TOKEN_LEXEMES_COUNT;

typedef struct
{
    int token;
    char *lexeme;
    int lineno;
    int colno;
    int len;
} Token;

Token create_token(const char *lexeme, int lineno, int colno);

int is_operator(const char *s);
int is_identifier(const char c);

int is_operator_onechar(const char c);
int is_operator_twochar(const char c1, const char c2);
int is_operator_threechar(const char c1, const char c2, const char c3);

#endif
