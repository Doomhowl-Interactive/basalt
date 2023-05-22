#include "basalt.h"

#include <windows.h>

typedef struct WindowContext {
    HWND window;
} WindowContext;

typedef struct OffscreenBuffer {
    // NOTE: pixels are 32-bits wide, AA RR GG BB
    BITMAPINFO info;
    Texture mappedCanvas;
    Texture mappedCanvas2;
} OffscreenBuffer;

GameContext Context = { 0 };
GameInput Input = { 0 };

static WindowContext Window = { 0 };
static OffscreenBuffer GlobalBackbuffer = { 0 };
static bool AllocatedConsole = false;

#define MAX_TITLE_LEN 128
BASALT void SetWindowTitle(const char* title)
{
    // check if changed
    char curTitle[MAX_TITLE_LEN];
    if (GetWindowTextA(Window.window, curTitle, MAX_TITLE_LEN) != 0) {
        if (!TextIsEqual(curTitle, title))
            SetWindowTextA(Window.window, title);
    } else {
        ERR("Failed to set change window title!\n");
    }
}

#define MAX_WORKDIR_LEN 128
BASALT const char* GetWorkingDirectory()
{
    static char workingDir[MAX_WORKDIR_LEN];
    if (!GetCurrentDirectory(MAX_WORKDIR_LEN, workingDir)) {
        ERR("Could not determine working directory %s", GetLastError());
    }
    return workingDir;
}

static String ConsoleLog = {0};
BASALT void BasaltPrintColored(ConsoleColor color, const char* format, ... )
{
    // allocate console string if not already
    if (ConsoleLog.text == NULL)
    {
        String str = MakeString();
        memcpy(&ConsoleLog, &str, sizeof(String));
    }

    static char line[1024];
    va_list list;
    va_start(list, format);
    vsnprintf(line, 1024 - 1, format, list);
    va_end(list);
    strcat(line,"\n");

    static const int colors[] = {0,7,8,9,10,11,12,13,14,15};
    if (AllocatedConsole)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colors[color]);
    }
    printf(line);

    AppendString(&ConsoleLog, line);
}

BASALT String GetBasaltLog();

func void OpenSystemConsole();
func void CloseSystemConsole();

func usize GetMicroseconds()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    now.QuadPart *= 1000000;
    now.QuadPart /= frequency.QuadPart;
    return now.QuadPart;
}

func void SleepMicroseconds(usize micros)
{
    usize millis = micros / 1000;
    Sleep(millis);
}

func Size GetWindowSize(HWND window)
{
    RECT clientRect;
    GetClientRect(window, &clientRect);
    Size size = {
        clientRect.right - clientRect.left,
        clientRect.bottom - clientRect.top,
    };
    return (size);
}

func void ResizeDIBSection(OffscreenBuffer* buffer, int width, int height)
{
    // switch out screen textures for correctly sized ones
    DisposeTexture(Context.canvas);
    Context.canvas = InitTexture(width, height);

    DisposeTexture(buffer->mappedCanvas);
    buffer->mappedCanvas = InitTexture(width, height);
    buffer->mappedCanvas2 = InitTexture(width, height);

    buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
    buffer->info.bmiHeader.biWidth = width;
    buffer->info.bmiHeader.biHeight = -height;
    buffer->info.bmiHeader.biPlanes = 1;
    buffer->info.bmiHeader.biBitCount = 32;
    buffer->info.bmiHeader.biCompression = BI_RGB;
}

func void HandleKeyEvent(WPARAM wParam, bool pressed)
{
    // filter out other stuff
    uchar key = (uchar)wParam;
    Input.pressedKeys[key] = pressed;

    if (pressed)
        Input.pressedKeysOnce[key] = true;
}

static void DisplayBufferInWindow(HDC deviceContext, int winWidth, int winHeight, OffscreenBuffer buffer)
{
    CopyTextureInto(buffer.mappedCanvas, Context.canvas);
    MapTextureToCorrectFormat(buffer.mappedCanvas2, buffer.mappedCanvas);

    StretchDIBits(deviceContext,
                  /*
                  dest: X, Y, Width, Height,
                  source: X, Y, Width, Height,
                  */
                  0,
                  0,
                  winWidth,
                  winHeight,
                  0,
                  0,
                  buffer.mappedCanvas2.width,
                  buffer.mappedCanvas2.height,
                  buffer.mappedCanvas2.pixels,
                  &buffer.info,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}

LRESULT CALLBACK MainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch (message) {
        case WM_CLOSE:
            Context.shouldClose = true;
            break;
        case WM_ACTIVATEAPP:
            OutputDebugStringA("WM_ACTIVATEAPP\n");
            break;
        case WM_DESTROY:
            Context.shouldClose = true;
            break;
        case WM_PAINT: {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window, &paint);
            Size size = GetWindowSize(window);
            DisplayBufferInWindow(deviceContext, size.width, size.height, GlobalBackbuffer);
            EndPaint(window, &paint);
        } break;
        case WM_KEYDOWN: {
            HandleKeyEvent(wParam, true);
            if (IsKeyPressed(KEY_Q))
                Context.shouldClose = true;
        } break;
        case WM_KEYUP:
            HandleKeyEvent(wParam, false);
            break;
        default: {
            // handle other messages we don't care about
            result = DefWindowProc(window, message, wParam, lParam);
        } break;
    }

    return (result);
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int showCode)
{
    GameConfig config = ConfigureGame();

    WNDCLASS windowClass = { 0 };

    ResizeDIBSection(&GlobalBackbuffer, config.width, config.height);

    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.lpfnWndProc = MainWindowCallback;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = "HandmadeHerowindowClass";

    if (RegisterClassA(&windowClass)) {
        HWND window = CreateWindowExA(0,
                                      windowClass.lpszClassName,
                                      "Handmade Hero",
                                      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      config.width,
                                      config.height,
                                      0,
                                      0,
                                      instance,
                                      0);
        Window.window = window;

        // Check launch arguments first
        if (!ParseLaunchArguments(__argc, __argv))
        {
            return EXIT_SUCCESS;
        }

        if (Config.hasUnitTesting)
        {
            UnitTest();
        }

        if (Config.hasConsole)
        {
            OpenSystemConsole();
        }

        printf("color 4f");

        if (Window.window) {
            HDC deviceContext = GetDC(window);

            InitializeGame();

            double maxFps = Config.unlockedFramerate ? 10000 : 60;
            double delta = 1.0 / maxFps;
            double fps = maxFps;
            while (!Context.shouldClose) {
                MSG message;
                Size size = GetWindowSize(window);

                // Poll inputs
                POINT p;
                GetCursorPos(&p);
                ScreenToClient(window, &p);

                float scaleX = size.width / (float)config.width;
                float scaleY = size.height / (float)config.height;

                Input.mousePos.x = Clamp(p.x / scaleX, 0, config.width);
                Input.mousePos.y = Clamp(p.y / scaleY, 0, config.height);

                while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
                    if (message.message == WM_QUIT)
                        Context.shouldClose = true;

                    TranslateMessage(&message);
                    DispatchMessageA(&message);
                }

                // begin time measurement
                usize startTime = GetMicroseconds();

                // do updateing and drawing
                UpdateAndRenderGame(Context.canvas, (float)delta);
                Context.frameIndex++;
                Context.timeElapsed += delta;

                DisplayBufferInWindow(deviceContext, size.width, size.height, GlobalBackbuffer);

                // brake engine to respect frame rate cap
                usize interTime = GetMicroseconds();

                usize interMicros = interTime - startTime;
                usize maxMicros = 1.0 / maxFps * 1000000;
                long waitMicros = maxMicros - interMicros;
                if (waitMicros > 0 && interMicros < maxMicros)
                    SleepMicroseconds(waitMicros);

                // stop time measurement
                usize endTime = GetMicroseconds();
                usize elapsedMicros = endTime - startTime;
                delta = elapsedMicros / 1000000.0;
                fps = 1.0 / delta;

                // set window title
                static double timer = 0.f;
                if (timer > 0.2) {
                    // set window title to framerate
                    const char* title = FormatText("%s - %d FPS - %f delta", config.title, (int)fps, delta);
                    SetWindowTitle(title);
                    timer = 0.0;
                }
                timer += delta;

                // clear keys held down
                memset(Input.pressedKeysOnce, 0, sizeof(Input.pressedKeysOnce));
            }
        } else {
            // TODO(casey): Logging
        }
    } else {
        // TODO(casey): Logging
    }

    DisposeGame();

    if (Config.hasConsole)
        CloseSystemConsole();

    return 0;
}

func void OpenSystemConsole()
{
    if (!AllocatedConsole) {
        AllocConsole();
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        AllocatedConsole = true;
    } else {
        ERR("Failed to allocate console!");
    }
    printf("Allocated Windows console");
}

func void CloseSystemConsole()
{
    if (AllocatedConsole)
        FreeConsole();
}
