#include <stdio.h>
#include <string.h>

#include "expressions.h"
#include "literals.h"

int consume_atom(TokenStream *stream)
{
    Token *token = consume_token(stream);
    if (is_number(token->lexeme) || is_character(token->lexeme) || is_string(token->lexeme))
        return 1;
    rollback_token(stream);
    return 0;
}

int consume_power(TokenStream *stream)
{
    if (consume_atom(stream))
    {
        if (consume_token(stream)->token == DOUBLESTAR)
        {
            int maybe_factor = consume_factor(stream);
            if (maybe_factor == 0)
            {
                printf("[error (invalid syntax)]: Expected token at index %ld, found none.\n\tin %s:%d\n", stream->index, __FILE__, __LINE__);
                stream->index -= 2; // rollback consumed atom and '**'
                return 0;
            }
            return maybe_factor + 2; // add consumed atom and '**'
        }
        rollback_token(stream);
        return 1; // just the consumed atom
    }
    rollback_token(stream);
    return 0;
}

int consume_factor(TokenStream *stream)
{
    Token *maybe_unary = consume_token(stream);
    if (maybe_unary->token == PLUS || maybe_unary->token == MINUS || maybe_unary->token == TILDE)
    {
        int maybe_factor = consume_factor(stream);
        if (maybe_factor == 0)
        {
            printf("[error (invalid syntax)]: Expected token at index %ld, found none.\n\tin %s:%d\n", stream->index, __FILE__, __LINE__);
            stream->index -= 1; // rollback consumed unary
            return 0;
        }
        return maybe_factor + 1; // add consumed unary
    }
    rollback_token(stream);
    return consume_power(stream);
}

static int _is_sep(int token, int tokens[], int sep_len)
{
    for (int i = 0; i < sep_len; i++)
    {
        if (token == tokens[i])
            return token;
    }
    return 0;
}

static int _consume_greedy(
    TokenStream *stream,
    int (*consume_item)(TokenStream *),
    int sep[],
    int sep_len)
{
    int maybe_token = consume_item(stream);
    Token *maybe_operator = consume_token(stream);
    int maybe_sep = _is_sep(maybe_operator->token, sep, sep_len);
    while (maybe_sep)
    {
        int maybe_other = consume_item(stream);
        if (maybe_other == 0)
        {
            printf("[error (invalid syntax)]: Expected token at index %ld, found none.\n\tin %s:%d\n", stream->index, __FILE__, __LINE__);
            stream->index -= maybe_other + 1; // rollback consumed item and operator
            return 0;
        }
        maybe_token += maybe_other + 1; // add consumed item and operator
        maybe_operator = consume_token(stream);
        maybe_sep = _is_sep(maybe_operator->token, sep, sep_len);
    }
    rollback_token(stream);
    return maybe_token;
}

int consume_term(TokenStream *stream)
{
    int seps[] = {STAR, SLASH, PERCENT};
    return _consume_greedy(stream, consume_factor, seps, 3);
}

int consume_sum(TokenStream *stream)
{
    int seps[] = {PLUS, MINUS};
    return _consume_greedy(stream, consume_term, seps, 2);
}

int consume_shift(TokenStream *stream)
{
    int seps[] = {LSHIFT, RSHIFT};
    return _consume_greedy(stream, consume_sum, seps, 2);
}

int consume_and(TokenStream *stream)
{
    int seps[] = {AMPER};
    return _consume_greedy(stream, consume_shift, seps, 1);
}

int consume_xor(TokenStream *stream)
{
    int seps[] = {CIRCUMFLEX};
    return _consume_greedy(stream, consume_and, seps, 1);
}

int consume_or(TokenStream *stream)
{
    int seps[] = {VBAR};
    return _consume_greedy(stream, consume_xor, seps, 1);
}

int consume_comparison(TokenStream *stream)
{
    int seps[] = {LT, GT, DOUBLEEQUAL, GE, LE, NE};
    return _consume_greedy(stream, consume_or, seps, 6);
}
