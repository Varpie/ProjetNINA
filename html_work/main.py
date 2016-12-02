# -*- coding: utf-8 -*-
from selenium import webdriver

import random
import lxml.html.clean

driver = webdriver.Firefox()
def get_html_from_url(var_url):
    driver.get(var_url)
    html = driver.page_source
    cleaner = lxml.html.clean.Cleaner()
    cleaner.javascript = True
    cleaner.style = True
    return cleaner.clean_html(html).encode('utf-8')

def navigate(var_url):
	if(var_url[:1] == '#' or var_url[:4] == 'http'):
		css_selector = "a[href*='"+var_url+"']"
		driver.find_element_by_css_selector(css_selector).click()
		return driver.current_url
	elif(var_url != ""):
		url = driver.current_url
		pos = url.find('/',9)
		domain = url[:pos+1]
		valid_url = domain+var_url
		driver.get(valid_url)
		return driver.current_url