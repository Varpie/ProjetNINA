#ifndef MAIN_H_
#define MAIN_H_

#include <python2.7/Python.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include "log.hpp"
#include "navigation/intelligence.hpp"
#include "ctrl_keyboard/timed_keystrokes.h"
#include "ctrl_keyboard/write_keyboard.h"

#include <linux/input.h>
#include <fcntl.h>
#include <X11/Xlib.h>
#include <signal.h>
#include <thread>
#include <atomic>
#define MOUSEFILE "/dev/input/mice"

void print_help();
void parse_config();
void daemonize();
void stopping_detection();
void handle_sigquit(int signum);

#endif
