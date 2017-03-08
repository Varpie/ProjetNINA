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

#define MAX_SHIFT_LEVEL 3
#define SHIFT 42
#define ALT_GR 100
#define CTRL_KEY 29


/* Globals */
static int uinp_fd = -1;
static const char *DEVICE_NAME  = "Custom Device";

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
 * -1 if error, 0 if OK.
 */
int setup_uinput_device();

/* Implementation for mouse event */
void send_click_events(int x, int y);

/*Send an event of value = 1 and type = EV_KEY (press of key) */
void press_a_button(int key);

/*Send an event of value =  and type = EV_KEY (release of key) */
void release_a_button(int key);

/*
 * Send press event then release event of key (passed as an ascii character)
 * Possibility to add a modifier (shift, for example)
 */
//void send_a_button(int key, int modifier);

/* send press event then release event of a key */
//void send_a_button_default(int key);

/* Convert ASCII code to KEY defined in /linux/input.h */
//void cvrt_char(int *a, int c);

/* write a character using the Uinput device */
//void write_char(char c);

/* write an array using the Uinput device */
//void write_array(char array[], int size);

/*
 * Load the conf file containing time.
 * These delays are put between each stoke to make the typing looks more normal
 */
//void load_random(double stat[]);
/*
 * Return an independent random number following normal distribution
 * It uses the box muller approximation.
 * mean : mean of the normal distribution (N(mean, sig))
 * sig : standrad deviation of the normal distribution (N(mean, sig))
 */
//double box_muller(double mean, double sig);
/**
 * Convert a UTF-16 character to it's KeySym
 * \param ucs long UTF-16 character
 * \return KeySym corresponding KeySym
 */
KeySym ucs2keysym(long ucs);
/*
 * Destroy uinput device
 * Return -1 if error, 0 if ok
 */
int destroy_uinput_device();
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

#ifdef __cplusplus
 } /* extern C */
#endif

#endif
