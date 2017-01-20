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
#include <unistd.h>
#include <time.h>
#include <math.h>

static const char testText[] = "this is a simple test to get your typing speed";
static const int sizeText = 46;

struct act_mean{
  double mean;
  int n;
}

/*
 * Write the two parameters in a file named conf in the root of the program
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
/*
 * Clean the buffer
 */
void clean_stdin();
/*
 * Ask the user to type a phrase. Log the time of the input and calculate
 * the difference between each time
 */
void ask_keystrokes();

#ifdef __cplusplus
 } /* extern C */
#endif

#endif
