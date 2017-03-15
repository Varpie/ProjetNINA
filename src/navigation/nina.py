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

class TimeoutError(Exception): pass

def get_pid():
    """!
    @return return driver's pid
    @rtype int
    """
    return str(driver.service.process.pid)

def end_python():
    """!
    Close driver and destroy virtual keyboad reference when navigation is over
    """
    uinput_wrapping_module.destroy_uinput_device_func()
    driver.quit()

def get_body_html():
    """!drag

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
            driver.back()
        else:
            html = driver.page_source
            cleaner = lxml.html.clean.Cleaner()
            cleaner.javascript = True
            cleaner.style = True
            return cleaner.clean_html(html).encode('utf-8')
    except:
        driver.back()
        return get_body_html()

def write_search(keyword):
    """!
    Focus to urlbar of the browser and types a keyword

    @type keyword: string
    @param keyword: Keyword to be searched

    @return returns current url
    @rtype string
    """
    # f6 -> 64 | entrée -> 28
    # driver.get("https://www.google.com")
    # inputElement = driver.find_element_by_name("q")
    # inputElement.click();
    # TODO : Choose between google search or taburl search
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
        #print "=py=== Tab closed ! ===py="
        return True
    else:
        return False

def navigate(var_url):
    try:
        return nav(var_url)
    except Exception, e:
        print e
        return "failed"

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
    # current url, and domain
    current = driver.current_url
    # protocol relative url -> free to http or https
    if(var_url[:2] == "//"):
        # we change it to absolute
        var_url = "http:" + var_url
    #root relative url -> miss the domains
    elif(var_url[:1] == '/'):
        # we add it => absolute url
        pos = current.find('/',9)
        domain = current[:pos+1]
        var_url = domain + var_url[1:]
    # tag url or full url with tag at the end
    elif var_url.find('#'):
        try:
            # Execute javascript, and avoir driver to wait for DOM readyState event
            driver.execute_script("document.body.querySelector('a[href=\""+var_url+"\"]').click()");
            return driver.current_url
        except Exception, e:
            pass
            # print e
    # absolute url
    if(var_url[:4] == "http"):
        css_selector = "a[href*='"+var_url+"']"
        try:
            # we try to find element with link
            element = driver.find_element_by_css_selector(css_selector)
            if(element.is_displayed()):
            # if findable and displayed we click on it
                element.click()
            else:
                driver.get(var_url)
        # if not we get it
        except:
            driver.get(var_url)
    # invalid url
    else:
        # we return failed to get another rand from C++
        return "failed"
    #we return current url to keep navigate
    return driver.current_url

def define_verbose(level):
    if(int(level) > 0):
        sys.stderr = sys.__stderr__
    if(int(level) > 1):
        sys.stdout = sys.__stdout__