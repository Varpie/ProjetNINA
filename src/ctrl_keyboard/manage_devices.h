#include "write_keyboard.h"
#include <X11/extensions/XInput2.h>

int create_master(Display *dpy, char *name);

int* find_device_id(Display *dpy, char *name, int *nbid);