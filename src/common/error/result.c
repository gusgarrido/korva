#include <stdio.h>
#include <stdlib.h>

#include "result.h"

void free_result(const Result res) {
    if (res.code != 0 && res.value != NULL)
        free(res.value);
}

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
    if (next->value == NULL)
        printf("[RESULT] End of traceback.\n\n");
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

const Result result_fromerr(const Result err, const char *message)
{
    Result *cpy = malloc(sizeof(Result));
    if (cpy != NULL)
    {
        cpy->code = err.code;
        cpy->message = err.message;
        cpy->value = NULL;
    }
    Result res = {.value = cpy, .code = err.code, .message = message};
    return res;
}
