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

void print_help();
void parse_config();

/*! \struct HyperLink main.h
    \brief A struct to contain tuples of html links and description.
*/
typedef struct HyperLink{
    char text[512];
    char url[2048];
} HyperLink;

/*! \fn char * get_bodyhtml_from_url(char *url)
    \brief Get source html from url.
    \param url The url to scrape.

    This functions calls a scrapy Spider
    By embedding Python
*/
char * get_bodyhtml_from_url(char *url);
/*! \fn int select_hyperlinks_from_html(char *html,struct HyperLink *links)
    \brief Create tuples of links and Desc from raw html
    \param html The source html.
    \param links Adress passed array of HyperLink struct.

    This function iterates over the whole html, and select
    the chars that should be put in the links array
*/
int select_hyperlinks_from_html(char *html,struct HyperLink *links);

#endif
