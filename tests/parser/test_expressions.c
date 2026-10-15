#include <stdlib.h>

#include "lib.h"
#include "unity.h"

#include "frontend/lexer/tokenization.h"
#include "frontend/parser/expressions.h"

void setUp(void) {}

void tearDown(void) {}

void test_atom(void)
{
    {
        TokenStream tstream = init_tstream("");
        if (tstream.length < 0)
            return TEST_FAIL_MESSAGE("Failed to tokenize input");
        Result res = consume_atom(&tstream);
        TEST_ASSERT_EQUAL_INT_MESSAGE(1, res.code, "No tokens provided, yet one was successfully consumed");
    }

    {
        TokenStream tstream = init_tstream("1");
        if (tstream.tokens == NULL)
            return TEST_FAIL_MESSAGE("Failed to tokenize input");
        Result res = consume_atom(&tstream);
        free_tokens(&tstream.tokens, tstream.length);
        TEST_ASSERT_EQUAL_INT_MESSAGE(0, res.code, res.message);
    }

    {
        TokenStream tstream = init_tstream("\"foo\"");
        if (tstream.tokens == NULL)
            return TEST_FAIL_MESSAGE("Failed to tokenize input");
        Result res = consume_atom(&tstream);
        free_tokens(&tstream.tokens, tstream.length);
        TEST_ASSERT_EQUAL_INT_MESSAGE(0, res.code, res.message);
    }

    {
        TokenStream tstream = init_tstream("'foo'");
        if (tstream.tokens == NULL)
            return TEST_FAIL_MESSAGE("Failed to tokenize input");
        Result res = consume_atom(&tstream);
        free_tokens(&tstream.tokens, tstream.length);
        TEST_ASSERT_EQUAL_INT_MESSAGE(0, res.code, res.message);
    }

    for (int i = 0; i < TOKEN_LEXEMES_COUNT; i++) {
        char *lexeme = TOKEN_LEXEMES[i];
        if (!is_operator_onechar(lexeme[0]))
            continue;
        TokenStream tstream = init_tstream(lexeme);
        if (tstream.tokens == NULL)
            return TEST_FAIL_MESSAGE("Failed to tokenize input");
        Result res = consume_atom(&tstream);
        free_tokens(&tstream.tokens, tstream.length);
        TEST_ASSERT_EQUAL_INT_MESSAGE(1, res.code, "Operator parsed as atom");
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_atom);
    return UNITY_END();
}