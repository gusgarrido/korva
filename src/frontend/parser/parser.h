#ifndef PARSER_H
#define PARSER_H

#include "common/error/result.h"

#include "../token.h"

typedef struct {
    Token **tokens;
    size_t length;
    size_t index;
    size_t previous;
} TokenStream;

Result parse(Token **tokens, size_t size);
void rollback_token(TokenStream *stream);
Result consume_token(TokenStream *stream);


#endif
