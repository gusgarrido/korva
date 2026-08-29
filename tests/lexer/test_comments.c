#include "lib.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void test_inline_comments(void)
{
    {
        const char *cases[] = {
            "token//",
            "token//comment",
            "token //",
            "token // comment",
            "token //comment",
            "token //comment ",
            "token /// extra",
            "token ///extra",
            "token // /* block-like */",
            "token // comment\n",
            "token // comment\n// other comment",
            "token \n// comment",
            "// /* */\ntoken"};
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
            TEST_ASSERT_SIMPLE_TOKENS(cases[i], "token");
    }

    {
        const char *cases[] = {
            "token // comment\nnext",
            "token // comment\nnext//other comment",
            "token //\nnext//comment"};
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
            TEST_ASSERT_SIMPLE_TOKENS(cases[i], "token", "next");
    }

    {
        const char *cases[] = {
            "token / / next\n",
            "token / /\nnext\n",
            "token /\n/ next"};
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
            TEST_ASSERT_SIMPLE_TOKENS(cases[i], "token", "/", "/", "next");
    }
}

void test_eof_inline_comments(void)
{
    const char *empty[] = {
        "//",
        "//\n",
        "// /* */\n",
        "//a\n//b\n//c\n"};

    for (int i = 0; i < sizeof(empty) / sizeof(empty[0]); i++)
        assert_simple_tokens(empty[i], NULL, 0);
}

void test_misc_inline_comments(void)
{
    TEST_ASSERT_SIMPLE_TOKENS("/", "/");
    TEST_ASSERT_SIMPLE_TOKENS("/ /", "/", "/");
    TEST_ASSERT_SIMPLE_TOKENS("token /", "token", "/");
    TEST_ASSERT_SIMPLE_TOKENS("// /*\ntoken */", "token", "*", "/");

    TEST_ASSERT_SIMPLE_TOKENS("\"//\"", "\"//\"");
    TEST_ASSERT_SIMPLE_TOKENS("'//'", "'//'");
    TEST_ASSERT_SIMPLE_TOKENS("\"// not comment\"", "\"// not comment\"");
    TEST_ASSERT_SIMPLE_TOKENS("'// not comment'", "'// not comment'");
}

void test_block_comments(void)
{
    {
        const char *cases[] = {
            "token /* block comment */",
            "token /*block comment*/",
            "token /* block//comment */",
            "token /* block /*comment */",
            "token /* block * / comment */",
            "token /*",
            "token /* unterminated comment",
            "token /*\nblock\ncomment */",
            "token /* //block\ncomment */",
            "token /* unterminated\ncomment",
            "/* // */token",
            "/* outer /* inner */ token",
            "/* outer /** inner */ token"};
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
            TEST_ASSERT_SIMPLE_TOKENS(cases[i], "token");
    }

    {
        const char *cases[] = {
            "token /* block comment */ next",
            "token /**/ next",
            "token /* block\ncomment*/ next",
            "token /*\n*/ next",
            "token/**/next",
            "token/*comment*/next",
            "token/**//**/next",
            "token/**//**///x\n/*y*/next"};
        for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
            TEST_ASSERT_SIMPLE_TOKENS(cases[i], "token", "next");
    }
}

void test_eof_block_comments(void)
{
    const char *empty[] = {
        "/*",
        "/*/",
        "/**/",
        "/*/*/",
        "/*",
        "/**",
        "/* *",
        "/* /* */",
        "/***/",
        "/****/",
        "/* // */"};
    for (int i = 0; i < sizeof(empty) / sizeof(empty[0]); i++)
        assert_simple_tokens(empty[i], NULL, 0);

    TEST_ASSERT_SIMPLE_TOKENS("/* */ */", "*", "/");
    TEST_ASSERT_SIMPLE_TOKENS("/*/**/*/", "*", "/");
    TEST_ASSERT_SIMPLE_TOKENS("/ *", "/", "*");
    TEST_ASSERT_SIMPLE_TOKENS("* /", "*", "/");
    TEST_ASSERT_SIMPLE_TOKENS("*//", "*");
}

void test_misc_block_comments(void)
{
    TEST_ASSERT_SIMPLE_TOKENS("/*/**/*/token", "*", "/", "token");
    TEST_ASSERT_SIMPLE_TOKENS("token /* block comment */*/ next", "token", "*", "/", "next");
    TEST_ASSERT_SIMPLE_TOKENS("token /*\nblock comment\n*/*/ next", "token", "*", "/", "next");
    TEST_ASSERT_SIMPLE_TOKENS("token /* block comment */ next */", "token", "next", "*", "/");
    TEST_ASSERT_SIMPLE_TOKENS("token /* \nblock comment\n */ next */", "token", "next", "*", "/");

    TEST_ASSERT_SIMPLE_TOKENS("\"/* */\"", "\"/* */\"");
    TEST_ASSERT_SIMPLE_TOKENS("'/* */'", "'/* */'");
    TEST_ASSERT_SIMPLE_TOKENS("\"/* not comment */\"", "\"/* not comment */\"");
    TEST_ASSERT_SIMPLE_TOKENS("'/* not comment */'", "'/* not comment */'");
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_inline_comments);
    RUN_TEST(test_eof_inline_comments);
    RUN_TEST(test_misc_inline_comments);

    RUN_TEST(test_block_comments);
    RUN_TEST(test_eof_block_comments);
    RUN_TEST(test_misc_block_comments);
    return UNITY_END();
}
