#pragma once

#include <stdio.h>

enum json_operation
{
    JSON_OBJECT_START,
    JSON_OBJECT_END,
    JSON_ARRAY_START,
    JSON_ARRAY_END,
    JSON_STRING,
    JSON_INT,
    JSON_NUMBER
};

typedef struct json_stream json_stream;

json_stream *Create_JSON_Stream(FILE *file);
void Destroy_JSON_Stream(json_stream *stream);

int jfprintf(json_stream *restrict stream, enum json_operation op, ...);
int kv_jfprintf(json_stream *restrict stream, const char *key, enum json_operation op, ...);

#ifdef TEST
void TEST_JSON();
#endif