# -*- coding: utf-8 -*-
from selenium import webdriver
import random
import lxml.html.clean
import time

driver = webdriver.Firefox()

def get_body_html(var_url):
	driver.get(var_url)
	html = driver.page_source
	cleaner = lxml.html.clean.Cleaner()
	cleaner.javascript = True
	cleaner.style = True
	return cleaner.clean_html(html).encode('utf-8')

def navigate(var_url):
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
	# tag url
	elif(var_url[:1] == '#'):
		# we add current url to it => absolute url
		# if we're not alrealy in case of relative at the end of current
		if(var_url.find('#') == -1):
			var_url = current[:-1] + var_url
		#otherwise we return "failed" to get another random from C++
		else:
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
		''' we return failed to get another rand from C++ '''
		#print("failed") #debug
		return "failed"
	#we return current url to keep navigate
	return current