/* Custom headers. */
#include "logger.h"
#include "asserts.h"

/* System headers. */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* Create log file. */
b8 initialize_logging()
{
    return true;
}

/* Cleanup logging/write queued entries. */
void shutdown_logging()
{
}

void log_output(log_level_t level, const char *message, ...)
{
    /* Log levels. */
    const char *level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARNING]: ",
                                    "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};

    /* Size of logs */
    char out_message[32000];
    memset(out_message, 0, sizeof(out_message));

    /* Realization of variable number of parameters. */
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, 32000, message, arg_ptr);
    va_end(arg_ptr);

    char out_message2[32000];
    sprintf(out_message2, "%s%s\n", level_strings[level], out_message);

    /* Output */
    printf("%s", out_message2);
}

/* Error message output. */
void report_assertion_failure(const char *expression, const char *message,
                              const char *file, i32 line)
{
    log_output(FATAL, "Assertion Failure: %s, message: '%s', ", expression, message);
    log_output(FATAL, "in file: %s, line: %d\n", file, line);
}
