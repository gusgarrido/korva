#ifndef CONSUMERS_H
#define CONSUMERS_H

#include "common/error/result.h"
#include "common/utils/reader.h"

Result consume_comment(Reader *reader);
Result consume_operator(Reader *reader);
Result consume_numlit(Reader *reader);
Result consume_strlit(Reader *reader);
Result consume_identifier(Reader *reader);

#endif