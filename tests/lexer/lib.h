#include "frontend/lexer/tokenization.h"

void assert_tokens(const char *buffer, const Token expected[], size_t expected_len);
void assert_tokens_err(const char *buffer, size_t expected_len);
void assert_type_tokens(const char *buffer, const int expected[], size_t expected_len);
void assert_simple_tokens(const char *buffer, const char *expected[], size_t expected_len);

#define TEST_ASSERT_TOKENS(input, ...)                                          \
    do                                                                          \
    {                                                                           \
        const Token expected[] = {__VA_ARGS__};                                 \
        assert_tokens(input, expected, sizeof(expected) / sizeof(expected[0])); \
    } while (0)

#define TEST_ASSERT_SIMPLE_TOKENS(input, ...)                                          \
    do                                                                                 \
    {                                                                                  \
        const char *expected[] = {__VA_ARGS__};                                        \
        assert_simple_tokens(input, expected, sizeof(expected) / sizeof(expected[0])); \
    } while (0)

#define TEST_ASSERT_TYPE_TOKENS(input, ...)                                          \
    do                                                                               \
    {                                                                                \
        const int expected[] = {__VA_ARGS__};                                        \
        assert_type_tokens(input, expected, sizeof(expected) / sizeof(expected[0])); \
    } while (0);

#define TEST_ASSERT_EQUAL_TOKEN(expected, actual)                 \
    do                                                            \
    {                                                             \
        TEST_ASSERT_EQUAL_INT(expected.token, actual.token);      \
        TEST_ASSERT_EQUAL_STRING(expected.lexeme, actual.lexeme); \
        TEST_ASSERT_EQUAL_INT(expected.lineno, actual.lineno);    \
        TEST_ASSERT_EQUAL_INT(expected.colno, actual.colno);      \
        TEST_ASSERT_EQUAL_INT(expected.len, actual.len);          \
    } while (0)

#define TEST_ASSERT_EQUAL_TOKEN_MESSAGE(expected, actual, message)                 \
    do                                                                             \
    {                                                                              \
        TEST_ASSERT_EQUAL_INT_MESSAGE(expected.token, actual.token, message);      \
        TEST_ASSERT_EQUAL_STRING_MESSAGE(expected.lexeme, actual.lexeme, message); \
        TEST_ASSERT_EQUAL_INT_MESSAGE(expected.lineno, actual.lineno, message);    \
        TEST_ASSERT_EQUAL_INT_MESSAGE(expected.colno, actual.colno, message);      \
        TEST_ASSERT_EQUAL_INT_MESSAGE(expected.len, actual.len, message);          \
    } while (0)
