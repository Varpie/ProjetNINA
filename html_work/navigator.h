#ifndef NAVIGATOR_H_
#define NAVIGATOR_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <python2.7/Python.h>
#include <vector>
#include <time.h>

class HyperLink 
{
  public:
    std::string text;
    std::string url;
};

class Navigator
{
	public:
		Navigator();
		~Navigator();
		std::string get_bodyhtml_from_url(std::string url);
		void select_hyperlinks_from_html(std::string html,std::vector<HyperLink> &links);
		std::string navigate(std::string url);
	private:
    	PyObject *module;
};

HyperLink select_random_in_vector(std::vector<HyperLink> &links);

#endif
