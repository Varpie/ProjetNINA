#include "write_keyboard.h"
#include <X11/extensions/XInput2.h>

int create_master(Display *dpy, char *name);

XIDeviceInfo * find_device_id(Display *dpy, char *name, int *nbid);

int link_devices(Display *dpy, XIDeviceInfo slave, XIDeviceInfo master);
