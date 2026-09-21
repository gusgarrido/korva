#include <stdint.h>
#include <string.h>

#include "expressions.h"
#include "literals.h"

Result consume_atom(TokenStream *stream)
{
    Result res = consume_token(stream);
    if (res.code != 0)
        return result_fromerr(&res, "Failed to consume token");
    Token *token = (Token *)res.value;

    if (is_number(token->lexeme) || is_character(token->lexeme) || is_string(token->lexeme))
        return result_ok((void *)(uintptr_t)1);
    rollback_token(stream);
    return result_err(1, "Received token does not fit atom qualifiers");
}

Result consume_power(TokenStream *stream)
{
    Result atom_res = consume_atom(stream);
    if (atom_res.code != 0)
        return result_fromerr(&atom_res, "Failed to retrieve atom");
    // We don't really need the value from consume_atom, since it's always 1 if no failure.

    Result token_res = consume_token(stream);
    if (token_res.code != 0)
        return result_fromerr(&token_res, "Failed to retrieve token");
    Token *obj = (Token *)token_res.value;
    if (obj->token == DOUBLESTAR)
    {
        Result maybe_res_factor = consume_factor(stream);
        if (maybe_res_factor.code != 0)
            return result_fromerr(&maybe_res_factor, "Failed to retrieve factor");
        int maybe_factor = (uintptr_t)maybe_res_factor.value;
        if (maybe_factor == 0)
        {
            // printf("[error (invalid syntax)]: Expected token at index %ld, found none.\n\tin %s:%d\n", stream->index, __FILE__, __LINE__);
            stream->index -= 2; // rollback consumed atom and DOUBLESTAR
            return result_err(1, "Expected token, found none");
        }
        return result_ok((void *)(uintptr_t)(maybe_factor + 2)); // add consumed atom and '**'
    }
    rollback_token(stream);
    return result_ok((void *)(uintptr_t)1); // just the consumed atom
}

Result consume_factor(TokenStream *stream)
{
    Result token_res = consume_token(stream);
    if (token_res.code != 0)
        return result_fromerr(&token_res, "Failed to retrieve token");
    Token *obj = (Token *)token_res.value;

    if (obj->token == PLUS || obj->token == MINUS || obj->token == TILDE)
    {
        Result maybe_res_factor = consume_factor(stream);
        if (maybe_res_factor.code != 0)
            return result_fromerr(&maybe_res_factor, "Failed to retrieve factor");
        int maybe_factor = (uintptr_t)maybe_res_factor.value;
        if (maybe_factor == 0)
        {
            // printf("[error (invalid syntax)]: Expected token at index %ld, found none.\n\tin %s:%d\n", stream->index, __FILE__, __LINE__);
            stream->index -= 1; // rollback consumed unary
            return result_err(1, "Expected token, found none");
        }
        return result_ok((void *)(uintptr_t)(maybe_factor + 1)); // add consumed unary
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

static Result _consume_greedy(
    TokenStream *stream,
    Result (*consume_item)(TokenStream *),
    int sep[],
    int sep_len)
{
    Result maybe_res_token = consume_item(stream);
    if (maybe_res_token.code != 0)
        return result_fromerr(&maybe_res_token, "Failed to retrieve item");
    int maybe_token = (uintptr_t)maybe_res_token.value;

    Result obj = consume_token(stream);
    if (obj.code != 0)
        return result_fromerr(&obj, "Failed to retrieve token");
    Token *maybe_operator = (Token *)obj.value;
    int maybe_sep = _is_sep(maybe_operator->token, sep, sep_len);
    while (maybe_sep)
    {
        Result maybe_res_other = consume_item(stream);
        if (maybe_res_other.code != 0)
            return result_fromerr(&maybe_res_other, "Failed to retieve item");
        int maybe_other = (uintptr_t)maybe_res_other.value;
        if (maybe_other == 0)
        {
            // printf("[error (invalid syntax)]: Expected token at index %ld, found none.\n\tin %s:%d\n", stream->index, __FILE__, __LINE__);
            stream->index -= maybe_other + 1; // rollback consumed item and operator
            return result_err(1, "Expected token, found none");
        }
        maybe_token += maybe_other + 1; // add consumed item and operator
        Result obj = consume_token(stream);
        if (obj.code != 0)
            return result_fromerr(&obj, "Failed to retrieve token");
        maybe_operator = (Token *)obj.value;
        maybe_sep = _is_sep(maybe_operator->token, sep, sep_len);
    }
    rollback_token(stream);
    return result_ok((void *)(uintptr_t)maybe_token);
}

Result consume_term(TokenStream *stream)
{
    int seps[] = {STAR, SLASH, PERCENT};
    return _consume_greedy(stream, consume_factor, seps, 3);
}

Result consume_sum(TokenStream *stream)
{
    int seps[] = {PLUS, MINUS};
    return _consume_greedy(stream, consume_term, seps, 2);
}

Result consume_shift(TokenStream *stream)
{
    int seps[] = {LSHIFT, RSHIFT};
    return _consume_greedy(stream, consume_sum, seps, 2);
}

Result consume_and(TokenStream *stream)
{
    int seps[] = {AMPER};
    return _consume_greedy(stream, consume_shift, seps, 1);
}

Result consume_xor(TokenStream *stream)
{
    int seps[] = {CIRCUMFLEX};
    return _consume_greedy(stream, consume_and, seps, 1);
}

Result consume_or(TokenStream *stream)
{
    int seps[] = {VBAR};
    return _consume_greedy(stream, consume_xor, seps, 1);
}

Result consume_comparison(TokenStream *stream)
{
    int seps[] = {LT, GT, DOUBLEEQUAL, GE, LE, NE};
    return _consume_greedy(stream, consume_or, seps, 6);
}
