// Reference: X11 App in C with Xlib - Tsoding Daily
// https://youtu.be/764fnfEb1_c

#include "basalt.h"
#include <X11/Xutil.h>
#include <assert.h>
#include <stdint.h>
#include <time.h>
#include <X11/Xlib.h>

static bool ShouldBeRunning = true;

class(OffscreenBuffer) {
    Display *display;
    Window window;
    Texture canvas;
    Texture mappedCanvas;

    GC gc;
    uchar* pixels;
    XImage *image;
    char title[128];
};

class(SInput) {
    bool isMouseDown;
    Point mousePos;
};

static SInput Input = { 0 };
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

pubfunc Point GetMousePosition() {
    return Input.mousePos;
}

pubfunc bool IsMouseDown() {
    return Input.isMouseDown;
}

pubfunc bool IsMouseUp() {
    return !Input.isMouseDown;
}

func Size GetMonitorSize(Display* display) {
    Size size;

    int screen = DefaultScreen(display);
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

    // Determine monitor size
    // TODO: this should be done dynamically
    Size size = GetMonitorSize(display);
    buffer.mappedCanvas = InitTexture(size.width, size.height);
    DEBUG("Allocated texture for monitor size %d x %d", size.width, size.height);

    XWindowAttributes wAttribs = {0};
    XGetWindowAttributes(display, window, &wAttribs);

    buffer.image =
        XCreateImage(display, wAttribs.visual, wAttribs.depth, ZPixmap, 0,
                     (char *)buffer.mappedCanvas.pixels,
                     buffer.mappedCanvas.width, buffer.mappedCanvas.height,
                     32, buffer.mappedCanvas.width * sizeof(uint32_t));
    return buffer;
}

func void RenderOffscreenBuffer(OffscreenBuffer *buffer, int width, int height) {
    assert(buffer->mappedCanvas.pixels && buffer->canvas.pixels);

    Rect rect = { 0, 0, MIN(width, buffer->mappedCanvas.width), MIN(height, buffer->mappedCanvas.height) };
    MapTextureToCorrectFormat(buffer->canvas);
    DrawTextureScaled(buffer->mappedCanvas, buffer->canvas, rect);

    XPutImage(buffer->display, buffer->window, buffer->gc, buffer->image, 0, 0,
              0, 0, width, height);
}

int main(int argc, char **argv) {
    DEBUG("Opening Xorg display...");
    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        FATAL("Failed to open X display!");
    }

#ifdef BASALT_DEBUG
    UnitTest();
#endif

    Size size = GetMonitorSize(display);

    // HACK: make the window the size of the entire monitor so the DC is big enough

    // if only things were that simple...
    Window win = XCreateSimpleWindow(display, RootWindow(display, DefaultScreen(display)), 10,
                                     10, size.width, size.height, 0, 0, 255);

    // === make window closing work (it's a big deal for some reason)
    Atom wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(display, win, &wmDeleteWindow, 1);

    XSelectInput(display, win,
                 ExposureMask | KeyPressMask | ResizeRedirectMask);

    Texture canvas = InitTexture(WIDTH, HEIGHT);

    ActiveBuffer = InitOffscreenBuffer(display, win, canvas);

    XMapWindow(display, win);
    XSync(display, false);

    SetWindowTitle(GAME_TITLE);

    // HACK: resize window to game size
    int posX = size.width / 2 - WIDTH / 2;
    int posY = size.height / 2 - HEIGHT / 2;
    XMoveResizeWindow(display, win, posX, posY, WIDTH, HEIGHT);

    InitializeGame();

    int width = WIDTH;
    int height = HEIGHT;

    double delta = 1.0 / 60.0;
    double fps = 60;
    while (ShouldBeRunning) {
        while (XPending(display) > 0) {
            XEvent event;
            XNextEvent(display, &event);

            switch (event.type) {
            case Expose:
                break;

            case ClientMessage:
                if ((Atom)event.xclient.data.l[0] == wmDeleteWindow) {
                    DEBUG("Closing window...");
                    ShouldBeRunning = false;
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

            clock_t startTime = clock();
            // draw graphics
            UpdateAndRenderGame(canvas, (float) delta);
            RenderOffscreenBuffer(&ActiveBuffer, width, height);

            clock_t endTime = clock();
            delta = (endTime - startTime) / (double)CLOCKS_PER_SEC;
            fps = 1.0 / delta;
        }

        XEvent expose;
        expose.type = Expose;
        expose.xexpose.window = win;
        XSendEvent(display, win, true, ExposureMask, &expose);
        XFlush(display);

        static double timer = 0.f;
        if (timer > 0.2)
        {
            // set window title to framerate
            char title[200] = { 0 };
            sprintf(title, "%s - %d FPS", GAME_TITLE, (int)fps);
            SetWindowTitle(title);
            timer = 0.0;
        }
        timer += delta;
    }

    XCloseDisplay(display);
    DEBUG("Closed Xorg display");
    return 0;
}
