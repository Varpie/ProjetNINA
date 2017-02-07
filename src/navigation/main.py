# -*- coding: utf-8 -*-
from selenium import webdriver
import uinput_wrapping_module
import threading
import random
import lxml.html.clean
import time
import signal

""" init """
driver = webdriver.Firefox()
uinput_wrapping_module.setup_uinput_device_func()

def handle_frames():
    if(len(driver.window_handles) > 1):
        for handle in driver.window_handles:
            print handle
#driver.switch_to_window(handle)

def end_python():
    """
    Close driver and destroy virtual keyboad when navigation is over
    """
    uinput_wrapping_module.destroy_uinput_device_func()
    driver.quit()

def get_body_html():
    """
    Get body html of a page, and strips it from its Styles and Scripts
    It does so, with webdriver.
    Cleaning it does with lxml.html.clean dependance

    @rtype: string
    @return: returns html
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
    """
    Focus to urlbar of the browser and types a keyword

    @type keyword: string
    @param keyword: Keyword to be searched

    @rtype: string
    @return: returns current url
    """
    # f6 -> 64 | entrée -> 28
    # driver.get("https://www.google.com")
    # inputElement = driver.find_element_by_name("q")
    # inputElement.click();
    # TODO : Choose between google search or taburl search
    uinput_wrapping_module.send_a_button_default_func(64)
    uinput_wrapping_module.write_array_func(keyword)
    uinput_wrapping_module.send_a_button_default_func(28)
    time.sleep(3.5)
    return driver.current_url

def navigate(var_url):
    """
    Thread nav() function and kill it if it takes more than 40 seconds.

    @type var_url: string
    @param var_url: parameter for nav() function

    @rtype: string
    @return: returns nav result OR 'failed' if killed

    ! 40 replaced by 15 for test purpose
    """
    handle_frames()
    ret = runFunctionWithTimeout(nav, (var_url,), timeout_duration=15)
    if(ret is not None):
        if(len(ret) != 0):
            return ret
        else:
            return "failed"
    else:
        return "failed"

def nav(var_url):
    """
    Browse to the indicated page

    @type var_url: string
    @param var_url: Destination url to browse

    @rtype: string
    @return: returns current url (to avoid redirections errors)
    OR returns failed if url wasn't valid
    """
    #print("entered") #debug
    #print("entree : "+var_url) #debug
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
            #print "sortie, js : "+var_url #debug
            driver.execute_script("document.body.querySelector('a[href=\""+var_url+"\"]').click()");
            return driver.current_url
        except Exception, e:
            #print e
            print " "
    # absolute url
    if(var_url[:4] == "http"):
        css_selector = "a[href*='"+var_url+"']"
        try:
            # we try to find element with link
            element = driver.find_element_by_css_selector(css_selector)
            if(element.is_displayed()):
            # if findable and displayed we click on it
                #print "sortie : clicked http"
                element.click()
            else:
                #print("sortie, get 2 : "+var_url) #debug
                driver.get(var_url)
        # if not we get it
        except:
            #print "sortie, get 3 : "+var_url #debug
            driver.get(var_url)
    # invalid url
    else:
        # we return failed to get another rand from C++
        #print "sortie failed" #debug
        return "failed"
    #we return current url to keep navigate
    return driver.current_url

def runFunctionWithTimeout(func, args=(), kwargs={}, timeout_duration=10, default=None):
    """
    Thread a function and kill it if execution time exceed timeout_duration

    @type func: function
    @param func: Function to be executed

    @type timeout_duration: int
    @param timeout_duration: Number of seconds given to the function

    @rtype: mixed
    @return: returns function result OR exception raised by it

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
    """
    Exception raiser for runFunctionWithTimeout function

    @type func: function
    @param func: Function to be executed

    @rtype: mixed
    @return: returns function result OR exception raised by it
    """
    try:
        result = func(*args, **kwargs)
    except Exception, message:
        return ["exception", message]
    return ["RESULT", result]
