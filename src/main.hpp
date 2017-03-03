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

std::string MOUSEFILE = "/dev/input/mice";
int logging::verbose = 0;
bool dict::whitelist = false;
std::string dict::whitefile;
bool dict::blacklist = false;
std::string dict::blackfile;
bool dict::other = false;
std::string dict::otherfile;
long timer;
bool countdown::links = false;
long countdown::number;
std::string url = "http://www.wikipedia.org/wiki/Special:Random";
std::atomic<bool> threading::running(true);
std::string config_path;

void print_help();
void parse_config();
void daemonize();
void stop_daemon();
void stopping_detection();
void timeout(long timer);
void handle_sigquit(int signum);

#endif
