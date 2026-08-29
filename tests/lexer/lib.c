#include <stdlib.h>

#include "lib.h"
#include "unity.h"

static Token *_init(const char *buffer, size_t expected_len)
{
    Token *actual = NULL;
    Result result = tokenize(&actual, buffer);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, result.code, buffer);
    size_t received = (size_t)(uintptr_t) result.value;
    TEST_ASSERT_EQUAL_size_t_MESSAGE(expected_len, received, buffer);
    return actual;
}

void assert_tokens(const char *buffer, const Token expected[], size_t expected_len)
{
    Token *actual = _init(buffer, expected_len);
    if (expected_len > 0)
    {
        for (size_t i = 0; i < expected_len; i++)
            TEST_ASSERT_EQUAL_TOKEN_MESSAGE(expected[i], actual[i], buffer);
    }
    else
        TEST_ASSERT_NULL_MESSAGE(actual, buffer);
    free_tokens(&actual, expected_len);
}

void assert_tokens_err(const char *buffer, size_t expected_len) {
    Token *actual = NULL;
    Result result = tokenize(&actual, buffer);
    TEST_ASSERT_NOT_EQUAL_INT_MESSAGE(0, result.code, buffer);
    free_tokens(&actual, expected_len);
}

void assert_type_tokens(const char *buffer, const int expected[], size_t expected_len)
{
    Token *actual = _init(buffer, expected_len);
    if (expected_len > 0)
    {
        for (size_t i = 0; i < expected_len; i++)
            TEST_ASSERT_EQUAL_INT_MESSAGE(expected[i], actual[i].token, buffer);
    }
    else
        TEST_ASSERT_NULL_MESSAGE(actual, buffer);
    free_tokens(&actual, expected_len);
}

void assert_simple_tokens(const char *buffer, const char *expected[], size_t expected_len)
{
    Token *actual = _init(buffer, expected_len);
    if (expected_len > 0)
    {
        for (size_t i = 0; i < expected_len; i++)
            TEST_ASSERT_EQUAL_STRING_MESSAGE(expected[i], actual[i].lexeme, buffer);
    }
    else
        TEST_ASSERT_NULL_MESSAGE(actual, buffer);
    free_tokens(&actual, expected_len);
}
