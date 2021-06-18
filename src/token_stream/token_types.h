#ifndef TOKEN_TYPES_H
#define TOKEN_TYPES_H

#include "keyword_matcher/keyword_tokens.h"
#include "keyword_matcher/opcode_tokens.h"

#define TOKEN_ERROR -2
#define TOKEN_EOF -1
#define TOKEN_NO_MATCH 0
#define TOKEN_IDENTIFIER 1
#define TOKEN_NUMBER 2
#define TOKEN_EXTERNAL_NAME 3
#define TOKEN_DATA 4

#endif
