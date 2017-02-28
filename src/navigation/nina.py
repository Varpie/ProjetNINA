#!/usr/bin/env python2
# -*- coding: utf-8 -*-
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

def end_python():
    """!
    Close driver and destroy virtual keyboad reference when navigation is over
    """
    uinput_wrapping_module.destroy_uinput_device_func()
    driver.quit()

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
        uinput_wrapping_module.send_key_with_ctrl_func(44,29);
        #print "=py=== Tab closed ! ===py="
        return True
    else:
        return False

def navigate(var_url):
    """!
    Thread nav() function and kill it if it takes more than 40 seconds.

    @type var_url: string
    @param var_url: parameter for nav() function

    @return returns nav result OR 'failed' if killed
    @rtype string

    ! 40 replaced by 15 for test purpose
    """
    if(handle_frames()):
        driver.back()
        return "failed"
    ret = runFunctionWithTimeout(nav, (var_url,), timeout_duration=15)
    if(ret is not None):
        if(len(ret) != 0):
            return ret
        else:
            return "failed"
    else:
        return "failed"

def nav(var_url):
    """!
    Browse to the indicated page

    @type var_url: string
    @param var_url: Destination url to browse

    @return returns current url (to avoid redirections errors)
    OR returns failed if url wasn't valid
    @rtype string
    """
    # wait at least 0.5s
    driver.implicitly_wait(0.5)
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

def runFunctionWithTimeout(func, args=(), kwargs={}, timeout_duration=10, default=None):
    """!
    Thread a function and kill it if execution time exceed timeout_duration

    @type func: function
    @param func: Function to be executed

    @type timeout_duration: int
    @param timeout_duration: Number of seconds given to the function

    @return returns function result OR exception raised by it
    @rtype mixed

    """
    class InterruptableThread(threading.Thread):
        def __init__(self):
            threading.Thread.__init__(self)
            self.result = default
        def run(self):
            self.result = runFunctionCatchExceptions(func, *args, **kwargs)
    it = InterruptableThread()
    it.start()
    it.join(timeout_duration)
    if it.isAlive():
        return default
    if it.result[0] == "exception":
        raise it.result[1]
    return it.result[1]

def runFunctionCatchExceptions(func, *args, **kwargs):
    """!
    Exception raiser for runFunctionWithTimeout function

    @type func: function
    @param func: Function to be executed

    @return returns function result OR exception raised by it
    @rtype mixed
    """
    try:
        result = func(*args, **kwargs)
    except Exception, message:
        return ["exception", message]
    return ["RESULT", result]