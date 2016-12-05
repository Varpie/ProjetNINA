#ifndef GET_TIMED_KEY_
#define GET_TIME_KEY_

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

void writeConfFile(double m, double sig);
double esperance(double t[]);
double std_dev(double t[]);
void clean_stdin();

#endif
