#ifndef WRITE_KB_
#define WRITE_KB_

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include <wctype.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>

#define MAX_SHIFT_LEVEL 3
#define SHIFT 42
#define ALT_GR 100
#define CTRL_KEY 29
#define SIZE_MAP 256

/* Globals */
static int uinp_fd = -1;
static const char *DEVICE_NAME  = "Custom Device";
static XIDeviceInfo * master_ptr;
static const char test_text[] = "this is a simple test to get your typing speed";
static const int size_text = 46;
static const double time_limit = 2000000.0;
static const double base_time = 100000.0;
static const int base_n = 1;
static const char *delimiter = ";";
static const char name_conf[] = "/var/nina/timed_keys";

typedef struct act_mean{
  double mean;
  int n;
}act_mean;

extern act_mean map[SIZE_MAP][SIZE_MAP];

//static int box_muller_v = -1;
//realtive url !!!
//static const char name_conf[] = "/home/etienne/ProjetNINA/config/conf";
// static const char name_conf[] = "../../config/conf";

/* uInput device structure */
struct uinput_user_dev uinp;
/* Input device structure */
struct input_event event;

/*
 *Setup the uinput device
 *\return -1 if error, 0 if OK.
 */
int setup_uinput_device(void);

/* Implementation for mouse event */
void send_click_events(int x, int y);

/*Send an event of value = 1 and type = EV_KEY (press of key) */
void press_a_button(int key);

/*Send an event of value =  and type = EV_KEY (release of key) */
void release_a_button(int key);

/**
 * Convert a UTF-16 character to it's KeySym
 * \param ucs long UTF-16 character
 * \return KeySym corresponding KeySym
 */
KeySym ucs2keysym(long ucs);
/*
 * Destroy uinput device
 * \return -1 if error, 0 if ok
 */
int destroy_uinput_device(void);
/**
 * Convert a char* to a wchar_t* to hanle multibytes encoded character (like €)
 * Then call the write_widechar method for each character
 * \param str char* The string wanted to be wrote
 */
int write_string(char *str);
/**
 * Write the character defined by its Unicode and return the value of the keycode sent
 * \param ucs long character to be wrote in Unicode (UTF-16)
 * \param prev_key int keycode of the previous key pressed
 * \return int keycode sent
 */
int write_widechar(long ucs,int prev_key);
/**
 * Send press and release of a key with the SHIFT modifier
 * \param key int Keycode of the key pressed
 */
void send_key_with_shift(int key);
/**
 * Send press and release of a key with the ALT_GR modifier
 * \param key int Keycode of the key pressed
 */
void send_key_with_altgr(int key);
/**
 * Send press and release of a key with the SHIFT and ALT_GR modifiers
 * \param key int Keycode of the key pressed
 */
void send_key_with_altgr_shift(int key);
/**
 * Send press and release of a key with CTRL modifier
 * \param key int Keycode of the key pressed
 */
void send_key_with_ctrl(int key);
/**
 * Send press and release of a key
 * \param key int Keycode of the key pressed
 */
void send_key(int key);
/**
 * Sleep for a certain time defined in the map (see timed_keystrokes.c)
 * \param currkey int Keycode of the key that will be pressed
 * \param prevkey int Keycode of the key that have been pressed
 */
void key_delay(int currkey, int prevkey);

/**
 * Wrapper of destroy_uinput_device and ungrab_focus
 *\return 0 if OK, -1 if error
 */
int free_kbd(void);
/**
 * Wrapper of setup_uinput_device, load_map and grab_focus
 * \return 0 if OK, -1 if error
 */
int setup_kbd(int pid);

/**
  * Destroy the master device of the application
  * \return 0 if OK, -1 if error
  */
int ungrab_focus(void);

/**
 * Search for the X informations about the keyboard created with uinput.
 * Create a new master device.
 * Link the uinput device to the new master device
 * Grab the focus of the browser launched by the application by sending a click event.
 */
int grab_focus(int pid);
/**
 * Search in all the window managed by X a window with a certain pid.
 *\param dpy Display* pointer to the X display
 *\param pid unsigned long pid of the window searhed for.
 *\return An Window struct containing the information of the window found. NULL if no window with @pid has been found. 
 */
Window get_window_by_pid(Display *dpy, unsigned long pid);

/**
 * Write the two parameters in a file named conf in the root of the program
 * \param m double mean
 * \param sig double standard deviation
 */
void writeConfFile(double m, double sig);
/*
 * Calculate the mean of the array passed in argument
 */
double esperance(double t[]);
/*
 * Calculate the standard deviation of the array passed in argument
 */
double std_dev(double t[]);
/**
 * Ask the user to type a phrase. Log the time of the input and calculate
 * the difference between each time
 */
void ask_keystrokes(void);
/**
 * Load the conf file and store it in the map
 */
void load_map(void);
/**
 * Write the map into the conf file
 */
void update_mapconf(void);
/**
 * Create a conf file from the standard map (mean and n define by base_time and base_n)
 */
void create_mapconf(void);
/**
 * Listen the input of /dev/event0 during @timer seconds and store the difference
 * bewteen the time of the previous key and the time of the actual key pressed
 * in a map
 *\param time int number of seconds the program will listen to /dev/event0
 */
void keystroke_time(int time);

/**
 * add a new master device to the associated display
 *\param dpy Display* pointer to the display
 *\param name char* name of the device created
 *\return 0 if no error, -1 else
 */
int create_master(Display *dpy, char *name);
/**
 * Remove a master device to the associated display
 *\param dpy Display* pointer to the display
 *\param id int id of the master device
 *\return 0 if no error, -1 else
 */
int remove_master(Display *dpy, int id);
/**
 * Find the ids of devices associated to a certain name
 *\param dpy Display* pointer to the display
 *\param name char* name of the devices searched
 *\param nbid int* pointer to the int where will be stored the number of devices found
 *\return A XIDeviceInfo array containing the devices found
 */
XIDeviceInfo * find_device_id(Display *dpy, char *name, int *nbid);

/**
 * Link a slave device to a master device
 *\param dpy Display* pointer to the display
 *\param slave XIDeviceInfo the slave device to link
 *\param master XIDeviceInfo the master device to be linked to
 *\return 0 if no error, -1 else
 */
int link_devices(Display *dpy, XIDeviceInfo slave, XIDeviceInfo master);

#ifdef __cplusplus
 } /* extern C */
#endif

#endif
