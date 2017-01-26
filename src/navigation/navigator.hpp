#ifndef NAVIGATOR_HPP_
#define NAVIGATOR_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <python2.7/Python.h>
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
    /**
     * Navigator copy constructor
     * \brief informs that copy constructor was called
     *
     * It's good to keep an eye on this to know if we're handelling a deep or a
     * shallow copy.
     */
    Navigator(const Navigator& obj){std::cout << "Navigator Copy constructor" << std::endl;}
    /**
     * Navigator assignment
     * \brief informs that assignment was called
     */
    Navigator& operator=(const Navigator& obj){std::cout << "Navigator assignment " << std::endl;}
    /** Navigator class destructor
     * \brief Closes python connexion
     */
		~Navigator();
		/** Function to call python function with args and return
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
    /** Function to call python function with no args
     * \fn std::string call_python_function_nargs(std::string function)
     * \param function String the name of the function
     * \return String Result of python function
     * \brief Equivalent of call_python_function but don't pass argument
     */
    std::string call_python_function_nargs(std::string function);
    /** Function to call python function with no args and no return
     * \fn void call_python_function_void_nargs(std::string function)
     * \param function String the name of the function
     * \brief Equivalent of call_python_function_nargs but don't get return
     */
    void call_python_function_void_nargs(std::string function);
    /** Function to perform a keyword search on default search engine of the browser
     * \fn std::string write_search(std::string keyword)
     * \param keyword String the keyword to browse
     * \return String the url after the search
     */
    std::string write_search(std::string keyword);
    /** Function to get html of a page
    * \fn std::string get_body_html()
    * \return String the raw html of the current page stripped of Styles and Scripts
    * \brief Calls call_python_function_nargs with get_body_html as function parametter
    */
		std::string get_body_html();
		/** Function to change page of the browser
		 * \fn std::string navigate(std::string url)
		 * \param url the url to navigate
		 * \return String current url of the browser, avoid redirection problems
		 * \brief Calls call_python_function with navigate as function parametter
		 */
		std::string navigate(std::string url);
    /** Function to end python properly
     * \fn void start_driver();
     * \brief Calls call_python_function_void_nargs with end_python as function parametter
     *
     * An "Homemade" destructor to end Webdriver and Uinput which are not destroyed
     * dynamically after the end of python context and still exist in the RAM
     */
    void end_python();
		/** Function to get HyperLinks from raw html
		 * \fn select_hyperlinks_from_html(std::string html,std::vector<HyperLink> &links)
		 * \param html Raw html
		 * \param links Vector to put HyperLinks once found
		 * \brief Get href and text for each <a> tag, and put them in an HyperLink
		 */
    void select_hyperlinks_from_html(std::string html, std::vector<HyperLink> &links);
		void select_hyperlinks_from_html(std::string html, std::vector<HyperLink> &links, std::vector<std::string> rubbish);
    int parse_tag_a(HyperLink &lk,std::string &tag_a);
	private:
    	PyObject *module;
};

#endif
