from selenium import webdriver

driver = webdriver.Firefox()

driver.get("https://en.wikipedia.org/wiki/Special:Random")
links = driver.find_elements_by_tag_name("a")
dict_links = dict()
for link in links:
    dict_links[link.get_attribute("text")] = link.get_attribute("href")
print dict_links
