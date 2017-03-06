#ifndef GET_TIMED_KEY_
#define GET_TIME_KEY_

#ifdef __cplusplus
extern "C" {
#endif

#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

static const char test_text[] = "this is a simple test to get your typing speed";
static const int size_text = 46;
static const double time_limit = 2000000.0;
static const double base_time = 100000.0;
static const int base_n = 1;
static const char *delimiter = ";";
static const char name_conf[] = "/etc/nina/conf";
#define SIZE_MAP 256

typedef struct act_mean{
  double mean;
  int n;
}act_mean;
/*
static act_mean **map = (act_mean **)calloc(256,sizeof(act_mean*));
int i =0;
for(i = 0; i < 256; i++)
    map[i] = (act_mean*)calloc(256,sizeof(act_mean));
*/
extern act_mean map[SIZE_MAP][SIZE_MAP];
/**
 * Write the two parameters in a file named conf in the root of the program
 * \param m double
 * \param
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
void ask_keystrokes();
/**
 * Load the conf file and store it in the map
 */
void load_map();
/**
 * Write the map into the conf file
 */
void update_mapconf();
/**
 * Create a conf file from the standard map (mean and n define by base_time and base_n)
 */
void create_mapconf();
/**
 * Listen the input of /dev/event0 during @timer seconds and store the difference
 * bewteen the time of the previous key and the time of the actual key pressed
 * in a map
 *\param time int number of seconds the program will listen to /dev/event0
 */
void keystroke_time(int time);
#ifdef __cplusplus
 } /* extern C */
#endif

#endif
