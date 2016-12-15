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

/* Globals */
static int uinp_fd = -1;
static int box_muller_v = -1;
static const char name_conf[] = "conf";

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
void send_click_events();

/*Send an event of value = 1 and type = EV_KEY (press of key) */
void press_a_button(int key);

/*Send an event of value =  and type = EV_KEY (release of key) */
void release_a_button(int key);

/*
 * Send press event then release event of key (passed as an ascii character)
 * Possibility to add a modifier (shift, for example)
 */
void send_a_button(int key, int modifier);

/* send press event then release event of a key */
void send_a_button_default(int key);

/* Convert ASCII code to KEY defined in /linux/input.h */
void cvrt_char(int *a, int c);

/* write a character using the Uinput device */
void write_char(char c);

/* write an array using the Uinput device */
void write_array(char array[], int size);

/*
 * Load the conf file containing time.
 * These delays are put between each stoke to make the typing looks more normal
 */
void load_random(double stat[]);
/*
 * Return an independent random number following normal distribution
 * It uses the box muller approximation.
 * TODO : benchmark vs Marsaglia Polar Approximation
 * mean : mean of the normal distribution (N(mean, sig))
 * sig : standrad deviation of the normal distribution (N(mean, sig))
 */
double box_muller(double mean, double sig);
/*
 * Destroy uinput device
 * Return -1 if error, 0 if ok
 */
int destroy_uinput_device();

#ifdef __cplusplus
 } /* extern C */
#endif

#endif