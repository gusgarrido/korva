#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "parser.h"

int consume_atom(TokenStream *stream);
int consume_power(TokenStream *stream);
int consume_factor(TokenStream *stream);
int consume_term(TokenStream *stream);
int consume_sum(TokenStream *stream);

int consume_shift(TokenStream *stream);
int consume_and(TokenStream *stream);
int consume_xor(TokenStream *stream);
int consume_or(TokenStream *stream);

int consume_comparison(TokenStream *stream);

#endif
