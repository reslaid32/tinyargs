/**
 * @file tinyargs.h
 * @brief Header file for the argument parser library.
 *
 * This file contains the declarations for the argument parser functions and data structures.
 */

#ifndef TINYARGS_H
#define TINYARGS_H

#include <stdbool.h>

/**
 * @enum arg_type_t
 * @brief Enumeration for argument types.
 *
 * This enumeration defines the types of arguments that can be used:
 * - ARG_TYPE_FLAG: A boolean flag (e.g., `-h` or `--help`).
 * - ARG_TYPE_VALUE: A key-value pair (e.g., `-n name` or `--name=value`).
 */
typedef enum {
    ARG_TYPE_FLAG,    /**< A boolean flag */
    ARG_TYPE_VALUE    /**< A key-value pair */
} arg_type_t;

/**
 * @struct arg_t
 * @brief Structure representing an argument.
 *
 * This structure defines an argument including its names, type, and description.
 */
typedef struct {
    const char *short_name;     /**< Short name of the argument (e.g., `-h`) */
    const char *long_name;      /**< Long name of the argument (e.g., `--help`) */
    arg_type_t type;            /**< Type of the argument (flag or value) */
    bool required;              /**< Whether the argument is required */
    bool set;                   /**< Whether the argument has been set */
    const char *value;          /**< Value associated with the argument (for key-value pairs) */
    const char *description;    /**< Description of the argument */
} arg_t;

/**
 * @struct arg_parser_t
 * @brief Structure for the argument parser.
 *
 * This structure holds the arguments and the count of arguments.
 */
typedef struct {
    arg_t *args; /**< Array of arguments */
    int count;  /**< Number of arguments */
} arg_parser_t;

/**
 * @brief Create a new argument parser.
 *
 * @return A pointer to the created argument parser.
 */
arg_parser_t* arg_parser_create();

/**
 * @brief Add an argument to the parser.
 *
 * @param parser Pointer to the argument parser.
 * @param short_name Short name of the argument (optional).
 * @param long_name Long name of the argument (optional).
 * @param type Type of the argument (flag or value).
 * @param required Whether the argument is required.
 * @param description Description of the argument.
 */
void arg_parser_add(arg_parser_t *parser, const char *short_name, const char *long_name, arg_type_t type, bool required, const char *description);

/**
 * @brief Parse command-line arguments.
 *
 * @param parser Pointer to the argument parser.
 * @param argc Argument count.
 * @param argv Array of argument values.
 * @return 1 if parsing was successful, 0 otherwise.
 */
int arg_parser_parse(arg_parser_t *parser, int argc, char *argv[]);

/**
 * @brief Get the value of an argument.
 *
 * @param parser Pointer to the argument parser.
 * @param name Name of the argument (short or long).
 * @return Value of the argument if present, NULL otherwise.
 */
const char* arg_parser_get_value(arg_parser_t *parser, const char *name);

/**
 * @brief Check if a flag is set.
 *
 * @param parser Pointer to the argument parser.
 * @param name Name of the flag (short or long).
 * @return true if the flag is set, false otherwise.
 */
bool arg_parser_is_flag_set(arg_parser_t *parser, const char *name);

/**
 * @brief Check if an argument is present.
 *
 * @param parser Pointer to the argument parser.
 * @param name Name of the argument (short or long).
 * @return true if the argument is present, false otherwise.
 */
bool TINYARGS_Has(arg_parser_t *parser, const char *name);

/**
 * @brief Print help information for the arguments.
 *
 * @param parser Pointer to the argument parser.
 */
void arg_parser_print_help(arg_parser_t *parser);

/**
 * @brief Free the memory allocated for the parser.
 *
 * @param parser Pointer to the argument parser to be freed.
 */
void arg_parser_free(arg_parser_t *parser);

#endif // TINYARGS_H
