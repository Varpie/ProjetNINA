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
#include "../log.hpp"

/**
 * HyperLink Class, a class to gather text and url of a <a> tag
 * \class HyperLink Navigator.hpp
 */
class HyperLink
{
  public:
    std::string text;
    std::string url;
};

/**
 * Navigator Class, ensure navigation connexion
 * \class Navigator Navigator.hpp
 */
class Navigator
{
	public:
		/** Navigator class constructor
		 * \brief Open python connexion
		 */
		Navigator();
		/** Navigator class destructor
		 * \brief Closes python connexion
		 */
		~Navigator();
		/** Function to call python code
		 * \fn std::string call_python_function(std::string function,std::string arg)
		 * \param function String the name of the function
		 * \param arg String the argument passed to the function
		 * \return String Result of python function
		 *
		 * A function that parse parameters into py_objects type
		 * and call the function with it thank to python connexion opened
		 * in constructor
		 */
		std::string call_python_function(std::string function,std::string arg);
		/** Function to get html of a page
		 * \fn std::string get_body_html(std::string url)
		 * \param url the url to get page
		 * \return String the raw html of the page stripped of Styles and Scripts
		 * \brief Calls call_python_function with get_body_html in function parametter
		 */
		std::string get_body_html();
		/** Function to change page of the browser
		 * \fn std::string navigate(std::string url)
		 * \param url the url to navigate
		 * \return String current url of the browser, avoid redirection problems
		 * \brief Calls call_python_function with navigate in function parametter
		 */
		std::string navigate(std::string url);
		/** Function to get HyperLinks from raw html
		 * \fn select_hyperlinks_from_html(std::string html,std::vector<HyperLink> &links)
		 * \param html Raw html
		 * \param links Vector to put HyperLinks once found
		 * \brief Get href and text for each <a> tag, and put them in an HyperLink
		 */
		void select_hyperlinks_from_html(std::string html,std::vector<HyperLink> &links);
	private:
    	PyObject *module;
};

#endif
