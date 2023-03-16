#include "basalt.h"

#ifdef WINDOWS
#include <windows.h>

typedef struct {
    // NOTE: pixels are 32-bits wide, AA RR GG BB
    BITMAPINFO info;
    Texture canvas;
} OffscreenBuffer;

Point GetMousePosition() {
    // TODO: implement mouse
    return {};
}

static bool ShouldBeRunning;
static OffscreenBuffer GlobalBackbuffer;

static Size GetWindowSize(HWND window) {
    RECT clientRect;
    GetClientRect(window, &clientRect);
    Size size = {
        clientRect.right - clientRect.left,
        clientRect.bottom - clientRect.top,
    };
    return (size);
}

static void ResizeDIBSection(OffscreenBuffer *buffer, int width, int height) {
    MemFree(buffer->canvas.pixels);

    buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
    buffer->info.bmiHeader.biWidth = width;
    buffer->info.bmiHeader.biHeight = -height;
    buffer->info.bmiHeader.biPlanes = 1;
    buffer->info.bmiHeader.biBitCount = 32;
    buffer->info.bmiHeader.biCompression = BI_RGB;
    buffer->canvas = InitTexture(width, height);
}

static void DisplayBufferInWindow(HDC deviceContext, int winWidth,
                                  int winHeight, OffscreenBuffer buffer) {
    StretchDIBits(deviceContext,
                  /*
                  dest: X, Y, Width, Height,
                  source: X, Y, Width, Height,
                  */
                  0, 0, winWidth, winHeight, 0, 0, buffer.canvas.width,
                  buffer.canvas.height, buffer.canvas.pixels, &buffer.info,
                  DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK MainWindowCallback(HWND window, UINT message, WPARAM wParam,
                                    LPARAM lParam) {
    LRESULT result = 0;

    switch (message) {
    case WM_CLOSE:
        {
            ShouldBeRunning = false;
        }
        break;
    case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        }
        break;
    case WM_DESTROY:
        {
            ShouldBeRunning = false;
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window, &paint);
            Size size = GetWindowSize(window);
            DisplayBufferInWindow(deviceContext, size.width, size.height,
                                  GlobalBackbuffer);
            EndPaint(window, &paint);
        }
        break;
    default:
        {
            // handle other messages we don't care about
            result = DefWindowProc(window, message, wParam, lParam);
        }
        break;
    }

    return (result);
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance,
                     LPSTR commandLine, int showCode) {
    WNDCLASS windowClass = {0};

    ResizeDIBSection(&GlobalBackbuffer, WIDTH, HEIGHT);

    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.lpfnWndProc = MainWindowCallback;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = "HandmadeHerowindowClass";

#ifdef BASALT_DEBUG
    UnitTest();
#endif

    if (RegisterClassA(&windowClass)) {
        HWND window = CreateWindowExA(
            0, windowClass.lpszClassName, "Handmade Hero",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, instance, 0);

        if (window) {
            HDC deviceContext = GetDC(window);
            ShouldBeRunning = true;

            InitializeGame();

            while (ShouldBeRunning) {
                MSG message;

                while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
                    if (message.message == WM_QUIT)
                        ShouldBeRunning = false;

                    TranslateMessage(&message);
                    DispatchMessageA(&message);
                }

                Texture canvas = GlobalBackbuffer.canvas;
                float delta = 1.f/60.f;
                UpdateAndRenderGame(canvas, delta);

                Size size = GetWindowSize(window);
                DisplayBufferInWindow(deviceContext, size.width, size.height,
                                      GlobalBackbuffer);
            }
        } else {
            // TODO(casey): Logging
        }
    } else {
        // TODO(casey): Logging
    }

    DisposeGame();

    return (0);
}

void __stdcall WinMainCRTStartup() {
    int result =
        WinMain(GetModuleHandle(0), 0, 0, 0); // TODO: command line args
    ExitProcess(result);
}
#endif
