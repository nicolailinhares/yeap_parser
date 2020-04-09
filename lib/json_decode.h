#ifndef YETAPARSER_JSON_DECODE_H
#define YETAPARSER_JSON_DECODE_H

#include "json_node.h"

typedef int ERR;
ERR validate_json(const char *json_text, const int length);
ERR json_decode(const char *json_text, const int length, json_node *json);

#define ERR_OK 0
#define ERR_UNEXPECTED_EOF -1
#define ERR_EXPECTING_EOB_OR_PAIR -2
#define ERR_UNEXPECTED_ESCAPED_CHAR -3
#define ERR_EXPECTING_VALUE -4
#define ERR_ANCESTOR_NOT_FOUND -5
#define ERR_EXPECTING_ELEMENT -6
#define ERR_EXPECTING_DIGIT_OR_END -7
#define ERR_EXPECTING_PAIR -8
#define ERR_COULD_NOT_PUSH_CONTEXT -9

#endif