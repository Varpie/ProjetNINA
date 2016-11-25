#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <python2.7/Python.h>

void print_help();
void parse_config();

typedef struct HyperLink{
    char text[512];
    char url[2048];
} HyperLink;

char * get_bodyhtml_from_url(char *url);
int select_hyperlinks_from_html(char *html,struct HyperLink *links);

#endif
