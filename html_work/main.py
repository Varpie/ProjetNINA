# -*- coding: utf-8 -*-
from selenium import webdriver
import random
import lxml.html.clean
import time

driver = webdriver.Firefox()

def get_body_html(var_url):
	"""
	Get body html of a page, and strips it from its Styles and Scripts
	It does so, with webdriver.
	Cleaning it does with lxml.html.clean dependance

	@type var_url: string
	@param var_url: The url where to get html

	@rtype: string
	@return: returns html
	"""
	driver.get(var_url)
	links = driver.find_elements_by_tag_name("a")
	if(len(links) < 1):
		driver.back()
	html = driver.page_source
	cleaner = lxml.html.clean.Cleaner()
	cleaner.javascript = True
	cleaner.style = True
	return cleaner.clean_html(html).encode('utf-8')

def navigate(var_url):
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
	print("current :" + current);
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
	# tag url
	elif(var_url[:1] == '#'):
		css_selector = "a[href*='"+var_url+"']"
		try:
			element = driver.find_element_by_css_selector(css_selector)
			if(element.is_displayed()):
				element.click()
			else:
				driver.get(var_url)
		except:
			if(current.find('#') == -1 and var_url.len() != 1):
				var_url = current[:-1] + var_url
			else:
				#print "failed"
				return "failed"
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
				#print("sortie : "+var_url) #debug
				driver.get(var_url)
		# if not we get it
		except:
			#print("sortie : "+var_url) #debug
			driver.get(var_url)
	# invalid url
	else:
		# we return failed to get another rand from C++
		#print "failed" #debug
		return "failed"
	#we return current url to keep navigate
	return driver.current_url
