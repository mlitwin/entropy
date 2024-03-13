#include "json.h"

#include <stdlib.h>
#include <stdarg.h>
#include <float.h>

/*
https://www.json.org/json-en.html
*/

enum json_stack_type
{
    JSON_STACK_TYPE_ROOT,
    JSON_STACK_TYPE_OBJECT,
    JSON_STACK_TYPE_ARRAY
};

struct json_stack_node
{
    enum json_stack_type type;
    int depth;
    int count;
    struct json_stack_node *next;
};

static void stack_node_push(struct json_stack_node **node, enum json_stack_type type)
{
    struct json_stack_node *new_node = calloc(sizeof(struct json_stack_node), 1);
    new_node->type = type;
    if (type != JSON_STACK_TYPE_ROOT)
    {
        new_node->depth = (*node) ? (*node)->depth + 1 : 1;
        new_node->next = *node;
    }

    *node = new_node;
}

static void stack_node_pop(struct json_stack_node **node)
{
    struct json_stack_node *next = (*node)->next;

    free(*node);

    *node = next;
}

struct json_stream
{
    FILE *file;
    struct json_stack_node *node;
};

json_stream *Create_JSON_Stream(FILE *file)
{
    json_stream *stream = calloc(sizeof(json_stream), 1);
    stream->file = file;
    stack_node_push(&stream->node, JSON_STACK_TYPE_ROOT);

    return stream;
}

void Destroy_JSON_Stream(json_stream *stream)
{
    while (stream->node)
    {
        stack_node_pop(&stream->node);
    }
    free(stream);
}

static int string_print(json_stream *restrict stream, const char *string)
{
    int ret = 0;
    const char *c = string;

    ret += fputc('"', stream->file);
    while (*c != 0)
    {
        if (*c == '"' || *c == '\\')
        {
            ret += fputc('\\', stream->file);
        }
        ret += fputc(*c, stream->file);
        c++;
    }
    ret += fputc('"', stream->file);

    return ret;
}

static int indent_print(json_stream *restrict stream, int indent)
{
    int ret = 0;

    while (indent > 0)
    {
        ret += fputc(' ', stream->file);
        indent--;
    }

    return ret;
}

static int print_prefix(json_stream *restrict stream, enum json_operation op)
{
    int ret = 0;
    int depth = stream->node->depth;
    if ((op != JSON_OBJECT_END) && (op != JSON_ARRAY_END))
    {
        if (stream->node->count > 0)
            ret += fprintf(stream->file, ",\n");
    }
    else
    {
        depth--;
    }

    ret += indent_print(stream, 2 * depth);

    return ret;
}

static int value_print(json_stream *restrict stream, enum json_operation op, va_list args)
{
    int ret = 0;

    switch (op)
    {
    case JSON_OBJECT_START:
        stack_node_push(&stream->node, JSON_STACK_TYPE_OBJECT);
        ret += fprintf(stream->file, "{\n");
        break;
    case JSON_OBJECT_END:
        ret += fprintf(stream->file, "}\n");
        stack_node_pop(&stream->node);
        stream->node->count++;

        break;
    case JSON_ARRAY_START:
        ret += fprintf(stream->file, "[\n");
        stack_node_push(&stream->node, JSON_STACK_TYPE_ARRAY);
        break;
    case JSON_ARRAY_END:
        ret += fprintf(stream->file, "]\n");
        stack_node_pop(&stream->node);
        stream->node->count++;
        break;
    case JSON_STRING:
    {
        const char *s = va_arg(args, char *);
        ret += string_print(stream, s);
        stream->node->count++;
    }
    break;
    case JSON_INT:
    {
        int v = va_arg(args, int);
        ret += fprintf(stream->file, "%d", v);
        stream->node->count++;
    }
    break;
    case JSON_NUMBER:
    {
        double v = va_arg(args, double);
        ret += fprintf(stream->file, "%.*g", 17, v);
        stream->node->count++;
    }
    break;
    }

    return ret;
}

int jfprintf(json_stream *restrict stream, enum json_operation op, ...)
{
    int ret = 0;
    va_list args;

    va_start(args, op);
    ret += print_prefix(stream, op);
    ret += value_print(stream, op, args);
    va_end(args);

    return ret;
}
int kv_jfprintf(json_stream *restrict stream, const char *key, enum json_operation op, ...)
{
    int ret = 0;
    va_list args;

    ret += print_prefix(stream, op);
    ret += string_print(stream, key);
    ret += fprintf(stream->file, ": ");

    va_start(args, op);
    ret += value_print(stream, op, args);
    va_end(args);

    return ret;
}

#ifdef TEST
#include "test.h"

#define OUTPUT_BUFLEN (1024 * 32)

void TEST_JSON()
{
    char buf[OUTPUT_BUFLEN];

    FILE *mem = fmemopen(buf, OUTPUT_BUFLEN, "w");
    json_stream *stream = Create_JSON_Stream(mem);

    jfprintf(stream, JSON_OBJECT_START);
    kv_jfprintf(stream, "text", JSON_STRING, "te\"st");
    kv_jfprintf(stream, "number", JSON_NUMBER, 1.23456);

    jfprintf(stream, JSON_OBJECT_END);

    Destroy_JSON_Stream(stream);
    fclose(mem);

    printf("%s", buf);
}
#endif