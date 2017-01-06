#ifndef INTELLIGENCE_HPP_
#define INTELLIGENCE_HPP_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <python2.7/Python.h>
#include <vector>
#include "navigator.hpp"
#include "../log.hpp"

typedef std::vector<tuple<int,std::string>> tuple_list;

/**
 * Intelligence Class, regroup navigation processes
 * \class Intelligence Intelligence.hpp
 */
class Intelligence
{
	public:
		/** Intelligence class constructor
		 * \param start_url a String that define start_url of browse
		 */

		Intelligence(std::string &start_url);
		Intelligence(const Intelligence& obj){std::cout << "Intelligence Copy constructor" << std::endl;}
		Intelligence& operator=(const Intelligence& obj){std::cout << "Intelligence assignment " << std::endl;}

		/** Intelligence class destructor
		 */
		~Intelligence();
		/** Roam function, just roam randomly on internet
		 * \fn void roam(void)
		 */
		void roam(void);
		void load_blacklist();
		void dump_blacklist();
	private:
		std::string current_url;
		Navigator* navigator;
		std::vector<std::string> blacklist;
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

/** Function that selects a random in a vector matching with whitelist
 * \fn HyperLink select_whitelist(std::vector<HyperLink> &links,std::string url)
 * \param links Vector of HyperLinks
 * \param url Url to compare
 * \return an HyperLink matching with whitelist
 */
 HyperLink select_whitelist(std::vector<HyperLink> &links,std::string url, std::vector<std::string> whitelist);

 std::vector<std::string> init_whitelist(std::string name);

 HyperLink select_blacklist(std::vector<HyperLink> &links,std::string url, std::vector<std::string> blacklist);

 std::vector<std::string> init_blacklist(std::string name);

void add_to_blacklist(std::string wrong_url);

#endif
