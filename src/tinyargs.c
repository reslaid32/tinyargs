/**
 * @file tinyargs.c
 * @brief Implementation file for the argument parser library.
 *
 * This file contains the definitions of the functions declared in `tinyargs.h`.
 */

#include "tinyargs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

arg_parser_t* arg_parser_create() {
    arg_parser_t *parser = (arg_parser_t *)malloc(sizeof(arg_parser_t));
    parser->args = NULL;
    parser->count = 0;
    return parser;
}

void arg_parser_add(arg_parser_t *parser, const char *short_name, const char *long_name, arg_type_t type, bool required, const char *description) {
    parser->args = (arg_t *)realloc(parser->args, sizeof(arg_t) * (parser->count + 1));
    parser->args[parser->count].short_name = short_name;
    parser->args[parser->count].long_name = long_name;
    parser->args[parser->count].type = type;
    parser->args[parser->count].required = required;
    parser->args[parser->count].set = false;
    parser->args[parser->count].value = NULL;
    parser->args[parser->count].description = description;
    parser->count++;
}

int arg_parser_parse(arg_parser_t *parser, int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        bool recognized = false;
        for (int j = 0; j < parser->count; j++) {
            arg_t *arg = &parser->args[j];

            if ((arg->short_name && strcmp(argv[i], arg->short_name) == 0) || 
                (arg->long_name && strcmp(argv[i], arg->long_name) == 0)) {
                recognized = true;
                arg->set = true;

                if (arg->type == ARG_TYPE_VALUE) {
                    if (i + 1 < argc) {
                        arg->value = argv[++i];
                    } else if (arg->required) {
                        printf("Error: Missing value for argument %s\n", argv[i]);
                        return 0;
                    }
                }
                break;
            }
        }

        if (!recognized) {
            printf("Error: Unrecognized argument %s\n", argv[i]);
            return 0;
        }
    }

    for (int j = 0; j < parser->count; j++) {
        if (parser->args[j].required && !parser->args[j].set) {
            printf("Error: Missing required argument %s\n", parser->args[j].long_name);
            return 0;
        }
    }

    return 1;
}

const char* arg_parser_get_value(arg_parser_t *parser, const char *name) {
    for (int i = 0; i < parser->count; i++) {
        if ((parser->args[i].short_name && strcmp(parser->args[i].short_name, name) == 0) || 
            (parser->args[i].long_name && strcmp(parser->args[i].long_name, name) == 0)) {
            return parser->args[i].value;
        }
    }
    return NULL;
}

bool arg_parser_is_flag_set(arg_parser_t *parser, const char *name) {
    for (int i = 0; i < parser->count; i++) {
        if ((parser->args[i].short_name && strcmp(parser->args[i].short_name, name) == 0) || 
            (parser->args[i].long_name && strcmp(parser->args[i].long_name, name) == 0)) {
            return parser->args[i].set;
        }
    }
    return false;
}

bool arg_parser_has(arg_parser_t *parser, const char *name) {
    for (int i = 0; i < parser->count; i++) {
        if ((parser->args[i].short_name && strcmp(parser->args[i].short_name, name) == 0) || 
            (parser->args[i].long_name && strcmp(parser->args[i].long_name, name) == 0)) {
            if (parser->args[i].type == ARG_TYPE_FLAG) {
                return parser->args[i].set;
            } else if (parser->args[i].type == ARG_TYPE_VALUE) {
                return parser->args[i].value != NULL;
            }
        }
    }
    return false;
}

void arg_parser_print_help(arg_parser_t *parser) {
    printf("Usage:\n");
    for (int i = 0; i < parser->count; i++) {
        arg_t *arg = &parser->args[i];
        const char *type_str = (arg->type == ARG_TYPE_FLAG) ? "Flag" : "Key=Value";
        if (arg->short_name && arg->long_name) {
            printf("  %s, %s: %s (Type: %s)\n", arg->short_name, arg->long_name, arg->description, type_str);
        } else if (arg->short_name) {
            printf("  %s:     %s (Type: %s)\n", arg->short_name, arg->description, type_str);
        } else if (arg->long_name) {
            printf("  %s:     %s (Type: %s)\n", arg->long_name, arg->description, type_str);
        }
    }
}

void arg_parser_free(arg_parser_t *parser) {
    if (parser) {
        free(parser->args);
        free(parser);
    }
}
