#ifndef READER_H
#define READER_H

#include <stddef.h>

typedef struct {
    size_t index;
    size_t lineno;
    size_t colno;
} Position;

typedef struct {
    const char *buffer;
    const size_t end;

    Position pos;
    Position previous;
} Reader;

int is_eof(const Reader reader);
int is_whitespace(const char c);
char current(const Reader reader);

void undo(Reader *reader);
char consume(Reader *reader);
char *read(Reader *reader, size_t size);
char *read_until(Reader *reader, int (*check)(char));
char *read_while(Reader *reader, int (*check)(char));
char *read_word(Reader *reader);

#endif
