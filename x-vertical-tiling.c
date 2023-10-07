#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/extensions/Xrandr.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <math.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

void print_usage(char *program_name);
Window get_active_window(Display *display, Window root);
void get_window_offset(Display *display, Window window, long *l, long *r, long *t, long *b);
int get_target_monitor_index(XRRMonitorInfo *monitors, int monitors_count, XWindowAttributes window_attr);

int main(int argc, char *argv[]) {
    Display *display;
    Window root, active_window;
    XWindowAttributes active_window_attr;
    XRRMonitorInfo *monitors_info;
    long l, r, t, b;
    int direction, monitors_count, target_monitor_index, window_target_y;

    if (argc == 2) {
        if (strcmp(argv[1], "up") == 0) {
            direction = 1;
        }
        else if (strcmp(argv[1], "down") == 0) {
            direction = 0;
        }
        else {
            print_usage(argv[0]);
        }
    }
    else {
        print_usage(argv[0]);
    }

    display = XOpenDisplay(NULL);
    root = DefaultRootWindow(display);

    monitors_info = XRRGetMonitors(display, root, True, &monitors_count);
    active_window = get_active_window(display, root);

    XGetWindowAttributes(display, active_window, &active_window_attr);
    get_window_offset(display, active_window, &l, &r, &t, &b);

    target_monitor_index = get_target_monitor_index(monitors_info, monitors_count, active_window_attr);

    window_target_y = monitors_info[target_monitor_index].y;
    if (direction == 0) {
        window_target_y += monitors_info[target_monitor_index].height / 2;
    }

    XMoveResizeWindow(
        display, 
        active_window, 
        monitors_info[target_monitor_index].x - l, 
        window_target_y - t,
        monitors_info[target_monitor_index].width + l + r, 
        monitors_info[target_monitor_index].height / 2 + t + b
    );

    XCloseDisplay(display);
    XRRFreeMonitors(monitors_info);

    return EXIT_SUCCESS;
}

void print_usage(char *program_name) {
    printf("Usage: %s [up|down]\n", program_name);
    printf("Options:\n");
    printf("  up    Move the active window up to the top half of the target monitor.\n");
    printf("  down  Move the active window down to the bottom half of the target monitor.\n");
    exit(EXIT_FAILURE);
}

Window get_active_window(Display *display, Window root) {
    Window focus, focus_root, focus_parent, *focus_children;
    unsigned int focus_nchildren;
    int revert_to;

    XGetInputFocus(display, &focus, &revert_to);

    focus_parent = focus;
    while (focus_parent != root) {
        focus = focus_parent;
        XQueryTree(display, focus, &focus_root, &focus_parent, &focus_children, &focus_nchildren);
        XFree(focus_children);
    }
    
    return focus;
}

void get_window_offset(Display *display, Window window, long *l, long *r, long *t, long *b) {
    Atom atom, actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    long *property;

    atom = XInternAtom(display, "_GTK_FRAME_EXTENTS", False);
    XGetWindowProperty(
        display, window, atom, 0, 4, False, XA_CARDINAL, 
        &actual_type, &actual_format, &nitems, &bytes_after, (unsigned char **)&property
    );

    *l = (property != NULL ? property[0] : 0);
    *r = (property != NULL ? property[1] : 0);
    *t = (property != NULL ? property[2] : 0);
    *b = (property != NULL ? property[3] : 0);

    if (property != NULL) XFree(property);

    return;
}

int get_target_monitor_index(XRRMonitorInfo *monitors, int monitors_count, XWindowAttributes window) {
    int monitor_index, max_area, window_xmax, window_ymax, monitor_xmax, monitor_ymax, dx, dy;

    monitor_index = 0;
    max_area = 0;

    window_xmax = window.x + window.width;
    window_ymax = window.y + window.height;

    for (int i = 0; i < monitors_count; ++i) {
        monitor_xmax = monitors[i].x + monitors[i].width;
        monitor_ymax = monitors[i].y + monitors[i].height;

        dx = min(window_xmax, monitor_xmax) - max(window.x, monitors[i].x);
        dy = min(window_ymax, monitor_ymax) - max(window.y, monitors[i].y);
        
        if (dx >= 0 && dy >= 0 && dx * dy > max_area) {
            monitor_index = i;
            max_area = dx * dy;
        }
    }

    return monitor_index;
}
