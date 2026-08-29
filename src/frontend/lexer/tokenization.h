#ifndef TOKENIZATION_H
#define TOKENIZATION_H

#include "common/error/result.h"

#include "../token.h"

Result tokenize(Token **tokens, const char *content);
void free_tokens(Token **tokens, size_t size);
void print_token(Token token);
void print_tokens(Token *tokens, size_t size);

#endif
