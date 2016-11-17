from selenium import webdriver
import random

driver = webdriver.Firefox()

driver.get("https://en.wikipedia.org/wiki/Special:Random")
links = driver.find_elements_by_tag_name("a")
dict_links = dict()
for link in links:
    dict_links[link.get_attribute("text")] = link.get_attribute("href")
driver.find_element_by_link_text(dict_links.keys()[random.randint(1,len(dict_links))]).click()
