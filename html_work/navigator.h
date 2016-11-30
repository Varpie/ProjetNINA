#ifndef NAVIGATOR_H_
#define NAVIGATOR_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <python2.7/Python.h>
#include <list>

class HyperLink {
  public:
    std::string text;
    std::string url;
};

std::string get_bodyhtml_from_url(std::string url);
void select_hyperlinks_from_html(std::string html,std::list<HyperLink> &links);
#endif
