#ifndef MAIN_H_
#define MAIN_H_

#include <python2.7/Python.h>
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
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

/**
 * Mouse event file path
 */
std::string MOUSEFILE = "/dev/input/mice";
/**
 * Initialize verbose level at 0
 */
int logging::verbose = 0;
/**
 * Initialize whitelist utilisation to false
 */
bool dict::whitelist = false;
/**
 * Whitelist file path
 */
std::string dict::whitefile;
/**
 * Initialize blacklist utilisation to false
 */
bool dict::blacklist = false;
/**
 * Blacklist file path
 */
std::string dict::blackfile;
/**
 * Initialize otherlist utilisation to false
 */
bool dict::other = false;
/**
 * Otherlist file path
 */
std::string dict::otherfile;
/**
 * Execution time in seconds
 */
long timer;
/**
 * Links countdown utilisation set to false
 */
bool countdown::links = false;
/**
 * Number of links browsed
 */
long countdown::number;
/**
 * Start url
 */
std::string url = "http://www.wikipedia.org/wiki/Special:Random";
/**
 * Nina's runnig status
 */
std::atomic<bool> threading::running(true);
/**
 * Config file path
 */
std::string config_path;

/** Function that print nina's help
 * \fn void print_help()
 */
void print_help();
/** Function that parse config file
 * \fn void parse_config()
 */
void parse_config();
/** Function that detach nina from the terminal
 * \fn void daemonize()
 */
void daemonize();
/** Function that stop nina from kernel module
 * \fn void stop_daemon()
 */
void stop_daemon();
/** Function that stop nina with mouse position
 * \fn void stopping_detection()
 */
void stopping_detection();
/** Function that stop nina when timer reach 0
 * \fn void timeout()
 * \param timer long Execution time in seconds
 */
void timeout(long timer);
/** Function that stop nina when a sigquit is handle
 * \fn void handle_sigquit()
 */
void handle_sigquit(int signum);

#endif
