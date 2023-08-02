#ifndef PLATFORM_H
#define PLATFORM_H

#include "../defines.h"

typedef struct PlatformState
{
    void *internal_state;
} platform_state_t;

/* Platform creation. */
KEY_API b8 platform_startup(platform_state_t *platform_state, const char *name,
                    i64 x, i64 y, i64 width, i64 height);

/* Platform closure. */
KEY_API void platform_shutdown(platform_state_t *patform_state);

/* Messages about platform pumps. */
KEY_API b8 platform_pump_messages(platform_state_t *platform_state);

/* Allocation of platform resources. */
void *platform_allocate(u64 size, b8 aligned);

/* Freeing up the platform's resources. */
void platform_free(void *block, b8 aligned);

/* Releasing the platform memory. */
void *platform_zero_memory(void *block, u64 size);

/* Copying the platform's memory. */
void *platform_copy_memory(void *dest, const void *source, u64 size);

/* Memory allocation of the platform. */
void *platform_set_memory(void *dest, i64 value, u64 size);

/* Writing platform information to the console. */
void platform_console_write(const char *message, u8 color);

/* Writing platform error information to the console. */
void platform_console_write_error(const char *message, u8 color);

/* Obtaining the full operating time of the platform. */
f64 platform_get_absolute_time();

/* Sleep on the thread for the provided ms. This blocks the main thread.
Should only be used for giving time back to the OS for unused update power.
Therefore it is not exported. */
void platform_sleep(u64 milliseconds);

#endif
