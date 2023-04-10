// Reference: X11 App in C with Xlib - Tsoding Daily
// https://youtu.be/764fnfEb1_c

#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <X11/extensions/render.h>

#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "basalt.h"
#include "basalt_plat.h"

class(OffscreenBuffer) {
    Display *display;
    Window window;
    Texture canvas;
    Texture mappedCanvas;
    Texture mappedCanvas2;

    GC gc;
    uchar* pixels;
    XImage *image;
    char title[128];
};

GameContext Context     = { 0 };
GameInput Input         = { 0 };

static OffscreenBuffer ActiveBuffer = { 0 };

pubfunc void SetWindowTitle(const char* title) {
    if (ActiveBuffer.display != NULL) {
        // check if changed
        if (strcmp(ActiveBuffer.title, title) != 0)
        {
            strcpy(ActiveBuffer.title, title);
            XStoreName(ActiveBuffer.display,
                       ActiveBuffer.window,
                       title);
        }
    } else {
        ERR("Failed to set change window title!\n");
    }
}

func Size GetMonitorSize(Display* display) {
    int screen = DefaultScreen(display);
    Size size;
    size.width = DisplayWidth(display, screen);
    size.height = DisplayHeight(display, screen);
    return size;
}

func OffscreenBuffer InitOffscreenBuffer(Display *display, Window window,
                                           Texture canvas) {
    OffscreenBuffer buffer = {0};
    buffer.display = display;
    buffer.window = window;
    buffer.gc = XCreateGC(display, window, 0, NULL);
    buffer.canvas = canvas;
    buffer.mappedCanvas = InitTexture(canvas.width, canvas.height);

    // Determine monitor size
    // TODO: this should be done dynamically
    Size size = GetMonitorSize(display);
    buffer.mappedCanvas2 = InitTexture(size.width, size.height);
    DEBUG("Allocated two textures for monitor size %d x %d", size.width, size.height);

    XWindowAttributes wAttribs = {0};
    XGetWindowAttributes(display, window, &wAttribs);

    buffer.image = XCreateImage(display, wAttribs.visual, wAttribs.depth, ZPixmap, 0,
                               (char *)buffer.mappedCanvas2.pixels,
                               buffer.mappedCanvas2.width, buffer.mappedCanvas2.height,
                               32, buffer.mappedCanvas2.width * sizeof(uint32_t));
    return buffer;
}

func void RenderOffscreenBuffer(OffscreenBuffer *buffer, int width, int height) {
    assert(buffer->mappedCanvas.pixels && buffer->canvas.pixels);

    Rect rect = { 0, 0, MIN(width, buffer->mappedCanvas2.width), MIN(height, buffer->mappedCanvas2.height) };

    // FIXME: SLOW 25% performance hit
    MapTextureToCorrectFormat(buffer->mappedCanvas, buffer->canvas);

    // FIXME: SLOW 50% performance hit
    DrawTextureScaled(buffer->mappedCanvas2, buffer->mappedCanvas, R2(rect));

    XPutImage(buffer->display, buffer->window, buffer->gc, buffer->image, 0, 0,
              0, 0, width, height);
}

func void HandleKeyEvent(XEvent event, bool pressed){
    KeySym sym = XLookupKeysym(&event.xkey, 0);
    KeySym upperSym, lowerSym;
    XConvertCase(sym, &lowerSym, &upperSym);
    char* string = XKeysymToString(upperSym);

    // HACK: ignore special characters (for now)
    if (string[1] == '\0')
    {
        char key = string[0];
        Input.pressedKeys[key] = pressed;

        if (pressed)
            Input.pressedKeysOnce[key] = true;

        // DEBUG("%s %c", pressed ? "Pressed":"Released", key);
    }
}

func void DumpKeyboardTable()
{
    DEBUG("Dumping key table");
    for (int i = 33; i < 90; i++)
    {
        DEBUG("key: %d %c %d", i, i, Input.pressedKeys[i]);
    }
    DEBUG("==================");
}

int main(int argc, char **argv) {

    // Check launch arguments first
    if (!ParseLaunchArguments(argc, argv))
        return EXIT_SUCCESS;

    DEBUG("Opening Xorg display...");
    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        FATAL("Failed to open X display!");
    }

    UnitTest();

    Size size = GetMonitorSize(display);

    // HACK: make the window the size of the entire monitor so the DC is big enough

    // if only things were that simple...
    Window win = XCreateSimpleWindow(display, RootWindow(display, DefaultScreen(display)), 10,
                                     10, size.width, size.height, 0, 0, 255);

    // === make window closing work (it's a big deal for some reason)
    Atom wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(display, win, &wmDeleteWindow, 1);

    XSelectInput(display, win,
                 ExposureMask | KeyPressMask | KeyReleaseMask | ResizeRedirectMask);

    Texture canvas = InitTexture(WIDTH, HEIGHT);

    ActiveBuffer = InitOffscreenBuffer(display, win, canvas);

    XMapWindow(display, win);
    XSync(display, false);

    SetWindowTitle(GAME_TITLE);

    // HACK: resize window to game size
    int posX = size.width / 2 - WIDTH / 2;
    int posY = size.height / 2 - HEIGHT / 2;
    XMoveResizeWindow(display, win, posX, posY, WIDTH, HEIGHT);

    srand(time(NULL));
    InitializeGame();
    InitHotReloading();

    int width = WIDTH;
    int height = HEIGHT;

    double maxFps = Config.unlockedFramerate ? 10000:60;
    double prevDelta = 0.f;
    double delta = 1.0 / maxFps;
    double fps = maxFps;
    while (!Context.shouldClose) {
        while (XPending(display) > 0) {
            XEvent event;
            XNextEvent(display, &event);

            switch (event.type) {
            case KeyPress:
                {
                    HandleKeyEvent(event, true);
                    if (IsKeyPressed(KEY_Q))
                        Context.shouldClose = true;

                    if (IsKeyPressed(KEY_L))
                        DumpKeyboardTable();
                }
                break;
            case KeyRelease:
                HandleKeyEvent(event, false);
                break;
            case ClientMessage:
                if ((Atom)event.xclient.data.l[0] == wmDeleteWindow) {
                    DEBUG("Closing window...");
                    Context.shouldClose = true;
                }
                break;
            case ResizeRequest: {
                width = event.xresizerequest.width;
                height = event.xresizerequest.height;
            } break;
            }

            // poll the mouse
            Window rootWinResult, childWinResult;
            int rootMouseX, rootMouseY;
            int childMouseX, childMouseY;
            unsigned int maskResult = 0;
            if (XQueryPointer(display, win, &rootWinResult, &childWinResult, &rootMouseX, &rootMouseY,
                          &childMouseX, &childMouseY, &maskResult)){
                Input.mousePos.x = childMouseX;
                Input.mousePos.y = childMouseY;

                // HACK: might not work while pressing multiple mouse buttons
                Input.isMouseDown = maskResult == 272;
            }

        }

        struct timeval startTime;
        gettimeofday(&startTime, NULL);

        // Set window title to fps and delta-time
        static double timer = 0.f;
        if (timer > 0.2)
        {
            // set window title to framerate
            char title[200] = { 0 };
            sprintf(title, "%s - %d FPS - %f delta", GAME_TITLE, (int)fps, delta);
            SetWindowTitle(title);
            timer = 0.0;
        }

        timer += delta;

        // draw graphics
        if (UpdateAndRenderArchaeo(canvas))
        {
            // HACK: Prevent delta spikes from breaking the game
            if (delta > 1){
                delta = prevDelta;
            }
            UpdateAndRenderGame(canvas, (float) delta);
            Context.frameIndex++;
            Context.timeElapsed += delta;
        }

        RenderOffscreenBuffer(&ActiveBuffer, width, height);

        // throttle the engine to respect capped fps
        struct timeval interTime;
        gettimeofday(&interTime, NULL);

        size_t interMicros = interTime.tv_usec - startTime.tv_usec;
        size_t maxMicros = 1.0 / maxFps * 1000000;
        long waitMicros = maxMicros - interMicros;
        if (waitMicros > 0 && interMicros < maxMicros)
            usleep(waitMicros);

        struct timeval endTime;
        gettimeofday(&endTime, NULL);

        size_t elapsedMicros = endTime.tv_usec - startTime.tv_usec;
        prevDelta = delta;
        delta = elapsedMicros / 1000000.0;
        fps = 1.0 / delta;

        // clear keys
        memset(Input.pressedKeysOnce, 0, sizeof(Input.pressedKeysOnce));

        XEvent expose;
        expose.type = Expose;
        expose.xexpose.window = win;
        XSendEvent(display, win, true, ExposureMask, &expose);
        XFlush(display);
    }

    XCloseDisplay(display);
    DEBUG("Closed Xorg display");
    return EXIT_SUCCESS;
}
