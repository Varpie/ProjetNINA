#ifndef WRITE_KB_
#define WRITE_KB_

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

/* Globals */
static int uinp_fd = -1;

struct uinput_user_dev uinp; /* uInput device structure */
struct input_event event; /* Input device structure */

/* Setup the uinput device */
int setup_uinput_device();

void send_click_events();

void press_a_button(int key);

void release_a_button(int key);

void send_a_button(int key, int modifier);

void send_a_button_default(int key);

void cvrtChar(int *a, int c);

void writeChar(char c);

void writeArray(char array[], int size, int r[]);

void loadRandom(int rnd[]);

#endif
