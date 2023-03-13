// Reference: X11 App in C with Xlib - Tsoding Daily
// https://youtu.be/764fnfEb1_c

#include <X11/Xlib.h>

#define BASALT_STDLIB_IMPLEMENTATION
#include "basalt.h"

// core assets
extern unsigned char SPR_RGBA[];
extern unsigned char SPR_PLAYER_FOX[];
extern unsigned char SPR_BLOCK[];
extern unsigned char TILE_BLOCK_SMALL[];

int main(int argc, char **argv) {
    DEBUG("Opening Xorg server...");

    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        FATAL("Failed to open X display!");
    }

    int screen = DefaultScreen(display);
    Window win = XCreateSimpleWindow(display, RootWindow(display, screen), 10,
                                     10, WIDTH, HEIGHT, 0, 0, 255);

    XSelectInput(display, win, ExposureMask | KeyPressMask);
    XMapWindow(display, win);

    XSync(display, false);

    const char *msg = "Hello world!";

    int x = 0;
    XEvent e;
    while (1) {
        XNextEvent(display, &e);

        switch (e.type) {
        case Expose:
            XFillRectangle(display, win, DefaultGC(display, screen), 0, 0, x,
                           x);
            break;
        case KeyPress:
            x += 10;
            break;
        }

        XEvent expose;
        expose.type = Expose;
        expose.xexpose.window = win;
        XSendEvent(display, win, false, ExposureMask, &expose);
        XFlush(display);
    }

    XCloseDisplay(display);
    return 0;
}
