#include "lib.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void test_lineno_basic(void)
{
    TEST_ASSERT_TOKENS("foo", (Token){.token = IDENTIFIER, .lexeme = "foo", .len = 3, .lineno = 1, .colno = 1});
    TEST_ASSERT_TOKENS("    foo", (Token){.token = IDENTIFIER, .lexeme = "foo", .len = 3, .lineno = 1, .colno = 5});
    TEST_ASSERT_TOKENS("\tfoo", (Token){.token = IDENTIFIER, .lexeme = "foo", .len = 3, .lineno = 1, .colno = 5});

    TEST_ASSERT_TOKENS(
        "foo    bar",
        (Token){.token = IDENTIFIER, .lexeme = "foo", .len = 3, .lineno = 1, .colno = 1},
        (Token){.token = IDENTIFIER, .lexeme = "bar", .len = 3, .lineno = 1, .colno = 8});

    TEST_ASSERT_TOKENS(
        "foo bar baz",
        (Token){.token = IDENTIFIER, .lexeme = "foo", .len = 3, .lineno = 1, .colno = 1},
        (Token){.token = IDENTIFIER, .lexeme = "bar", .len = 3, .lineno = 1, .colno = 5},
        (Token){.token = IDENTIFIER, .lexeme = "baz", .len = 3, .lineno = 1, .colno = 9});

    TEST_ASSERT_TOKENS(
        "foo\nbar",
        (Token){.token = IDENTIFIER, .lexeme = "foo", .len = 3, .lineno = 1, .colno = 1},
        (Token){.token = IDENTIFIER, .lexeme = "bar", .len = 3, .lineno = 2, .colno = 1});

    TEST_ASSERT_TOKENS(
        "foo\n    bar",
        (Token){.token = IDENTIFIER, .lexeme = "foo", .len = 3, .lineno = 1, .colno = 1},
        (Token){.token = IDENTIFIER, .lexeme = "bar", .len = 3, .lineno = 2, .colno = 5});

    TEST_ASSERT_TOKENS(
        "foo\n\n\nbar",
        (Token){.token = IDENTIFIER, .lexeme = "foo", .len = 3, .lineno = 1, .colno = 1},
        (Token){.token = IDENTIFIER, .lexeme = "bar", .len = 3, .lineno = 4, .colno = 1});

    TEST_ASSERT_TOKENS(
        "foo\n    \nbar",
        (Token){.token = IDENTIFIER, .lexeme = "foo", .len = 3, .lineno = 1, .colno = 1},
        (Token){.token = IDENTIFIER, .lexeme = "bar", .len = 3, .lineno = 3, .colno = 1});

    TEST_ASSERT_TOKENS(
        "a+b*c==d",
        (Token){.token = IDENTIFIER, .lexeme = "a", .len = 1, .lineno = 1, .colno = 1},
        (Token){.token = PLUS, .lexeme = "+", .len = 1, .lineno = 1, .colno = 2},
        (Token){.token = IDENTIFIER, .lexeme = "b", .len = 1, .lineno = 1, .colno = 3},
        (Token){.token = STAR, .lexeme = "*", .len = 1, .lineno = 1, .colno = 4},
        (Token){.token = IDENTIFIER, .lexeme = "c", .len = 1, .lineno = 1, .colno = 5},
        (Token){.token = DOUBLEEQUAL, .lexeme = "==", .len = 2, .lineno = 1, .colno = 6},
        (Token){.token = IDENTIFIER, .lexeme = "d", .len = 1, .lineno = 1, .colno = 8});

    TEST_ASSERT_TOKENS(
        "<<=\n>>=\n...",
        (Token){.token = LSHIFTEQUAL, .lexeme = "<<=", .len = 3, .lineno = 1, .colno = 1},
        (Token){.token = RSHIFTEQUAL, .lexeme = ">>=", .len = 3, .lineno = 2, .colno = 1},
        (Token){.token = ELLIPSIS, .lexeme = "...", .len = 3, .lineno = 3, .colno = 1});
}

void test_lineno_comments(void)
{
    TEST_ASSERT_TOKENS(
        "foo // comment\nbar",
        (Token){.token = IDENTIFIER, .lexeme = "foo", .len = 3, .lineno = 1, .colno = 1},
        (Token){.token = IDENTIFIER, .lexeme = "bar", .len = 3, .lineno = 2, .colno = 1});

    TEST_ASSERT_TOKENS(
        "foo /* one\ntwo\nthree */ bar",
        (Token){.token = IDENTIFIER, .lexeme = "foo", .len = 3, .lineno = 1, .colno = 1},
        (Token){.token = IDENTIFIER, .lexeme = "bar", .len = 3, .lineno = 3, .colno = 10});
}

void test_lineno_strings(void)
{
    TEST_ASSERT_TOKENS("\"foo\"", (Token){.token = STRING, .lexeme = "\"foo\"", .len = 5, .lineno = 1, .colno = 1});
    TEST_ASSERT_TOKENS("\"foo\nbar\"", (Token){.token = STRING, .lexeme = "\"foo\nbar\"", .len = 9, .lineno = 1, .colno = 1});
    TEST_ASSERT_TOKENS("\"foo\\nbar\"", (Token){.token = STRING, .lexeme = "\"foo\\nbar\"", .len = 10, .lineno = 1, .colno = 1});

    TEST_ASSERT_TOKENS(
        "\"foo\nbar\" baz",
        (Token){.token = STRING, .lexeme = "\"foo\nbar\"", .len = 9, .lineno = 1, .colno = 1},
        (Token){.token = IDENTIFIER, .lexeme = "baz", .len = 3, .lineno = 2, .colno = 6});
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_lineno_basic);
    RUN_TEST(test_lineno_comments);
    RUN_TEST(test_lineno_strings);
    return UNITY_END();
}
