#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../token.h"

#include "consumers.h"

Result consume_comment(Reader *reader)
{
    char c = current(*reader);
    if (c != '/')
        return result_err(-1, "Not a comment");
    consume(reader);

    char next = current(*reader);
    if (next == '/')
    {
        consume(reader);
        while (!is_eof(*reader) && current(*reader) != '\n')
            consume(reader);
        return result_ok(NULL);
    }
    else if (next == '*')
    {
        consume(reader);
        while (!is_eof(*reader))
        {
            char cur = consume(reader);
            if (cur == '*' && current(*reader) == '/')
            {
                consume(reader);
                break;
            }
        }
        return result_ok(NULL);
    }
    undo(reader);
    return result_err(-1, "Not a comment");
}

Result consume_operator(Reader *reader)
{
    char c = current(*reader);
    if (!is_operator_onechar(c))
        return result_err(-1, "Not an operator");
    consume(reader);

    size_t len = 1;
    char *operator = malloc(2);
    if (operator == NULL)
        return result_err(1, "Out of memory");
    operator[0] = c;
    operator[1] = '\0';

    while (!is_eof(*reader) && is_operator(operator))
    {
        char next = current(*reader);
        if (!is_operator_onechar(next))
            break;
        consume(reader);
        char *tmp = realloc(operator, len + 2);
        if (tmp == NULL)
        {
            free(operator);
            return result_err(1, "Out of memory");
        }
        operator = tmp;
        operator[len++] = next;
        operator[len] = '\0';
    }
    if (!is_operator(operator))
    {
        operator[--len] = '\0';
        undo(reader);
    }
    return result_ok(operator);
}

Result consume_numlit(Reader *reader)
{
    char c = current(*reader);
    if (!isdigit(c))
        return result_err(-1, "Not a number literal");

    Position start = reader->pos;
    char *bit = read_while(reader, is_identifier);
    // Check if we can keep parsing as a floating-point number
    // Otherwise, add what we have and continue
    if (current(*reader) == '.')
    {
        consume(reader);
        if (isdigit(current(*reader)))
        {
            char *next = read_while(reader, is_identifier);
            size_t token_len = strlen(bit) + strlen(next) + 1;
            free(bit);
            free(next);
            reader->pos = start;
            char *token = read(reader, token_len);
            return result_ok(token);
        }
        else
            undo(reader);
    }
    return result_ok(bit);
}

Result consume_strlit(Reader *reader) {
    char c = current(*reader);
    if (c != '"' && c != '\'')
        return result_err(-1, "Not a string literal");
    Position start = reader->pos; // start from the quotation mark
    consume(reader);
    int escaped = 0;
    int terminated = 0;
    while (!is_eof(*reader) && !terminated)
    {
        char cur = consume(reader);
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

    size_t token_len = reader->pos.index - start.index;
    reader->pos = start;
    return result_ok(read(reader, token_len));
}

Result consume_identifier(Reader *reader) {
    char c = current(*reader);
    if (!isalpha(c) && c != '_')
        return result_err(-1, "Not an identifier");

    return result_ok(read_while(reader, is_identifier));
}
