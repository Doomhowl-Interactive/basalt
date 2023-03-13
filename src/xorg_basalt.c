// Reference: X11 App in C with Xlib - Tsoding Daily
// https://youtu.be/764fnfEb1_c

#include "basalt.h"

#ifdef LINUX

#include <X11/Xlib.h>

int main(int argc, char **argv) {
    DEBUG("Opening Xorg display...");
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

    int x = 0;
    XEvent e;
    while (XPending(display) > 0) {
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
    DEBUG("Closed Xorg display");
    return 0;
}

#endif
