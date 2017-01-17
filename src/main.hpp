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
#include "html_work/intelligence.hpp"
#include "timed_keystrokes/timed_keystrokes.h"
#include "uinput/write_keyboard.h"

void print_help();
void parse_config();

#endif