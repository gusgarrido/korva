#include <stdlib.h>
#include <string.h>

#include "reader.h"

static void update_curpos(const char c, Reader *reader)
{
    reader->previous.index = reader->pos.index;
    reader->previous.lineno = reader->pos.lineno;
    reader->previous.colno = reader->pos.colno;

    reader->pos.index++;
    switch (c)
    {
    case '\n':
        reader->pos.lineno++;
        reader->pos.colno = 1;
        break;
    case '\t':
        reader->pos.colno += 4;
        break;
    default:
        reader->pos.colno++;
        break;
    }
}

/**
 * Checks whether the provided Reader has reached end of file (EOF).
 */
int is_eof(const Reader reader)
{
    return reader.pos.index >= reader.end;
}

/**
 * Checks if a given character is a whitespace character (space, newline, tab, or carriage return).
 * Returns 1 if the character is whitespace, otherwise returns 0.
 */
int is_whitespace(const char c)
{
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

/**
 * Returns the current character from the Reader without advancing the index.
 * Returns NULL if EOF is reached.
 */
char current(const Reader reader)
{
    return reader.buffer[reader.pos.index];
}

/**
 * Moves the Reader's index back to the previous position, effectively undoing the last consumption operation.
 */
void undo(Reader *reader)
{
    reader->pos.index = reader->previous.index;
    reader->pos.lineno = reader->previous.lineno;
    reader->pos.colno = reader->previous.colno;
}

/**
 * Consumes a single character from the Reader and advances the index by one.
 * Returns NULL if EOF is reached.
 */
char consume(Reader *reader)
{
    char c = reader->buffer[reader->pos.index];
    update_curpos(c, reader);
    return c;
}

/**
 * Reads a specified number of characters from the Reader, consuming the buffer.
 * Returns a newly allocated string, or NULL.
 */
char *read(Reader *reader, size_t size)
{
    if (reader->pos.index + size > reader->end)
        return NULL;
    char *dest = malloc(size + 1);
    if (dest == NULL)
        return NULL;
    const char *src = reader->buffer + reader->pos.index;
    memcpy(dest, src, size);
    dest[size] = '\0';
    for (size_t i = 0; i < size; i++) {
        update_curpos(dest[i], reader);
    }
    return dest;
}

static char *read_cond(Reader *reader, int (*check)(char), int invert)
{
    size_t i = 0;
    int in_word = 0;
    Position start = reader->pos;
    while (!is_eof(*reader))
    {
        char c = consume(reader);
        int match = check(c);
        if (invert ? match : !match)
        {
            if (!in_word)
                continue;
            break;
        }
        in_word = 1;
        i++;
    }
    reader->pos = start;
    return read(reader, i);
}

/**
 * Reads characters from the Reader until the provided check function returns true, consuming the buffer.
 * The check function takes a character as input and returns 1 if it matches the desired condition, otherwise returns 0.
 * Returns a newly allocated string containing the read characters, or NULL.
 */
char *read_until(Reader *reader, int (*check)(char))
{
    return read_cond(reader, check, 1);
}

/**
 * Reads characters from the Reader while the provided check function returns true, consuming the buffer.
 * The check function takes a character as input and returns 1 if it matches the desired condition, otherwise returns 0.
 * Returns a newly allocated string containing the read characters, or NULL.
 */
char *read_while(Reader *reader, int (*check)(char))
{
    return read_cond(reader, check, 0);
}

/**
 * Reads a word from the Reader, defined as a sequence of non-whitespace characters.
 * Returns the word as a newly allocated string, or NULL.
 */
char *read_word(Reader *reader)
{
    return read_until(reader, is_whitespace);
}
