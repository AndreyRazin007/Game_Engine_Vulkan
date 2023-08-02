#ifndef ASSERTS_H
#define ASSERTS_H

#include "../defines.h"

/* Disable assertions by commenting out the below line. */
#define KASSERTIONS_ENABLED

#ifdef KASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debug_break() __debugbreak()
#else
#define debug_beak() __builtin_trap()
#endif

KEY_API void report_assertion_failure(const char *expression,
                                      const char *message,
                                      const char *file, i64 line);

#define KASSERT(expression)                                                \
    {                                                                      \
        if (expression)                                                    \
        {                                                                  \
        }                                                                  \
        else                                                               \
        {                                                                  \
            report_assertion_failure(#expression, "", __FILE__, __LINE__); \
            debug_break();                                                 \
        }                                                                  \
    }

#define KASSERT_MSG(expression, message)                                        \
    {                                                                           \
        if (expression)                                                         \
        {                                                                       \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            report_assertion_failure(#expression, message, __FILE__, __LINE__); \
            debug_break();                                                      \
        }                                                                       \
    }

#ifdef _DEBUG
#define KASSERT_DEBUG(expression)                                          \
    {                                                                      \
        if (expression)                                                    \
        {                                                                  \
        }                                                                  \
        else                                                               \
        {                                                                  \
            report_assertion_failure(#expression, "", __FILE__, __LINE__); \
            debug_break();                                                 \
        }                                                                  \
    }
#else
/* Does nothing at all. */
#define KASSERT_DEBUG(expression)
#endif

#else
/* Does nothing at all. */
#define KASSERT(expression)

/* Does nothing at all. */
#define KASSERT_MSG(expression, message)

/* Does nothing at all. */
#define KASSERT_DEBUG(expression)
#endif

#endif
