#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "result.h"

const Result return_ok(const void *value)
{
    Result res = {.value = value, .code = 0, .message = NULL};
    return res;
}

const Result return_err(const int code, const char *message)
{
    Result res = {.value = NULL, .code = code, .message = message};
    return res;
}

const Result return_fromerr(const Result *err, const char *message)
{
    Result res = {.value = err, .code = err->code, .message = message};
    return res;
}
