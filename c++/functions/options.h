#include "argtable3/argtable3.h"
#include "string.h"

#define ARGPARSE_CONCAT_I(a, b)         a##b
#define ARGPARSE_CONCAT(a, b)           ARGPARSE_CONCAT_I(a, b)
#define ARGPARSE_REMOVE_PARENS(tuple)   tuple
#define ARGPARSE_VARIADIC_TO_TUPLE(...) (__VA_ARGS__)

/* #region size */
// clang-format off
#define ARGPARSE_SIZE_I(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,N,...) N
#define ARGPARSE_SIZE(...) ARGPARSE_SIZE_I(__VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
// clang-format on
/* #endregion */
/* #region at */
// clang-format off
#define ARGPARSE_FIRST(a, ...)  a
#define ARGPARSE_REST(a, ...)   __VA_ARGS__
#define ARGPARSE_AT_0(a, ...)   a
#define ARGPARSE_AT_1(a, ...)   ARGPARSE_AT_0(__VA_ARGS__)
#define ARGPARSE_AT_2(a, ...)   ARGPARSE_AT_1(__VA_ARGS__)
#define ARGPARSE_AT_3(a, ...)   ARGPARSE_AT_2(__VA_ARGS__)
#define ARGPARSE_AT_4(a, ...)   ARGPARSE_AT_3(__VA_ARGS__)
#define ARGPARSE_AT_5(a, ...)   ARGPARSE_AT_4(__VA_ARGS__)
#define ARGPARSE_AT_6(a, ...)   ARGPARSE_AT_5(__VA_ARGS__)
#define ARGPARSE_AT_7(a, ...)   ARGPARSE_AT_6(__VA_ARGS__)
#define ARGPARSE_AT_8(a, ...)   ARGPARSE_AT_7(__VA_ARGS__)
#define ARGPARSE_AT_9(a, ...)   ARGPARSE_AT_8(__VA_ARGS__)
#define ARGPARSE_AT_10(a, ...)  ARGPARSE_AT_9(__VA_ARGS__)
#define ARGPARSE_AT_11(a, ...)  ARGPARSE_AT_10(__VA_ARGS__)
#define ARGPARSE_AT_12(a, ...)  ARGPARSE_AT_11(__VA_ARGS__)
#define ARGPARSE_AT_13(a, ...)  ARGPARSE_AT_12(__VA_ARGS__)
#define ARGPARSE_AT_14(a, ...)  ARGPARSE_AT_13(__VA_ARGS__)
#define ARGPARSE_AT_15(a, ...)  ARGPARSE_AT_14(__VA_ARGS__)
#define ARGPARSE_AT_16(a, ...)  ARGPARSE_AT_15(__VA_ARGS__)
#define ARGPARSE_AT_17(a, ...)  ARGPARSE_AT_16(__VA_ARGS__)
#define ARGPARSE_AT_18(a, ...)  ARGPARSE_AT_17(__VA_ARGS__)
#define ARGPARSE_AT_19(a, ...)  ARGPARSE_AT_18(__VA_ARGS__)
#define ARGPARSE_AT_N(n, tuple) ARGPARSE_CONCAT(ARGPARSE_AT_, n) tuple
#define ARGPARSE_AT(n, tuple)   ARGPARSE_AT_N(n, tuple)
// clang-format on
/* #endregion */
/* #region foreach */
// clang-format off
#define ARGPARSE_FOREACH_1(macro, a)       macro(a)
#define ARGPARSE_FOREACH_2(macro, a, ...)  macro(a) ARGPARSE_FOREACH_1(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_3(macro, a, ...)  macro(a) ARGPARSE_FOREACH_2(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_4(macro, a, ...)  macro(a) ARGPARSE_FOREACH_3(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_5(macro, a, ...)  macro(a) ARGPARSE_FOREACH_4(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_6(macro, a, ...)  macro(a) ARGPARSE_FOREACH_5(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_7(macro, a, ...)  macro(a) ARGPARSE_FOREACH_6(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_8(macro, a, ...)  macro(a) ARGPARSE_FOREACH_7(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_9(macro, a, ...)  macro(a) ARGPARSE_FOREACH_8(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_10(macro, a, ...) macro(a) ARGPARSE_FOREACH_9(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_11(macro, a, ...) macro(a) ARGPARSE_FOREACH_10(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_12(macro, a, ...) macro(a) ARGPARSE_FOREACH_11(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_13(macro, a, ...) macro(a) ARGPARSE_FOREACH_12(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_14(macro, a, ...) macro(a) ARGPARSE_FOREACH_13(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_15(macro, a, ...) macro(a) ARGPARSE_FOREACH_14(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_16(macro, a, ...) macro(a) ARGPARSE_FOREACH_15(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_17(macro, a, ...) macro(a) ARGPARSE_FOREACH_16(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_18(macro, a, ...) macro(a) ARGPARSE_FOREACH_17(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_19(macro, a, ...) macro(a) ARGPARSE_FOREACH_18(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_20(macro, a, ...) macro(a) ARGPARSE_FOREACH_19(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH_N(macro, n, ...)  ARGPARSE_CONCAT(ARGPARSE_FOREACH_, n)(macro, __VA_ARGS__)
#define ARGPARSE_FOREACH(macro, ...)       ARGPARSE_FOREACH_N(macro, ARGPARSE_SIZE(__VA_ARGS__), __VA_ARGS__)
// clang-format on
/* #endregion */

#define ARGPARSE_ARGUMENT_I_lit(short, long, help)   struct arg_lit* long = arg_litn(#short, #long, 0, 1, help)
#define ARGPARSE_ARGUMENT_I_int(short, long, help)   struct arg_int* long = arg_intn(#short, #long, NULL, 0, 1, help)
#define ARGPARSE_ARGUMENT_I_dbl(short, long, help)   struct arg_dbl* long = arg_dbln(#short, #long, NULL, 0, 1, help)
#define ARGPARSE_ARGUMENT_I_str(short, long, help)   struct arg_str* long = arg_strn(#short, #long, NULL, 0, 1, help)
#define ARGPARSE_ARGUMENT_I_file(short, long, help)  struct arg_file* long = arg_filen(#short, #long, NULL, 0, 1, help)
#define ARGPARSE_ARGUMENT_I(type, short, long, help) ARGPARSE_CONCAT(ARGPARSE_ARGUMENT_I_, type)(short, long, help)

#define ARGPARSE_DEFINE_LINE_I(tuple) ARGPARSE_ARGUMENT_I tuple;
#define ARGPARSE_DEFINE_ARG_I(tuple)  ARGPARSE_AT(2, tuple),

#define ARGPARSE_PARSE_SIZE(parse) (sizeof(parse) / sizeof(parse[0]))
#define ARGPARSE_ARG_END(parse)    ((struct arg_end*)parse[ARGPARSE_PARSE_SIZE(parse) - 1])

/**
 * @brief Define a list of arguments.
 *
 * @param parse The name of arguments array.
 * @param ... List of arguments.
 *
 * Each argument is represented as a tuple containing the following elements: <br>
 *   - type: The data type of the argument. Valid types are 'lit', 'int', 'dbl', 'str', and 'file'. WITHOUT quotation
 * marks.
 *   - short: The short name of the argument. WITHOUT quotation marks.
 *   - long: The long name or description of the argument. WITHOUT quotation marks.
 *   - help: Additional help or information about the argument.
 *
 * @example
 * ``` c
 * ARGPARSE_DEFINE(parse,
 *   (file, c, config, "Path to config file."),
 *   (dbl, d, delay, "Delay time in seconds."),
 *   (int, i, id, "Sensor Mode index."),
 * );
 * ```
 */
#define ARGPARSE_DEFINE(parse, ...)                                                        \
    ARGPARSE_FOREACH(ARGPARSE_DEFINE_LINE_I, __VA_ARGS__)                                  \
    struct arg_lit* ARGPARSE__help = arg_lit0(NULL, "help", "display this help and exit"); \
    struct arg_end* ARGPARSE__end = arg_end(20);                                           \
    void* parse[] = {ARGPARSE_FOREACH(ARGPARSE_DEFINE_ARG_I, __VA_ARGS__) ARGPARSE__help, ARGPARSE__end};
#define ARGPARSE_PARSE(parse, argc, argv, help, err_code, exit_code)    \
    do {                                                                \
        int __err = arg_parse(argc, argv, parse);                       \
        if (__err > 0) {                                                \
            arg_print_errors(stderr, ARGPARSE_ARG_END(parse), argv[0]); \
            ARGPARSE_FREE(parse);                                       \
            err_code;                                                   \
        }                                                               \
        if (ARGPARSE__help->count > 0) {                                \
            printf("Usage: %s", argv[0]);                               \
            arg_print_syntax(stdout, parse, "\n");                      \
            printf("%s\n", help);                                       \
            arg_print_glossary(stdout, parse, "  %-25s %s\n");          \
            exit_code;                                                  \
        }                                                               \
    } while (0)
#define ARGPARSE_FREE(parse) arg_freetable(parse, ARGPARSE_PARSE_SIZE(parse))

#define CHECK_REQUIRED(arg, err_code)                \
    do {                                             \
        if (arg->count <= 0) {                       \
            fprintf(stderr, #arg " is required.\n"); \
            err_code;                                \
        }                                            \
    } while (0)
#define GET_CONFIG(config, var_path, var_bin_config) \
    const char* var_path = *config->filename;        \
    bool var_bin_config = (strcmp(var_path + strlen(var_path) - 4, ".bin") == 0);

#if defined(__cplusplus)

static inline int argparse_get_val(struct arg_int* arg) { return *arg->ival; }
static inline double argparse_get_val(struct arg_dbl* arg) { return *arg->dval; }
static inline const char* argparse_get_val(struct arg_str* arg) { return *arg->sval; }
static inline const char* argparse_get_val(struct arg_file* arg) { return *arg->filename; }
/**
 * @brief Get the value of an argument or a default value if the argument is not provided.
 *
 * @param arg The argument to get the value from.
 * @param default The default value to return if the argument is not provided.
 */
#define GET_OR_DEFAULT(arg, default) ((arg)->count > 0) ? argparse_get_val(arg) : (default)

#else

static inline int argparse_get_val_int(struct arg_int* arg) { return *arg->ival; }
static inline double argparse_get_val_dbl(struct arg_dbl* arg) { return *arg->dval; }
static inline const char* argparse_get_val_str(struct arg_str* arg) { return *arg->sval; }
static inline const char* argparse_get_val_file(struct arg_file* arg) { return *arg->filename; }
/**
 * Use the _Generic to impl the function overloading.
 */
#define GET_OR_DEFAULT_VAR_I(arg)               \
    _Generic((arg),                             \
        struct arg_int *: argparse_get_val_int, \
        struct arg_dbl *: argparse_get_val_dbl, \
        struct arg_str *: argparse_get_val_str, \
        struct arg_file *: argparse_get_val_file)(arg)
/**
 * @brief Get the value of an argument or a default value if the argument is not provided.
 *
 * @param arg The argument to get the value from.
 * @param default The default value to return if the argument is not provided.
 */
#define GET_OR_DEFAULT(arg, default) ((arg)->count > 0) ? GET_OR_DEFAULT_VAR_I(arg) : (default)

#endif
