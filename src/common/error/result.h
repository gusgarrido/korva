#ifndef RESULT_H
#define RESULT_H

typedef struct
{
    void *value;
    char *message;
    int code;
} Result;

void print_result(Result res);

const Result result_ok(const void *value);
const Result result_err(const int code, const char *message);
const Result result_fromerr(const Result err, const char *message);

void free_result(const Result res);

#endif
