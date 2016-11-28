from selenium import webdriver
import random
from subprocess import call


driver = webdriver.Firefox()
driver.get("https://www.google.com")
inputElement = driver.find_element_by_name("q")
inputElement.click();
call(["../Uinput/uinput", "Ceci est un Test"])
