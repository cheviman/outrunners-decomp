/**
 *    Name:        json.h
 *    Description: a small JSON reader for the assets' layouts (assets.c)
 *    Version:     0.1.0
 *    Created:     27-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Reads a whole document into a tree: objects (their keys in order), arrays, strings (\uXXXX below 0x80
 *    only: the layouts are ASCII), numbers, true, false and null.
 */
#ifndef JSON_H
#define JSON_H

#include <stddef.h>

typedef enum
{
	JSON_NULL,
	JSON_FALSE,
	JSON_TRUE,
	JSON_NUMBER,
	JSON_STRING,
	JSON_ARRAY,
	JSON_OBJECT
} json_kind;

typedef struct json_value
{
	json_kind kind;
	double number;
	char *string;                /* a string's text; an object member's key is in keys */
	struct json_value *items;    /* an array's elements, an object's values */
	char **keys;                 /* an object's keys, as many as its values */
	size_t count;
} json_value;

/* The tree of 'text', or NULL with what went wrong in 'error' */
json_value *json_parse(const char *text, char *error, size_t error_size);
void json_free(json_value *value);

/* An object's member, or NULL */
const json_value *json_member(const json_value *object, const char *key);

/* A member's string, or NULL when it has none or another kind */
const char *json_text(const json_value *object, const char *key);

#endif
