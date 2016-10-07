# -*- coding: utf-8 -*-
import scrapy
import lxml.etree
import lxml.html

class GetHtmlSpider(scrapy.Spider, url):
	name = "getHtml"
	start_urls = [url]
	def parse(self, response):
		root = lxml.html.fromstring(response.body)
		print lxml.html.tostring(root)
