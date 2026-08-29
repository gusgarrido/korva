#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "frontend/token.h"

#include "lib.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void test_individual_operators(void)
{
    for (size_t i = 0; i < TOKEN_LEXEMES_COUNT; i++)
    {
        char *lexeme = TOKEN_LEXEMES[i];
        if (!is_operator_onechar(lexeme[0]))
            continue;
        TEST_ASSERT_SIMPLE_TOKENS(lexeme, lexeme);
    }
}

static char *_create_operator_buffer(int oplen)
{
    char *buffer = NULL;
    int buffer_len = 0;
    for (size_t i = 0; i < TOKEN_LEXEMES_COUNT; i++)
    {
        char *lexeme = TOKEN_LEXEMES[i];
        if (!is_operator_onechar(lexeme[0]) || strlen(lexeme) != oplen)
            continue;
        char *tmp = realloc(buffer, buffer_len + oplen + 2);
        if (tmp == NULL)
        {
            free(buffer);
            return NULL;
        }
        buffer = tmp;
        for (int j = 0; j < oplen; j++)
        {
            buffer[buffer_len++] = lexeme[j];
        }
        buffer[buffer_len++] = ' ';
        buffer[buffer_len] = '\0';
    }
    return buffer;
}

static size_t _build_expected_ops(const char **expected, int oplen)
{
    size_t n = 0;
    for (size_t i = 0; i < TOKEN_LEXEMES_COUNT; i++)
    {
        const char *lexeme = TOKEN_LEXEMES[i];

        if (is_operator_onechar(lexeme[0]) && strlen(lexeme) == oplen)
            expected[n++] = (char *)lexeme;
    }
    return n;
}

static void _test_operator_by_length(int oplen)
{
    char *buffer = _create_operator_buffer(oplen);
    if (buffer == NULL)
        return TEST_FAIL();
    const char *expected[128];
    size_t expected_len = _build_expected_ops(expected, oplen);
    assert_simple_tokens(buffer, expected, expected_len);
    free(buffer);
}

void test_single_operators()
{
    return _test_operator_by_length(1);
}

void test_double_operators()
{
    return _test_operator_by_length(2);
}

void test_triple_operators()
{
    return _test_operator_by_length(3);
}

static char *_wrap_operator(const char *start, const char *op, const char *end)
{
    size_t len = strlen(start) + strlen(op) + strlen(end) + 1;
    char *s = malloc(len);
    if (s == NULL)
        return NULL;
    snprintf(s, len, "%s%s%s", start, op, end);
    return s;
}

static void _test_wrapped(char *start, char *op, char *end)
{
    char *buffer = _wrap_operator(start, op, end);
    if (buffer == NULL)
        return TEST_FAIL();

    if (isdigit(start[0]) && isdigit(end[0]) && !strcmp(op, "."))
    {
        // If it's of the form 123.456, then the tokenizer is expected
        // to parse it as a single token (floating-point number)
        const char *exp[] = {buffer};
        int size = 1;
        assert_simple_tokens(buffer, exp, size);
    }
    else
    {
        char *exp[3];
        int size = 0;
        if (start[0] != '\0')
            exp[size++] = start;
        exp[size++] = op;
        if (end[0] != '\0')
            exp[size++] = end;
        assert_simple_tokens(buffer, (const char **)exp, size);
    }
    free(buffer);
}

void test_wrapped_operators(void)
{
    for (size_t i = 0; i < TOKEN_LEXEMES_COUNT; i++)
    {
        char *lexeme = TOKEN_LEXEMES[i];
        if (!is_operator_onechar(lexeme[0]))
            continue;

        _test_wrapped("foo", lexeme, "bar");
        _test_wrapped("foo", lexeme, "123");
        _test_wrapped("123", lexeme, "456");
        _test_wrapped("123", lexeme, "foo");
        _test_wrapped("foo", lexeme, "");
        _test_wrapped("", lexeme, "foo");
        _test_wrapped("123", lexeme, "");
        _test_wrapped("", lexeme, "123");
    }
}

void test_operator_stress(void)
{
    TEST_ASSERT_SIMPLE_TOKENS("=", "=");
    TEST_ASSERT_SIMPLE_TOKENS("==", "==");
    TEST_ASSERT_SIMPLE_TOKENS("===", "==", "=");
    TEST_ASSERT_SIMPLE_TOKENS("====", "==", "==");

    TEST_ASSERT_SIMPLE_TOKENS("*=", "*=");
    TEST_ASSERT_SIMPLE_TOKENS("**=", "**=");
    TEST_ASSERT_SIMPLE_TOKENS("***=", "**", "*=");
    TEST_ASSERT_SIMPLE_TOKENS("****=", "**", "**=");

    TEST_ASSERT_SIMPLE_TOKENS("*==", "*=", "=");
    TEST_ASSERT_SIMPLE_TOKENS("**==", "**=", "=");
    TEST_ASSERT_SIMPLE_TOKENS("***==", "**", "*=", "=");
    TEST_ASSERT_SIMPLE_TOKENS("****==", "**", "**=", "=");

    TEST_ASSERT_SIMPLE_TOKENS(">>=", ">>=");
    TEST_ASSERT_SIMPLE_TOKENS(">>>=", ">>", ">=");
    TEST_ASSERT_SIMPLE_TOKENS(">>>>=", ">>", ">>=");

    TEST_ASSERT_SIMPLE_TOKENS("<<=", "<<=");
    TEST_ASSERT_SIMPLE_TOKENS("<<==", "<<=", "=");

    TEST_ASSERT_SIMPLE_TOKENS("->", "->");
    TEST_ASSERT_SIMPLE_TOKENS("-->", "--", ">");

    TEST_ASSERT_SIMPLE_TOKENS("&&", "&&");
    TEST_ASSERT_SIMPLE_TOKENS("&=", "&=");
    TEST_ASSERT_SIMPLE_TOKENS("&&=", "&&", "=");
    TEST_ASSERT_SIMPLE_TOKENS("&&&", "&&", "&");

    TEST_ASSERT_SIMPLE_TOKENS("||=", "||", "=");
    TEST_ASSERT_SIMPLE_TOKENS("|||", "||", "|");

    TEST_ASSERT_SIMPLE_TOKENS("..", "..");
    TEST_ASSERT_SIMPLE_TOKENS("...", "...");
    TEST_ASSERT_SIMPLE_TOKENS("....", "...", ".");
    TEST_ASSERT_SIMPLE_TOKENS(".....", "...", "..");
    TEST_ASSERT_SIMPLE_TOKENS("......", "...", "...");
}

void test_concatenated_operators(void)
{
    TEST_ASSERT_SIMPLE_TOKENS("+=", "+=");
    TEST_ASSERT_SIMPLE_TOKENS("++=", "++", "=");

    TEST_ASSERT_SIMPLE_TOKENS("-=", "-=");
    TEST_ASSERT_SIMPLE_TOKENS("--=", "--", "=");

    TEST_ASSERT_SIMPLE_TOKENS("!foo", "!", "foo");
    TEST_ASSERT_SIMPLE_TOKENS("=foo", "=", "foo");

    TEST_ASSERT_SIMPLE_TOKENS("++foo", "++", "foo");
    TEST_ASSERT_SIMPLE_TOKENS("foo--", "foo", "--");

    TEST_ASSERT_SIMPLE_TOKENS("->foo", "->", "foo");

    TEST_ASSERT_SIMPLE_TOKENS("1+-2", "1", "+", "-", "2");
    TEST_ASSERT_SIMPLE_TOKENS("1--2", "1", "--", "2");
    TEST_ASSERT_SIMPLE_TOKENS("1---2", "1", "--", "-", "2");

    TEST_ASSERT_SIMPLE_TOKENS("a+b", "a", "+", "b");
    TEST_ASSERT_SIMPLE_TOKENS("a+++b", "a", "++", "+", "b");
    TEST_ASSERT_SIMPLE_TOKENS("a++++b", "a", "++", "++", "b");

    TEST_ASSERT_SIMPLE_TOKENS("foo->bar", "foo", "->", "bar");

    TEST_ASSERT_SIMPLE_TOKENS("foo+", "foo", "+");
    TEST_ASSERT_SIMPLE_TOKENS("foo==", "foo", "==");
    TEST_ASSERT_SIMPLE_TOKENS("foo>>=", "foo", ">>=");
}

void test_with_comments(void)
{
    TEST_ASSERT_SIMPLE_TOKENS("a/*x*/+b", "a", "+", "b");
    TEST_ASSERT_SIMPLE_TOKENS("a//x\n+b", "a", "+", "b");

    TEST_ASSERT_SIMPLE_TOKENS("/**/=", "=");
    TEST_ASSERT_SIMPLE_TOKENS("//x\n=", "=");
}

void test_with_strings(void)
{
    TEST_ASSERT_SIMPLE_TOKENS("\"+\"", "\"+\"");
    TEST_ASSERT_SIMPLE_TOKENS("\"==\"", "\"==\"");
    TEST_ASSERT_SIMPLE_TOKENS("\"***\"", "\"***\"");

    TEST_ASSERT_SIMPLE_TOKENS("'+'", "'+'");
    TEST_ASSERT_SIMPLE_TOKENS("'=='", "'=='");
    TEST_ASSERT_SIMPLE_TOKENS("'***'", "'***'");

    TEST_ASSERT_SIMPLE_TOKENS("'++'+a", "'++'", "+", "a");
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_individual_operators);
    RUN_TEST(test_single_operators);
    RUN_TEST(test_double_operators);
    RUN_TEST(test_triple_operators);
    RUN_TEST(test_wrapped_operators);
    RUN_TEST(test_operator_stress);
    RUN_TEST(test_concatenated_operators);
    RUN_TEST(test_with_comments);
    return UNITY_END();
}
