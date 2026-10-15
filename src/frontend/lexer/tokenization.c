#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "common/error/result.h"
#include "common/utils/reader.h"

#include "../token.h"

#include "consumers.h"

static Result add_token(Token **tokens, const char *lexeme, size_t *count, size_t lineno, size_t colno)
{
    if (lexeme == NULL)
        return result_err(1, "NULL lexeme provided");

    Token *tmp = realloc(*tokens, (*count + 1) * sizeof(Token));
    if (tmp == NULL)
        return result_err(1, "Out of memory");

    Token token = create_token(lexeme, lineno, colno);
    if (token.token == OP)
        return result_err(1, "Out of memory");

    tmp[*count] = token;
    *tokens = tmp;
    (*count)++;
    return result_ok((uintptr_t)0);
}

void print_token(Token token)
{
    printf("%d('%s')\n", token.token, token.lexeme);
}

void print_tokens(Token *tokens, size_t size)
{
    for (size_t i = 0; i < size; i++)
        printf("%d('%s') ", tokens[i].token, tokens[i].lexeme);
    printf("\n");
}

void free_tokens(Token **tokens, size_t size)
{
    Token *arr = *tokens;
    for (size_t i = 0; i < size; i++)
        free(arr[i].lexeme);
    free(arr);
    *tokens = NULL;
}

static Result handle_result(Result (*token_func)(Reader *), Reader *reader, Token **tokens, size_t *token_count)
{
    size_t lineno = reader->pos.lineno;
    size_t colno = reader->pos.colno;
    Result token_res = token_func(reader);

    if (token_res.code > 0)
        return result_fromerr(token_res, "Failed to consume item");
    if (token_res.code < 0)
        return result_fromerr(token_res, "Token does not match consume criteria");
    char *token = (char *)token_res.value;
    Result res = add_token(tokens, token, token_count, lineno, colno);
    free(token);
    return res;
}

Result tokenize(Token **tokens, const char *content)
{
    size_t token_count = 0;
    Reader reader = {.buffer = content, .end = strlen(content), .pos = {0, 1, 1}, .previous = {0, 1, 1}};
    while (!is_eof(reader))
    {
        Result com = consume_comment(&reader);
        if (com.code == 0)
            continue;
        Result op = handle_result(consume_operator, &reader, tokens, &token_count);
        if (op.code > 0)
            return result_fromerr(op, "Failed to consume operator");
        else if (op.code == 0)
            continue;

        Result numlit = handle_result(consume_numlit, &reader, tokens, &token_count);
        if (numlit.code > 0)
            return result_fromerr(numlit, "Failed to consume number literal");
        else if (numlit.code == 0)
            continue;

        Result strlit = handle_result(consume_strlit, &reader, tokens, &token_count);
        if (strlit.code > 0)
            return result_fromerr(strlit, "Failed to consume string literal");
        else if (strlit.code == 0)
            continue;

        Result identifier = handle_result(consume_identifier, &reader, tokens, &token_count);
        if (identifier.code > 0)
            return result_fromerr(identifier, "Failed to consume identifier");
        else if (identifier.code == 0)
            continue;

        if (!is_whitespace(current(reader)))
            return result_err(1, "Unrecognizable character");
        consume(&reader);
    }
    return result_ok((void *)(uintptr_t)token_count);
}
