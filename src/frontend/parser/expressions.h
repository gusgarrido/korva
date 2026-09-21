#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "common/error/result.h"

#include "parser.h"

Result consume_atom(TokenStream *stream);
Result consume_power(TokenStream *stream);
Result consume_factor(TokenStream *stream);
Result consume_term(TokenStream *stream);
Result consume_sum(TokenStream *stream);

Result consume_shift(TokenStream *stream);
Result consume_and(TokenStream *stream);
Result consume_xor(TokenStream *stream);
Result consume_or(TokenStream *stream);

Result consume_comparison(TokenStream *stream);

#endif
