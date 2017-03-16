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

  friend bool operator==(const HyperLink &h1, const HyperLink &h2) {
    return ((h1.text == h2.text) && (h1.url == h2.url));
  }
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
    Navigator(const Navigator& obj){logging::vout(1,"Navigator Copy constructor");}
    /**
     * Navigator assignment
     * \brief informs that assignment was called
     */
    Navigator& operator=(const Navigator& obj){logging::vout(1,"Navigator assignment");}
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
	std::string call_python_function(std::string function, std::string arg);
    /** Function to call python function with no args
     * \fn std::string call_python_function_nargs(std::string function)
     * \param function String the name of the function
     * \return String Result of python function
     * \brief Equivalent of call_python_function but don't pass argument
     */
    std::string call_python_function_nargs(std::string function);
    /** Function to call python function with no args
     * \fn int call_python_function_nargs_i(std::string function)
     * \param function String the name of the function
     * \return int Result of python function
     * \brief Equivalent of call_python_function_nargs, but returns int
     */
    int call_python_function_nargs_i(std::string function);
    /** Function to call python function with no args and no return
     * \fn void call_python_function_void_nargs(std::string function)
     * \param function String the name of the function
     * \brief Equivalent of call_python_function_nargs but don't get return
     */
    void call_python_function_void_nargs(std::string function);
    /** Function to call python function with passed args and no return
     * \fn void call_python_function_void_args(std::string function, std::string arg)
     * \param function String the name of the function
     * \param std::string arg the argument passed
     * \brief Equivalent of call_python_function but don't get return
     */
    void call_python_function_void_args(std::string function, std::string arg);
    /** Function to call python function with passed args and no return
     * \fn void call_python_function_void_args(std::string function, int arg)
     * \param function String the name of the function
     * \param int arg the argument passed
     * \brief Equivalent of call_python_function but don't get return
     */
    void call_python_function_void_args(std::string function, int arg);
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
	/** Function to get navigator's PID
     * \fn int get_pid()
     * \return int driver pid
     * \brief Calls call_python_function_void_nargs
     * Get driver's pid attribute.
     */
    int get_pid();
    /** Function to set python verbose levels
     * \fn void define_verbose(int level)
     * \brief Calls call_python_function_void_args with get_pid as function parameter 
     *
     * Shut all outputs if logging::verbose below 1
     * Allows errors if above 1
     * Allows prints if above 2
     */
    void define_verbose();
    /** Function to get HyperLinks from raw html
     * \fn select_hyperlinks_from_html(std::string html,std::vector<HyperLink> &links)
     * \param html std::string Raw html
     * \param links Vector to put HyperLinks once found
     * \brief Get href and text for each <a> tag, and put them in an HyperLink
     */
    void select_hyperlinks_from_html(std::string html, std::vector<HyperLink> &links);
    /** select_hyperlinks_from_html slightly modified to include additional processes
     * \fn void select_hyperlinks_from_html(std::string html, std::vector<HyperLink> &links, std::vector<std::string> rubbish)
     * \param html Raw html
	 * \param links Vector to put HyperLinks once found
     * \param rubbish std::vector<std::string> list of rubbish links
     * Include rubbish_links in it's selection process
     */
	void select_hyperlinks_from_html(std::string html, std::vector<HyperLink> &links, std::vector<std::string> rubbish);
    /** Parses a raw <a> html tag to fit HyperLink class pattern
     * \fn bool parse_tag_a(HyperLink &lk,std::string &tag_a)
     * \param lk HyperLink link to be filled with link url & text
     * \param tag_a std;;string raw <a> html tag
     * \return True if link is good, False otherwise
     * \brief Take href and text between tag and instanciate lk with it
     */
    bool parse_tag_a(HyperLink &lk,std::string &tag_a);
	private:
    /**
     * Python module that handle selenium api
     * Foundable at /usr/local/lib/python2.7/dist-packages/main.py
     */
    PyObject *module;
};
#endif
