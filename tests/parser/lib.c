#include "lib.h"
#include "unity.h"

#include "frontend/lexer/tokenization.h"

TokenStream init_tstream(char *content)
{
    Token *tokens = NULL;
    Result res = tokenize(&tokens, content);
    if (res.code != 0)
        return (TokenStream){NULL, -1, 0, 0};
    size_t token_len = (size_t)(uintptr_t)res.value;
    TokenStream tstream = {tokens, token_len, 0, 0};
    return tstream;
}