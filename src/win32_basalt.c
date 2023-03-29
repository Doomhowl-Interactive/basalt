#include "basalt.h"
#include <windows.h>

class(OffscreenBuffer) {
    // NOTE: pixels are 32-bits wide, AA RR GG BB
    BITMAPINFO info;
    Texture canvas;
    Texture mappedCanvas;
};

class(SInput) {
    Point mouse;
};
static SInput Input = { 0 };

pubfunc Point GetMousePosition() {
    return Input.mouse;
}

static bool ShouldBeRunning;
static OffscreenBuffer GlobalBackbuffer;

func void OpenSystemConsole();
func void CloseSystemConsole();

func Size GetWindowSize(HWND window) {
    RECT clientRect;
    GetClientRect(window, &clientRect);
    Size size = {
        clientRect.right - clientRect.left,
        clientRect.bottom - clientRect.top,
    };
    return (size);
}

func void ResizeDIBSection(OffscreenBuffer *buffer, int width, int height) {

    // switch out screen textures for correctly sized ones
    DisposeTexture(buffer->canvas);
    buffer->canvas = InitTexture(width, height);

    DisposeTexture(buffer->mappedCanvas);
    buffer->mappedCanvas = InitTexture(width, height);

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

    CopyTextureInto(buffer.mappedCanvas, buffer.canvas);
    MapTextureToCorrectFormat(buffer.mappedCanvas);

    StretchDIBits(deviceContext,
                  /*
                  dest: X, Y, Width, Height,
                  source: X, Y, Width, Height,
                  */
                  0, 0, winWidth, winHeight, 0, 0, buffer.mappedCanvas.width,
                  buffer.mappedCanvas.height, buffer.mappedCanvas.pixels, &buffer.info,
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
    OpenSystemConsole();
#endif

    if (RegisterClassA(&windowClass)) {
        HWND window = CreateWindowExA(
            0, windowClass.lpszClassName, "Handmade Hero",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
            WIDTH, HEIGHT, 0, 0, instance, 0);

        if (window) {
            HDC deviceContext = GetDC(window);
            ShouldBeRunning = true;

            InitializeGame();

            while (ShouldBeRunning) {
                MSG message;
                Size size = GetWindowSize(window);

                // Poll inputs
                POINT p;
                GetCursorPos(&p);
                ScreenToClient(window, &p);

                float scaleX = size.width / (float) WIDTH;
                float scaleY = size.height / (float) HEIGHT;

                Input.mouse.x = Clamp(p.x/scaleX, 0, WIDTH);
                Input.mouse.y = Clamp(p.y/scaleY, 0, HEIGHT);

                while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
                    if (message.message == WM_QUIT)
                        ShouldBeRunning = false;

                    TranslateMessage(&message);
                    DispatchMessageA(&message);
                }

                Texture canvas = GlobalBackbuffer.canvas;
                float delta = 1.f/60.f;
                UpdateAndRenderGame(canvas, delta);

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

#ifdef BASALT_DEBUG
    CloseSystemConsole();
#endif

    return (0);
}

func void OpenSystemConsole() {
    if (AllocConsole()) {
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr); 
    }
    printf("Allocated Windows console");
}

func void CloseSystemConsole() {
    #ifdef WIN32
    FreeConsole();
    #endif
}
