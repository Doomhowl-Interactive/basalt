// Reference: X11 App in C with Xlib - Tsoding Daily
// https://youtu.be/764fnfEb1_c

#include "basalt.h"

#ifdef LINUX

#include <X11/Xlib.h>

static bool ShouldBeRunning = true;

int main(int argc, char **argv) {
    DEBUG("Opening Xorg display...");
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        FATAL("Failed to open X display!");
    }

    int screen = DefaultScreen(display);

    // if only things were that simple
    Window win = XCreateSimpleWindow(display, RootWindow(display, screen), 10,
                                     10, WIDTH, HEIGHT, 0, 0, 255);

    // make window closing work (it's a big deal for some reason)
    Atom wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(display, win, &wmDeleteWindow, 1);

    XSelectInput(display, win, ExposureMask | KeyPressMask);
    XMapWindow(display, win);

    XSync(display, false);

    int x = 0;
    XEvent e = {0};
    while (ShouldBeRunning) {
        XNextEvent(display, &e);

        switch (e.type) {
        case Expose:
            XFillRectangle(display, win, DefaultGC(display, screen), 0, 0, x,
                           x);
            break;
        case KeyPress:
            x += 10;
            break;
        case ClientMessage:
            if ((Atom)e.xclient.data.l[0] == wmDeleteWindow) {
                DEBUG("Closing window...");
                ShouldBeRunning = false;
            }
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
