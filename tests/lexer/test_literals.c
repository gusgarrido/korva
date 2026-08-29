#include "frontend/token.h"

#include "lib.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void test_number_literal(void)
{
    TEST_ASSERT_TYPE_TOKENS("1.2.3", NUMBER, DOT, NUMBER);
    TEST_ASSERT_TYPE_TOKENS("..", DOUBLEDOT);
    TEST_ASSERT_TYPE_TOKENS("1..", NUMBER, DOUBLEDOT);
    TEST_ASSERT_TYPE_TOKENS("..1", DOUBLEDOT, NUMBER);

    TEST_ASSERT_TYPE_TOKENS("+1", PLUS, NUMBER);
    TEST_ASSERT_TYPE_TOKENS("-1", MINUS, NUMBER);
    TEST_ASSERT_TYPE_TOKENS("+3.14", PLUS, NUMBER);
    TEST_ASSERT_TYPE_TOKENS("-3.14", MINUS, NUMBER);

    TEST_ASSERT_TYPE_TOKENS("123..456", NUMBER, DOUBLEDOT, NUMBER);
    TEST_ASSERT_TYPE_TOKENS("1._0", NUMBER, DOT, IDENTIFIER);

    {
        const char *cases[] = {
            "0",
            "123456789",
            "00",
            "000123",
            "0.0",
            "1.0",
            "3.14",
            "123.456",
            "999.999"
            // Since we worry about parsing hex/bin/oct numbers later,
            // it's fine that the lexer greedily consumes these and
            // treats them as numbers.
            "123abc",
            "0foo",
            "0x0",
            "0xF"
            "0xFOO",
            "0b0",
            "0b1025s101",
            "0o0",
            "0o81ab29",
            // Same with underscore separators
            "1_000",
            "3.14_159",
            "1_00.5_0",
            "123_",
            "1__2",
            "1_.0",
        };
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
            TEST_ASSERT_TYPE_TOKENS(cases[i], NUMBER);
    }

    {
        const char *cases[] = {".0", ".5", ".123"};
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
            TEST_ASSERT_TYPE_TOKENS(cases[i], DOT, NUMBER);
    }

    {
        const char *cases[] = {"0.", "1.", "999."};
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
            TEST_ASSERT_TYPE_TOKENS(cases[i], NUMBER, DOT);
    }
}

void test_string_literal(void)
{
    assert_tokens_err("\"", 0);
    assert_tokens_err("foo \"", 1);
    assert_tokens_err("foo\"", 1);
    assert_tokens_err("foo \"hello world", 1);
    assert_tokens_err("\"hello world", 0);
    assert_tokens_err("\"\\\"", 0);

    {
        const char *cases[] = {
            "\"\"",
            "\"a\"",
            "\"abc\"",
            "\"hello world\"",
            "\"123\"",
            "\"!@#$%^&*()\"",
            "\" \"",
            "\"   \"",
            "\"\t\"",
            "\"\n\"",
            "\"\r\"",
            "\"\b\"",
            "\"\f\"",
            "\"\'\"",
            "\"\\\"\""};
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
            TEST_ASSERT_TYPE_TOKENS(cases[i], STRING);
    }
}

void test_character_literal(void)
{
    assert_tokens_err("'", 0);
    assert_tokens_err("foo '", 1);
    assert_tokens_err("foo'", 1);
    assert_tokens_err("foo 'hello world", 1);
    assert_tokens_err("'hello world", 0);
    assert_tokens_err("'\\'", 0);

    {
        const char *cases[] = {
            "''",
            "'a'",
            "'abc'",
            "'hello world'",
            "'123'",
            "'!@#$%^&*()'",
            "' '",
            "'   '",
            "'\t'",
            "'\n'",
            "'\r'",
            "'\b'",
            "'\f'",
            "'\"'",
            "'\\''"};
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
            TEST_ASSERT_TYPE_TOKENS(cases[i], CHARACTER);
    }
}

void test_identifiers(void)
{
    TEST_ASSERT_TYPE_TOKENS("1abc", NUMBER);  // invalid number literal, but we don't know that yet. The parser will tell us
    TEST_ASSERT_TYPE_TOKENS("123", NUMBER);
    TEST_ASSERT_TYPE_TOKENS("@name", AT, IDENTIFIER);
    TEST_ASSERT_TYPE_TOKENS("#var", HASH, IDENTIFIER);
    TEST_ASSERT_TYPE_TOKENS("$temp", DOLLAR, IDENTIFIER);
    TEST_ASSERT_TYPE_TOKENS("a-b", IDENTIFIER, MINUS, IDENTIFIER);
    TEST_ASSERT_TYPE_TOKENS("hello!", IDENTIFIER, EXCLAMATION);
    TEST_ASSERT_TYPE_TOKENS("space here", IDENTIFIER, IDENTIFIER);

    {
        const char *cases[] = {
            "x",
            "abc",
            "ABC",
            "_",
            "_temp",
            "var1",
            "_123",
            "A1_B2_C3",
            "foo_"};
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
            TEST_ASSERT_TYPE_TOKENS(cases[i], IDENTIFIER);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_number_literal);
    RUN_TEST(test_string_literal);
    RUN_TEST(test_character_literal);
    RUN_TEST(test_identifiers);
    return UNITY_END();
}
