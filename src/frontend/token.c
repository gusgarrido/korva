#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

char *TOKEN_LEXEMES[] = {
    "OP",
    "IDENTIFIER",
    "NUMBER",
    "STRING",
    "CHARACTER",
    "`",
    "~",
    "!",
    "@",
    "#",
    "$",
    "%",
    "^",
    "&",
    "*",
    "(",
    ")",
    "-",
    "=",
    "+",
    "[",
    "]",
    "{",
    "}",
    "\\",
    "|",
    ";",
    ":",
    ",",
    "<",
    ".",
    ">",
    "/",
    "?",
    "~=",
    "!=",
    "@=",
    "#=",
    "$=",
    "%=",
    "^=",
    "&=",
    "*=",
    "-=",
    "==",
    "+=",
    "|=",
    "\\=",
    ":=",
    "..",
    "<=",
    ">=",
    "/=",
    "?=",
    "&&",
    "**",
    "--",
    "++",
    "||",
    "=>",
    "->",
    "<<",
    ">>",
    "**=",
    "...",
    "<<=",
    ">>=",
    "KEYWORD",
    "SPACE",
    "NEWLINE"};

const size_t TOKEN_LEXEMES_COUNT = sizeof(TOKEN_LEXEMES) / sizeof(TOKEN_LEXEMES[0]);
const Token VOID_TOKEN = {.token = OP, .lexeme = "", .len = 0, .lineno = 0, .colno = 0};

Token create_token(const char *lexeme, int lineno, int colno)
{
    size_t token_len = strlen(lexeme);
    int type = is_operator(lexeme);
    if (type == 0)
    {
        if (token_len == 0)
            return VOID_TOKEN;
        if (lexeme[0] == '"')
            type = STRING;
        else if (lexeme[0] == '\'')
            type = CHARACTER;
        else if (isdigit(lexeme[0]))
            type = NUMBER;
        else if (isalpha(lexeme[0]) || lexeme[0] == '_')
            // TODO: differentiate between IDENTIFIER and KEYWORD
            type = IDENTIFIER;
        else if (lexeme[0] == ' ')
            type = SPACE;
        else if (lexeme[0] == '\n')
            type = NEWLINE;
    }

    if (type == 0)
        return VOID_TOKEN;

    char *owned = malloc(token_len + 1);
    if (owned == NULL)
        return VOID_TOKEN;
    strcpy(owned, lexeme);
    Token token = {.token = type, .lexeme = owned, .len = token_len, .lineno = lineno, .colno = colno};
    return token;
}

int is_operator(const char *s)
{
    switch (strlen(s))
    {
    case 1:
        return is_operator_onechar(s[0]);
    case 2:
        return is_operator_twochar(s[0], s[1]);
    case 3:
        return is_operator_threechar(s[0], s[1], s[2]);
    }
    return OP;
}

int is_identifier(const char c)
{
    return isalnum(c) || c == '_';
}

int is_operator_onechar(const char c)
{
    switch (c)
    {
    case '`':
        return TICK;
    case '~':
        return TILDE;
    case '!':
        return EXCLAMATION;
    case '@':
        return AT;
    case '#':
        return HASH;
    case '$':
        return DOLLAR;
    case '%':
        return PERCENT;
    case '^':
        return CIRCUMFLEX;
    case '&':
        return AMPER;
    case '*':
        return STAR;
    case '(':
        return LPAR;
    case ')':
        return RPAR;
    case '-':
        return MINUS;
    case '=':
        return EQ;
    case '+':
        return PLUS;
    case '[':
        return LBRACE;
    case ']':
        return RBRACE;
    case '{':
        return LSQB;
    case '}':
        return RSQB;
    case '\\':
        return BSLASH;
    case '|':
        return VBAR;
    case ':':
        return COLON;
    case ';':
        return SEMI;
    case ',':
        return COMMA;
    case '<':
        return LT;
    case '.':
        return DOT;
    case '>':
        return GT;
    case '/':
        return SLASH;
    case '?':
        return QUESTION;
    }
    return OP;
}

int is_operator_twochar(const char c1, const char c2)
{
    switch (c1)
    {
    case '~':
        switch (c2)
        {
        case '=':
            return TILDEEQUAL;
        }
        break;
    case '!':
        switch (c2)
        {
        case '=':
            return NE;
        }
        break;
    case '@':
        switch (c2)
        {
        case '=':
            return ATEQUAL;
        }
        break;
    case '#':
        switch (c2)
        {
        case '=':
            return HASHEQUAL;
        }
        break;
    case '$':
        switch (c2)
        {
        case '=':
            return DOLLAREQUAL;
        }
        break;
    case '%':
        switch (c2)
        {
        case '=':
            return PERCENTEQUAL;
        }
        break;
    case '^':
        switch (c2)
        {
        case '=':
            return CIRCUMFLEXEQUAL;
        }
        break;
    case '&':
        switch (c2)
        {
        case '=':
            return AMPEREQUAL;
        case '&':
            return DOUBLEAMPER;
        }
        break;
    case '*':
        switch (c2)
        {
        case '=':
            return STAREQUAL;
        case '*':
            return DOUBLESTAR;
        }
        break;
    case '-':
        switch (c2)
        {
        case '=':
            return MINUSEQUAL;
        case '-':
            return DOUBLEMINUS;
        case '>':
            return RARROW;
        }
        break;
    case '=':
        switch (c2)
        {
        case '=':
            return DOUBLEEQUAL;
        case '>':
            return RETTYPE;
        }
        break;
    case '+':
        switch (c2)
        {
        case '=':
            return PLUSEQUAL;
        case '+':
            return DOUBLEPLUS;
        }
        break;
    case '\\':
        switch (c2)
        {
        case '=':
            return BSLASHEQUAL;
        }
        break;
    case '|':
        switch (c2)
        {
        case '=':
            return VBAREQUAL;
        case '|':
            return DOUBLEVBAR;
        }
        break;
    case ':':
        switch (c2)
        {
        case '=':
            return COLONEQUAL;
        }
        break;
    case '.':
        switch (c2)
        {
        case '.':
            return DOUBLEDOT;
        }
        break;
    case '<':
        switch (c2)
        {
        case '=':
            return LE;
        case '<':
            return LSHIFT;
        }
        break;
    case '>':
        switch (c2)
        {
        case '=':
            return GT;
        case '>':
            return RSHIFT;
        }
        break;
    case '/':
        switch (c2)
        {
        case '=':
            return SLASHEQUAL;
        }
        break;
    case '?':
        switch (c2)
        {
        case '=':
            return QUESTIONEQUAL;
        }
        break;
    }
    return OP;
}

int is_operator_threechar(const char c1, const char c2, const char c3)
{
    switch (c1)
    {
    case '*':
        switch (c2)
        {
        case '*':
            switch (c3)
            {
            case '=':
                return DOUBLESTAREQUAL;
            }
            break;
        }
        break;
    case '.':
        switch (c2)
        {
        case '.':
            switch (c3)
            {
            case '.':
                return ELLIPSIS;
            }
            break;
        }
        break;
    case '<':
        switch (c2)
        {
        case '<':
            switch (c3)
            {
            case '=':
                return LSHIFTEQUAL;
            }
            break;
        }
        break;
    case '>':
        switch (c2)
        {
        case '>':
            switch (c3)
            {
            case '=':
                return RSHIFTEQUAL;
            }
            break;
        }
        break;
    }
    return OP;
}
