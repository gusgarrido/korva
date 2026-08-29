#ifndef PARSER_H
#define PARSER_H

#include "../token.h"

typedef struct {
    Token **tokens;
    size_t length;
    size_t index;
    size_t previous;
} TokenStream;

int parse(Token **tokens, size_t size);
void rollback_token(TokenStream *stream);
Token *consume_token(TokenStream *stream);


#endif
