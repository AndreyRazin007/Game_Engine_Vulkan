#include "platform.h"

/* Windows platform layer. */
#if KEY_PLATFORM_WINDOWS

#include "../core/logger.h"

#include <stdlib.h>
#include <windows.h>

/* Param input extraction. */
#include <windowsx.h>

typedef struct InternalState
{
    HINSTANCE h_instance;
    HWND hwnd;
} internal_state_t;

/* Clock. */
static f64 clock_frequency;
static LARGE_INTEGER start_time;

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 message,
                                       WPARAM w_param, LPARAM l_param);

b8 platform_startup(platform_state_t *platform_state, const char *name,
                    i64 x, i64 y, i64 width, i64 height)
{
    platform_state->internal_state = malloc(sizeof(internal_state_t));
    internal_state_t *state = (internal_state_t *)platform_state->internal_state;
    state->h_instance = GetModuleHandleA(0);

    /* Setup and register window class. */
    HICON icon = LoadIcon(state->h_instance, IDI_APPLICATION);
    WNDCLASSA wnd_classa;
    memset(&wnd_classa, 0, sizeof(wnd_classa));

    /* Get double-clicks. */
    wnd_classa.style = CS_DBLCLKS;

    wnd_classa.lpfnWndProc = win32_process_message;
    wnd_classa.cbClsExtra = 0;
    wnd_classa.cbWndExtra = 0;
    wnd_classa.hInstance = state->h_instance;
    wnd_classa.hIcon = icon;

    /* NULL, manage the cursor manually. */
    wnd_classa.hCursor = LoadCursor(NULL, IDC_ARROW);

    /* Transparent. */
    wnd_classa.hbrBackground = NULL;
    wnd_classa.lpszClassName = "window_class";

    if (!RegisterClassA(&wnd_classa))
    {
        MessageBoxA(0, "Window registration failed", "Error",
                    MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    /* Create window. */
    u64 client_x = x;
    u64 client_y = y;
    u64 client_width = width;
    u64 client_height = height;

    u64 window_x = client_x;
    u64 window_y = client_y;
    u64 window_width = client_width;
    u64 window_height = client_height;

    u64 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u64 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    /* Obtain the size of the border. */
    RECT border_rectangle = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rectangle, window_style, 0, window_ex_style);

    /* In this case, the border rectangle is negative. */
    window_x += border_rectangle.left;
    window_y += border_rectangle.top;

    /* Grow by the size of the OS border. */
    window_width += border_rectangle.right - border_rectangle.left;
    window_height += border_rectangle.bottom - border_rectangle.top;

    HWND handle = CreateWindowExA(window_ex_style, "window_class", name,
                                  window_style, window_x, window_y,
                                  window_width, window_height, 0, 0,
                                  state->h_instance, 0);

    if (handle == 0)
    {
        MessageBoxA(NULL, "Window creation failed!", "Error!",
                    MB_ICONEXCLAMATION | MB_OK);

        KEY_FATAL("Window creation failed!");
        return false;
    }
    else
    {
        state->hwnd = handle;
    }

    /* Show the window. */

    /* If the window should not accept input, this should be false. */
    b32 should_activate = 1;
    i32 show_window_command_flags = should_activate ? SW_SHOW
                                                    : SW_SHOWNOACTIVATE;

    /* If initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE;
       If initially maximized, use SW_SHOWMAXIMIZED : SW_MAXIMIZE */
    ShowWindow(state->hwnd, show_window_command_flags);

    /* Clock setup */
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);

    return true;
}

void platform_shutdown(platform_state_t *platform_state)
{
    /* Simply cold-cast to the known type. */
    internal_state_t *state = (internal_state_t *)platform_state->internal_state;

    if (state->hwnd)
    {
        DestroyWindow(state->hwnd);
        state->hwnd = 0;
    }
}

b8 platform_pump_messages(platform_state_t *platform_state)
{
    MSG message;

    while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return true;
}

void *platform_allocate(u64 size, b8 aligned)
{
    return malloc(size);
}

void platform_free(void *block, b8 aligned)
{
    free(block);
}

void *platform_zero_memory(void *block, u64 size)
{
    return memset(block, 0, size);
}

void *platform_copy_memory(void *dest, const void *source, u64 size)
{
    return memcpy(dest, source, size);
}

void *platform_set_memory(void *dest, i64 value, u64 size)
{
    return memset(dest, value, size);
}

void platform_console_write(const char *message, u8 color)
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    /* FATAL, ERROR, WARNING, INFO, DEBUG, TRACE. */
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};

    SetConsoleTextAttribute(console_handle, levels[color]);
    OutputDebugStringA(message);

    u64 length = strlen(message);
    LPDWORD number_written = 0;

    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length,
                  number_written, 0);
}

void platform_console_write_error(const char *message, u8 color)
{
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);

    /* FATAL, ERROR, WARNING, INFO, DEBUG, TRACE. */
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};

    SetConsoleTextAttribute(console_handle, levels[color]);
    OutputDebugStringA(message);

    u64 length = strlen(message);
    LPDWORD number_written = 0;

    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length,
                  number_written, 0);
}

f64 platform_get_absolute_time()
{
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);

    return (f64)now_time.QuadPart * clock_frequency;
}

void platform_sleep(u64 milliseconds)
{
    Sleep(milliseconds);
}

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 message, WPARAM w_param, LPARAM l_param)
{
    switch (message)
    {
    case WM_ERASEBKGND:
        /* Notify the OS that erasing will be handled by the application
               to prevent flicker. */
        return 1;
    case WM_CLOSE:
        /* Fire an event for the application to quit. */
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
    {
        /* Get the updated size.
           RECT r;
           GetClientRect(hwnd, &r);
           u32 width = r.right - r.left;
           u32 height = r.bottom - r.top;

           Fire an event for window resize. */
    }
    break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        /* Key pressed/released
           b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
           input processing. */
    }
    break;
    case WM_MOUSEMOVE:
    {
        /* Mouse move
           i32 x_position = GET_X_LPARAM(l_param);
           i32 y_position = GET_Y_LPARAM(l_param);
           input processing. */
    }
    break;
    case WM_MOUSEWHEEL:
    {
        /* i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
           if (z_delta != 0)
           {
               Flatten the input to an OS-independent (-1, 1)
               z_delta = (z_delta < 0) ? -1 : 1;
               input processing.
           } */
    }
    break;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    {
        /* b8 pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
           input processing. */
    }
    break;
    }

    return DefWindowProcA(hwnd, message, w_param, l_param);
}

/* End Windows platform layer. */
#endif
