#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "common/utils/reader.h"

#include "parser.h"
#include "expressions.h"

void rollback_token(TokenStream *stream)
{
    stream->index = stream->previous;
}

Result consume_token(TokenStream *stream)
{
    if (stream->index >= stream->length)
        return result_err(1, "Stream index out of bounds");
    
    stream->previous = stream->index;
    Token *token = stream->tokens[stream->index++];
    return result_ok(token);
}

Result parse(Token **tokens, size_t size)
{
    if (!tokens || !tokens[0])
        return result_ok((uintptr_t)0);
    TokenStream tstream = {tokens, size, 0, 0};
    return consume_comparison(&tstream);
}
