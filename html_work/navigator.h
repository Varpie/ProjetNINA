#ifndef NAVIGATOR_H_
#define NAVIGATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <python2.7/Python.h>

typedef struct HyperLink{
    std::string text;
    std::string url;
} HyperLink;

std::string get_bodyhtml_from_url(std::string url);
int select_hyperlinks_from_html(std::string html,struct HyperLink *links);

#endif
