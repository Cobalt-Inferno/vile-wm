#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    KeySym keysym;
    unsigned int mod;
} Keys;



const char term[] = "alacritty & disown";
const char load[] = "dmenu_run &";
int main(void)
{
    short focusedOnWindow;
    Display * dpy;
    XWindowAttributes attr;
    XButtonEvent start;
    XEvent ev;
    if(!(dpy = XOpenDisplay(0x0))) return 1;
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("v")), Mod1Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("f")), Mod1Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("t")), Mod1Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabButton(dpy, 1, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

    start.subwindow = None;
    for(;;)
    {
        XNextEvent(dpy, &ev);
        focusedOnWindow = (ev.type == KeyPress && ev.xkey.subwindow != None);
        if (focusedOnWindow && ev.xkey.keycode == XKeysymToKeycode(dpy, XStringToKeysym("f")))
        {
            system(term);
        }

        else if (focusedOnWindow && ev.xkey.keycode == XKeysymToKeycode(dpy, XStringToKeysym("t")))
        {
            system(load);
        }
        else if (focusedOnWindow && ev.xkey.keycode == XKeysymToKeycode(dpy, XStringToKeysym("v")))
            system("killall xinit");

        else if(ev.type == KeyPress && ev.xkey.subwindow != None) {
            XRaiseWindow(dpy, ev.xkey.subwindow);
        }
        else if(ev.type == ButtonPress && ev.xbutton.subwindow != None)
        {
//            XRaiseWindow(dpy, ev.xbutton.subwindow);
            XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
            start = ev.xbutton;
        }
        else if(ev.type == MotionNotify && start.subwindow != None)
        {
 //           XRaiseWindow(dpy, ev.xbutton.subwindow);
            int xdiff = ev.xbutton.x_root - start.x_root;
            int ydiff = ev.xbutton.y_root - start.y_root;
            XMoveResizeWindow(dpy, start.subwindow,
                attr.x + (start.button==1 ? xdiff : 0),
                attr.y + (start.button==1 ? ydiff : 0),
                MAX(1, attr.width + (start.button==3 ? xdiff : 0)),
                MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
        }
        else if(ev.type == ButtonRelease)
            start.subwindow = None;
            //XUngrabPointer(dpy, CurrentTime);
    }
}

