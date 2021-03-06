#!/usr/bin/env python2
# -*- coding: utf-8 -*-
import sys
import os
from selenium import webdriver
import uinput_wrapping_module
import threading
import random
import lxml.html.clean
import time
import signal

"""!
Selenium webdriver
Marionette on Firefox browser
"""
driver = webdriver.Firefox()
"""!
Command that creates file descriptor for a user space keyboad
ref to Uinput kernel module
"""
uinput_wrapping_module.setup_uinput_device_func()
"""
We redirect verbose to /dev/null by default
Outputs will be redirected acordingly to verbose level
in define_verbose function
"""
sys.stdout = os.devnull
sys.stderr = os.devnull

def get_pid():
    """!
    @return return driver's pid
    @rtype int
    """
    return driver.service.process.pid

def end_python():
    """!
    Close driver and destroy virtual keyboad reference when navigation is over
    """
    uinput_wrapping_module.destroy_uinput_device_func()
    driver.quit()

def define_verbose(level):
    """!
    Define verbose level
    Accepts logging::verbose c++ variable content

    @type level: int
    @param level: Actual level of verbose
    """
    if(level > 0):
        sys.stderr = sys.__stderr__
    if(level > 1):
        sys.stdout = sys.__stdout__

def get_body_html():
    """!
    Get body html of a page, and strips it from its Styles and Scripts
    It does so, with webdriver.
    Cleaning it does with lxml.html.clean dependance

    @return returns html
    @rtype string
    """
    try:
        #get 1st link of page to test if there is at least one
        link = driver.find_element_by_tag_name("a")
        if not link:
            try:
                driver.back()
                return get_body_html()
            except:
                return "failed"
        else:
            html = driver.page_source
            cleaner = lxml.html.clean.Cleaner()
            cleaner.javascript = True
            cleaner.style = True
            return cleaner.clean_html(html).encode('utf-8')
    except:
        try:
            driver.back()
            return get_body_html()
        except:
            return "failed"

def write_search(keyword):
    """!
    Focus to urlbar of the browser and types a keyword

    @type keyword: string
    @param keyword: Keyword to be searched

    @return returns current url
    @rtype string
    """
    uinput_wrapping_module.send_key_func(64)
    uinput_wrapping_module.write_string_func(keyword)
    uinput_wrapping_module.send_key_func(28)
    time.sleep(3.5)
    return driver.current_url

def handle_frames():
    """!
    Close browser when number is >1

    @return True if tab closed, false if no more than 2 tabs active
    @rtype Boolean
    """
    if(len(driver.window_handles) > 1):
        uinput_wrapping_module.send_key_with_ctrl_func(44);
        print "=py=== Tab closed ! ===py="
        return True
    else:
        return False

# ============ tools =========== #
class TimeoutError(Exception): pass

def timelimit(timeout):
    def internal(function):
        def internal2(*args, **kw):
            class Calculator(threading.Thread):
                def __init__(self):
                    threading.Thread.__init__(self)
                    self.result = None
                    self.error = None
                def run(self):
                    try:
                        self.result = function(*args, **kw)
                    except:
                        self.error = sys.exc_info()[0]
            c = Calculator()
            c.start()
            c.join(timeout)
            if c.isAlive():
                raise TimeoutError
            if c.error:
                raise c.error
            return c.result
        return internal2
    return internal
# ============ end tools =========== #

def navigate(var_url):
    """!
    Intercepts navigate() function call, to handle TimeoutException

    @type var_url: string
    @param var_url: url to pass to nav()

    @return returns nav() result
    OR returns failed if TimeoutException triggered
    @rtype string
    """
    try: 
        return nav(var_url)
    except: 
        return "failed" 

@timelimit(20)
def nav(var_url):
    """!
    Browse to the indicated page

    @type var_url: string
    @param var_url: Destination url to browse

    @return returns current url (to avoid redirections errors)
    OR returns failed if url wasn't valid
    @rtype string
    """
    current = driver.current_url
    if(var_url[:2] == "//"):
        var_url = "http:" + var_url
    elif(var_url[:1] == '/'):
        pos = current.find('/',9)
        domain = current[:pos+1]
        var_url = domain + var_url[1:]
    elif var_url.find('#'):
        try:
            driver.execute_script("document.body.querySelector('a[href=\""+var_url+"\"]').click()");
            return driver.current_url
        except Exception, e:
            pass
            # print e
    if(var_url[:4] == "http"):
        css_selector = "a[href*='"+var_url+"']"
        try:
            element = driver.find_element_by_css_selector(css_selector)
            if(element.is_displayed()):
                element.click()
            else:
                driver.get(var_url)
        except:
            driver.get(var_url)
    else:
        return "failed"
    return driver.current_url