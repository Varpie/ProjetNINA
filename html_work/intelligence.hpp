#ifndef INTELLIGENCE_HPP_
#define INTELLIGENCE_HPP_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <python2.7/Python.h>
#include <vector>
#include "navigator.hpp"

class Intelligence
{
	public:
		Intelligence(Navigator &nav,std::string &start_url);
		void roam(void);
	private:
		std::string current_url;
		Navigator navigator;
};

HyperLink select_random_in_vector(std::vector<HyperLink> &links);

#endif