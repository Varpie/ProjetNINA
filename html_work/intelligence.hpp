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


/** 
 * Intelligence Class, regroup navigation processes 
 * \class Intelligence Intelligence.hpp
 */
class Intelligence
{
	public:
		/** Intelligence class constructor
		 * \param nav a Navigator Object that ensure connexion with navigator functions
		 * \param start_url a String that define start_url of browse
		 */
		Intelligence(Navigator &nav,std::string &start_url);
		/** Intelligence class destructor
		 */
		~Intelligence();
		/** Roam function, just roam randomly on internet
		 * \fn void roam(void)
		 */
		void roam(void);
	private:
		std::string current_url;
		Navigator navigator;
};

/** Function to select a random in a vector
 * \fn HyperLink select_random_in_vector(std::vector<HyperLink> &links)
 * \param links Vector of HyperLinks
 * \return an HyperLink selected randomly
 */
HyperLink select_random_in_vector(std::vector<HyperLink> &links);

/** Function that selects a random in a vector with a different url than the one passed
 * \fn HyperLink select_diff_random_in_vector(std::vector<HyperLink> &links,std::string url)
 * \param links Vector of HyperLinks
 * \param url Url to compare
 * \return an HyperLink with different url than the one passed
 */
HyperLink select_diff_random_in_vector(std::vector<HyperLink> &links,std::string url);

#endif
