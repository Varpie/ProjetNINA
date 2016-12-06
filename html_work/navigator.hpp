#ifndef NAVIGATOR_HPP_
#define NAVIGATOR_HPP_

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
		std::string call_python_function(std::string function,std::string arg);
		std::string get_body_html(std::string url);
		std::string navigate(std::string url);
		void select_hyperlinks_from_html(std::string html,std::vector<HyperLink> &links);
	private:
    	PyObject *module;
};

#endif
