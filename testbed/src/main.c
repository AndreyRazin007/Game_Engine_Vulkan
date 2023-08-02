#include "../../engine/src/core/logger.h"
#include "../../engine/src/core/asserts.h"

/* Test. */
#include "../../engine/src/platform/platform.h"

int main(void)
{
    KEY_FATAL("A test message: %f", 3.14f);
    KEY_ERROR("A test message: %f", 3.14f);
    KEY_WARNING("A test message: %f", 3.14f);
    KEY_INFO("A test message: %f", 3.14f);
    KEY_DEBUG("A test message: %f", 3.14f);
    KEY_TRACE("A test message: %f", 3.14f);

    KASSERT(0 == 0);

    platform_state_t state;

    if (platform_startup(&state, "Engine Testbed", 100, 100, 1280, 720))
    {
        while (true)
        {
            platform_pump_messages(&state);
        }
    }

    platform_shutdown(&state);

    return 0;
}
