#ifndef RESULT_H
#define RESULT_H

typedef struct {
    const void *value;
    const char *message;
    const int code;
} Result;

const Result result_ok(const void *value);
const Result result_err(const int code, const char *message);
const Result result_fromerr(const Result *err, const char *message);

#endif
