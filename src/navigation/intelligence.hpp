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
#include <thread>
#include "navigator.hpp"
#include "../lib.hpp"

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
		/**
		 * Intelligence copy constructor
		 * \brief informs that copy constructor was called
		 *
		 * It's good to keep an eye on this to know if we're handelling a deep or a
		 * shallow copy.
		 */
		Intelligence(const Intelligence& obj){logging::vout(1,"Intelligence Copy constructor");}
		/**
		 * Intelligence assignment
		 * \brief informs that assignment was called
		 */
		Intelligence& operator=(const Intelligence& obj){logging::vout(1,"Intelligence assignment");}
		/** Intelligence class destructor
		 */
		~Intelligence();
		/** Roam function. Actual processing that roam on internet
		 * \fn void roam(void)
		 */
		void roam(void);
		/** Function that load all class lists such as blacklist & whitelists
		 * \fn void load_lists()
		 * Called in class constructor
		 */
		void load_lists();
		/** Counts the number of times AI passed by the current domain
		 * \fn int current_domain_occurences()
		 * \return int number of occurences
		 * Seek in history list and counts. Allows us to change domain if
		 * we passed too many times
		 */
		int current_domain_occurences();
		/** Function that dump autolists in text files
		 * \fn void dump_lists();
		 * Called in class destructor
		 */
		void dump_lists();
		/** Function that select a link with all the parameters
		 * \fn HyperLink select_link(std::vector<HyperLink> &links,std::string url)
		 * \param links std::vector<HyperLink> List of hyperLink to select one
		 * \param url string url Except this url in selection
		 * \return HyperLink link selected
		 */
		HyperLink select_link(std::vector<HyperLink> &links,std::string url);
		/** Function that test a link with all needed parameters (blacklist,whitelist...)
		 * \fn bool test_link(HyperLink &link,std::string &url)
		 * \return bool True : link valid, False : link not valid
		 * Called in select_link
		 */
		bool test_link(HyperLink &link,std::string &url);
		/** Google search a keyword
		 * \fn void search_keyword()
		 * Go on urltab, type keyword calling uinput and enter search
		 * Usually called when error case met
		 */
		std::string search_keyword();
		// HyperLink select_otherlist(std::vector<HyperLink> &links,std::string url, tuple_list list);
		/**
		 * Navigator object, liaison between C++ and Python
		 */
		Navigator* navigator;
	private:
		/**
		 * current url of the browser
		 */
		std::string current_url;
		/**
		 * user blacklist, configurable
		 */
		std::vector<std::string> blacklist;
		/**
		 * user whitelist, configurable
		 */
		std::vector<std::string> whitelist;
		/**
		 * keywords to search, configurable
		 */
		std::vector<std::string> keywords;
		/**
		 * history of last 50 visited pages
		 */
		std::vector<std::string> history;
		/**
		 * blacklist of navigation,
		 * filled when error case met, limited to 2500
		 */
		std::vector<std::string> auto_blacklist;
		/**
		 * fix rubbish links,
		 * links to not select in a google search
		 */
		std::vector<std::string> rubbish_links;
		/**
		 * user list, configurable
		 * with keyword's weight
		 */
		std::vector<std::tuple<int, std::string>> otherlist;
		/**
		 * Max size of blacklist vector
		 */
		int const static AUTO_BL_MAX = 2500;
		/**
		 * Max size of history vector
		 */
		int const static HISTORY_MAX = 50;
};
/** Function to select a random string in vector
 * \fn std::string select_keyword(std::vector<std::string> list)
 * \param list std::vector<std::string> list to select
 * \return std::string string selected
 */
std::string select_keyword(std::vector<std::string> list);
/** Function to select a random in a vector
 * \fn HyperLink select_random_in_vector(std::vector<HyperLink> &links)
 * \param links Vector of HyperLinks
 * \return an HyperLink selected randomly
 */
HyperLink select_random_in_vector(std::vector<HyperLink> &links);
/** Init a list from a text file
 * \fn std::vector<std::string> init_list(std::string name)
 * \param path std::string path to source file
 * \return std::vector<std::string> list initialized
 * Used to init Intelligence member lists in init_lists function
 */
std::vector<std::string> init_list(std::string path);
/** Init the otherlist from a text file
 * \fn std::vector<std::tuple<int, std::string>> init_otherlist(std::string name);
 * \param path std::string path to source file
 * \return std::vector<std::tuple<int, std::string>> list initialized
 * Used to init Intelligence otherlist in init_otherlist function
 */
std::vector<std::tuple<int, std::string>> init_otherlist(std::string name);
/** Used to fill size limited vectors such as history
 * \fn void append_vector(std::vector<std::string> &list,std::string param,int limit)
 * \param list std::vector<std::string> Vector to fill
 * \param param std::string Param to push back in list
 * \param limit int limit number
 * If limit exeeded, we remove first index of the list
 */
void append_vector(std::vector<std::string> &list,std::string param,int limit);
#endif
