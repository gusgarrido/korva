#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "result.h"

void print_result(Result res)
{
    printf("[RESULT: %d] ", res.code);
    if (res.code == 0)
    {
        // TODO: Print out res.value
        printf("\n");
        return;
    }
    printf("%s\n", res.message);
    if (res.value == NULL)
        return;

    Result *next = (Result *)res.value;
    printf("[RESULT] The above error was caused by:\n");
    print_result(*next);
}

const Result result_ok(const void *value)
{
    Result res = {.value = value, .code = 0, .message = NULL};
    return res;
}

const Result result_err(const int code, const char *message)
{
    Result res = {.value = NULL, .code = code, .message = message};
    return res;
}

const Result result_fromerr(const Result *err, const char *message)
{
    Result res = {.value = err, .code = err->code, .message = message};
    return res;
}
