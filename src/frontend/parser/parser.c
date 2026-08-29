#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common/utils/reader.h"

#include "parser.h"
#include "expressions.h"

void rollback_token(TokenStream *stream)
{
    stream->index = stream->previous;
}

Token *consume_token(TokenStream *stream)
{
    if (stream->index >= stream->length)
        return NULL;
    
    stream->previous = stream->index;
    Token *token = stream->tokens[stream->index++];
    return token;
}

int parse(Token **tokens, size_t size)
{
    if (!tokens || !tokens[0])
        return 0;
    TokenStream tstream = {tokens, size, 0, 0};
    consume_comparison(&tstream);
    return 0;
}
