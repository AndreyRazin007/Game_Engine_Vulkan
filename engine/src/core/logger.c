/* Custom headers. */
#include "logger.h"
#include "asserts.h"
#include "../platform/platform.h"

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

    b8 is_error = level < WARNING;

    /* Size of logs */
    const i64 message_length = 32000;
    char out_message_1[message_length];
    memset(out_message_1, 0, sizeof(out_message_1));

    /* Realization of variable number of parameters. */
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message_1, message_length, message, arg_ptr);
    va_end(arg_ptr);

    char out_message_2[message_length];
    sprintf(out_message_2, "%s%s\n", level_strings[level], out_message_1);

    /* Output for Windows. */
    if (is_error)
    {
        platform_console_write_error(out_message_2, level);
    }
    else
    {
        platform_console_write(out_message_2, level);
    }
}

/* Error message output. */
void report_assertion_failure(const char *expression, const char *message,
                              const char *file, i64 line)
{
    log_output(FATAL, "Assertion Failure: %s, message: '%s', ", expression,
               message);
    log_output(FATAL, "in file: %s, line: %d\n", file, line);
}
