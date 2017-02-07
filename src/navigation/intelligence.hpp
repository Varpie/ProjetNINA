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
#include <tuple>
#include <algorithm>
#include "navigator.hpp"
#include "../log.hpp"

typedef std::vector<std::tuple<int,std::string>> tuple_list;

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
		/** Function that selects a random in a vector with a different url than the one passed
		 * \fn HyperLink select_diff_random_in_vector(std::vector<HyperLink> &links,std::string url)
		 * \param links Vector of HyperLinks
		 * \param url Url to compare
		 * \return an HyperLink with different url than the one passed
		 */
		void load_lists();
		int current_domain_occurences();
		void dump_lists();
		HyperLink select_link(std::vector<HyperLink> &links,std::string url);
		void search_keyword();
		bool test_link(HyperLink &link,std::string &url);
		// HyperLink select_otherlist(std::vector<HyperLink> &links,std::string url, tuple_list list);
	private:
		std::string current_url;
		Navigator* navigator;
		std::vector<std::string> blacklist;
		std::vector<std::string> whitelist;
		std::vector<std::string> keywords;
		std::vector<std::string> history;
		std::vector<std::string> auto_blacklist;
		std::vector<std::string> rubbish_links;
		tuple_list otherlist;
		int const static AUTO_BL_MAX = 2500;
		int const static HISTORY_MAX = 50;
};

std::string select_keyword(std::vector<std::string> list);
/** Function to select a random in a vector
 * \fn HyperLink select_random_in_vector(std::vector<HyperLink> &links)
 * \param links Vector of HyperLinks
 * \return an HyperLink selected randomly
 */
HyperLink select_random_in_vector(std::vector<HyperLink> &links);

/** Function that selects a random in a vector matching with whitelist
 * \fn HyperLink select_whitelist(std::vector<HyperLink> &links,std::string url)
 * \param links Vector of HyperLinks
 * \param url Url to compare
 * \return an HyperLink matching with whitelist
 */

std::vector<std::string> init_list(std::string name);

tuple_list init_otherlist(std::string name);

void add_to_blacklist(std::string wrong_url);

void append_vector(std::vector<std::string> &list,std::string param,int limit);

#endif
