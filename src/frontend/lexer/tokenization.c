#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "common/error/result.h"
#include "common/utils/reader.h"

#include "../token.h"

static Result add_token(Token **tokens, const char *lexeme, size_t *count, size_t lineno, size_t colno)
{
    if (!lexeme)
    {
        return result_err(1, "NULL lexeme provided.");
    }

    Token *tmp = realloc(*tokens, (*count + 1) * sizeof(Token));
    if (tmp == NULL)
    {
        return result_err(1, "Failed to allocate memory.");
    }

    Token token = create_token(lexeme, lineno, colno);
    if (token.token == OP)
    {
        return result_err(1, "Failed to allocate memory.");
    }

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

Result tokenize(Token **tokens, const char *content)
{
    size_t token_count = 0;
    size_t lineno = 1;
    size_t colno = 1;
    size_t len = strlen(content);
    Reader reader = {.buffer = content, .end = len, .index = 0, .previous = 0};
    while (!is_eof(reader))
    {
        char c = consume(&reader);

        // COMMENTS
        if (c == '/')
        {
            char next = current(reader);
            if (next == '/')
            {
                consume(&reader);
                while (!is_eof(reader) && current(reader) != '\n')
                    consume(&reader);
                // We are going to handle the new line on the next iteration
                continue;
            }
            else if (next == '*')
            {
                consume(&reader);
                colno += 2;
                while (!is_eof(reader))
                {
                    char cur = consume(&reader);
                    colno += cur == '\t' ? 4 : 1;
                    if (cur == '\n')
                    {
                        colno = 1;
                        lineno++;
                    }
                    if (cur == '*' && current(reader) == '/')
                    {
                        colno++;
                        consume(&reader);
                        break;
                    }
                }
                continue;
            }
        }
        // OPERATORS
        if (is_operator_onechar(c))
        {
            char operator[8] = {c, '\0'};
            while (!is_eof(reader) && is_operator(operator))
            {
                char next = current(reader);
                if (!is_operator_onechar(next))
                    break;
                consume(&reader);
                size_t len = strlen(operator);
                operator[len] = next;
                operator[len + 1] = '\0';
            }
            if (!is_operator(operator))
            {
                operator[strlen(operator) - 1] = '\0';
                undo(&reader);
            }
            Result res = add_token(tokens, operator, &token_count, lineno, colno);
            colno += strlen(operator);
            if (res.code != 0)
                return result_fromerr(&res, "Failed to create token.");
            continue;
        }
        // NUMBER LITERALS
        if (isdigit(c))
        {
            undo(&reader);
            size_t start = reader.index;
            char *bit = read_while(&reader, is_identifier);
            // Check if we can keep parsing as a floating-point number
            // Otherwise, add what we have and continue
            if (current(reader) == '.')
            {
                consume(&reader);
                if (isdigit(current(reader)))
                {
                    char *next = read_while(&reader, is_identifier);
                    size_t token_len = strlen(bit) + strlen(next) + 1;
                    free(bit);
                    free(next);
                    reader.index = start;
                    char *token = read(&reader, token_len);
                    Result res = add_token(tokens, token, &token_count, lineno, colno);
                    colno += token_len;
                    if (res.code != 0)
                    {
                        free(token);
                        return result_fromerr(&res, "Failed to create token.");
                    }
                    free(token);
                    continue;
                }
                else
                    undo(&reader);
            }
            Result res = add_token(tokens, bit, &token_count, lineno, colno);
            colno += strlen(bit);
            if (res.code != 0)
            {
                free(bit);
                return result_fromerr(&res, "Failed to create token.");
            }
            free(bit);
            continue;
        }
        // STRING LITERALS
        if (c == '"' || c == '\'')
        {
            size_t start = reader.index - 1; // start from the quotation mark
            int escaped = 0;
            int terminated = 0;
            size_t new_lineno = lineno;
            size_t new_colno = colno + 1;
            while (!is_eof(reader) && !terminated)
            {
                char cur = consume(&reader);
                new_colno += cur == '\t' ? 4 : 1;
                if (cur == '\n')
                {
                    new_lineno++;
                    new_colno = 1;
                }
                if (escaped)
                {
                    escaped = 0;
                    continue;
                }

                if (cur == '\\')
                    escaped = 1;
                else if (cur == c && !escaped)
                    terminated = 1;
            }
            if (escaped || !terminated)
                return result_err(1, "Unterminated string literal.");

            size_t token_len = reader.index - start;
            reader.index = start;
            char *token = read(&reader, token_len);
            Result res = add_token(tokens, token, &token_count, lineno, colno);
            lineno = new_lineno;
            colno = new_colno;
            if (res.code != 0)
            {
                free(token);
                return result_fromerr(&res, "Failed to create token.");
            }
            free(token);
            continue;
        }
        // IDENTIFIERS
        if (isalpha(c) || c == '_')
        {
            undo(&reader);
            char *token = read_while(&reader, is_identifier);
            Result res = add_token(tokens, token, &token_count, lineno, colno);
            colno += strlen(token);
            if (res.code != 0)
            {
                free(token);
                return result_fromerr(&res, "Failed to create token.");
            }
            free(token);
            continue;
        }
        // WHITESPACES
        if (c == ' ')
            colno++;
        else if (c == '\t')
            colno += 4;
        else if (c == '\n')
        {
            colno = 1;
            lineno++;
        }
        else
            return result_err(1, "Unrecognized character.");
    }
    return result_ok((void *)(uintptr_t)token_count);
}
