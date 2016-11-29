# -*- coding: utf-8 -*-
from selenium import webdriver
import random
import lxml.html.clean

def get_html_from_url(var_url):
    driver = webdriver.Firefox()
    driver.get(var_url)
    html = driver.page_source
    cleaner = lxml.html.clean.Cleaner()
    cleaner.javascript = True
    cleaner.style = True
    return cleaner.clean_html(html).encode('utf-8')
