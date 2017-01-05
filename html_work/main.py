# -*- coding: utf-8 -*-
from selenium import webdriver
import random
import lxml.html.clean
import time
import signal

driver = webdriver.Firefox()


def write_search(keyworkd):
	pass

def close_driver(v):
	"""
	Close driver when navigation is over

	@type v: string
	@param v: Unused

	@rtype: string
	@return: returns unused
	"""
	driver.quit()
	# print "end"
	return "True"

def get_body_html(v):
	"""
	Get body html of a page, and strips it from its Styles and Scripts
	It does so, with webdriver.
	Cleaning it does with lxml.html.clean dependance

	@type v: string
	@param v: Unused

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
		return get_body_html("")

def nav(var_url):
	"""
	Browse to indicated page

	@type var_url: string
	@param var_url: Destination url to browse

	@rtype: string
	@return: returns current url (to avoid redirections errors)
	| returns failed if url wasn't valid
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
			print e
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



def runFunctionCatchExceptions(func, *args, **kwargs):
    try:
        result = func(*args, **kwargs)
    except Exception, message:
        return ["exception", message]

    return ["RESULT", result]


def runFunctionWithTimeout(func, args=(), kwargs={}, timeout_duration=10, default=None):
    import threading
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

def navigate(var_url):
	ret = runFunctionWithTimeout(nav, (var_url,), timeout_duration=40)
	if(len(ret) != 0):
		return ret
	else:
		return "failed"
