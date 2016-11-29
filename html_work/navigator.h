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
    void setText(std::string txt);
    void setUrl(std::string url);
    std::string getText();
    std::string getUrl();

  private:
    std::string text;
    std::string url;
};
void HyperLink::setText(std::string txt) {
   text = txt;
}
std::string HyperLink::getText() {
   return text;
}
void HyperLink::setUrl(std::string url) {
   url = url;
}
std::string HyperLink::getUrl() {
   return url;
}




std::string get_bodyhtml_from_url(std::string url);
int select_hyperlinks_from_html(std::string html,std::list<HyperLink> links);

#endif
