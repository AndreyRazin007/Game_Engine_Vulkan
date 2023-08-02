#ifndef LOGGER_H
#define LOGGER_H

#include "defines.h"

/* Enable log levels. */
#define LOG_WARNING_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

/* Disable debug and trace logging for release builds. */
#if KEY_RELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum log_level {
    FATAL = 0,
    ERROR = 1,
    WARNING = 2,
    INFO = 3,
    DEBUG = 4,
    TRACE = 5
} log_level_t;

b8 initialize_logging();
void shutdown_logging();

/* Output program logs. */
KEY_API void log_output(log_level_t level, const char *message, ...);

/* Logs a fatal-level message. */
#define KEY_FATAL(message, ...) log_output(FATAL, message, ##__VA_ARGS__);

#ifndef KEY_ERROR
/* Logs an error-level message. */
#define KEY_ERROR(message, ...) log_output(ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_WARNING_ENABLED == 1
/* Logs a warning-level message. */
#define KEY_WARNING(message, ...) log_output(WARNING, message, ##__VA_ARGS__);
#else
/* Does nothing when LOG_WARN_ENABLED != 1 */
#define KEY_WARNING(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
/* Logs a info-level message. */
#define KEY_INFO(message, ...) log_output(INFO, message, ##__VA_ARGS__);
#else
/* Does nothing when LOG_INFO_ENABLED != 1 */
#define KEY_INFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
/* Logs a debug-level message. */
#define KEY_DEBUG(message, ...) log_output(DEBUG, message, ##__VA_ARGS__);
#else
/* Does nothing when LOG_DEBUG_ENABLED != 1 */
#define KEY_DEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
/* Logs a trace-level message. */
#define KEY_TRACE(message, ...) log_output(TRACE, message, ##__VA_ARGS__);
#else
/* Does nothing when LOG_TRACE_ENABLED != 1 */
#define KEY_TRACE(message, ...)
#endif

#endif
