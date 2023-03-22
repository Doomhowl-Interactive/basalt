// Reference: X11 App in C with Xlib - Tsoding Daily
// https://youtu.be/764fnfEb1_c

#include "basalt.h"
#include <X11/Xutil.h>
#include <assert.h>
#include <stdint.h>
#include <X11/Xlib.h>

static bool ShouldBeRunning = true;

class(OffscreenBuffer) {
    Display *display;
    Window window;
    Texture canvas;

    GC gc;
    XImage *image;
    Texture monitorCanvas;
};

class(SInput) {
    bool isMouseDown;
    Point mousePos;
};
static SInput Input = { 0 };

pubfunc Point GetMousePosition() {
    return Input.mousePos;
}

pubfunc bool IsMouseDown() {
    return Input.isMouseDown;
}

pubfunc bool IsMouseUp() {
    return !Input.isMouseDown;
}

func OffscreenBuffer InitOffscreenBuffer(Display *display, Window window,
                                           Texture canvas) {
    OffscreenBuffer buffer = {0};
    buffer.display = display;
    buffer.window = window;
    buffer.gc = XCreateGC(display, window, 0, NULL);
    buffer.canvas = canvas;
    buffer.monitorCanvas = InitTexture(1920, 1080);

    XWindowAttributes wAttribs = {0};
    XGetWindowAttributes(display, window, &wAttribs);

    buffer.image =
        XCreateImage(display, wAttribs.visual, wAttribs.depth, ZPixmap, 0,
                     (char *)buffer.monitorCanvas.pixels,
                     buffer.monitorCanvas.width, buffer.monitorCanvas.height,
                     32, buffer.monitorCanvas.width * sizeof(uint32_t));
    return buffer;
}

func void RenderOffscreenBuffer(OffscreenBuffer *buffer, int width,
                                  int height) {
    assert(buffer->monitorCanvas.pixels && buffer->canvas.pixels);

    DrawTexture(buffer->monitorCanvas, buffer->canvas, 0, 0);

    XPutImage(buffer->display, buffer->window, buffer->gc, buffer->image, 0, 0,
              0, 0, width, height);
}

int main(int argc, char **argv) {
    DEBUG("Opening Xorg display...");
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        FATAL("Failed to open X display!");
    }

#ifdef BASALT_DEBUG
    UnitTest();
#endif

    int screen = DefaultScreen(display);

    // if only things were that simple...
    Window win = XCreateSimpleWindow(display, RootWindow(display, screen), 10,
                                     10, WIDTH, HEIGHT, 0, 0, 255);

    // === make window closing work (it's a big deal for some reason)
    Atom wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(display, win, &wmDeleteWindow, 1);

    XSelectInput(display, win,
                 ExposureMask | KeyPressMask | ResizeRedirectMask);
    // ================

    Texture canvas = InitTexture(WIDTH, HEIGHT);

    OffscreenBuffer buffer = InitOffscreenBuffer(display, win, canvas);

    XMapWindow(display, win);

    XSync(display, false);

    InitializeGame();

    int width = WIDTH;
    int height = HEIGHT;

    XEvent event = {0};
    while (ShouldBeRunning) {
        while (XPending(display) > 0) {
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

            // draw graphics
            float delta = 1.f / 60.f;
            UpdateAndRenderGame(canvas, delta);
            RenderOffscreenBuffer(&buffer, width, height);
        }

        XEvent expose;
        expose.type = Expose;
        expose.xexpose.window = win;
        XSendEvent(display, win, true, ExposureMask, &expose);
        XFlush(display);
    }

    XCloseDisplay(display);
    DEBUG("Closed Xorg display");
    return 0;
}
