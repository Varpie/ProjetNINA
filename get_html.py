# -*- coding: utf-8 -*-
import scrapy
import lxml.etree
import lxml.html

class GetHtmlSpider(scrapy.Spider):
	name = "getHtml"
	url = "https://www.wikipedia.org"
	def start_requests(self):
		yield scrapy.Request(url, self.parse)

	def parse(self,response):
		root = lxml.html.fromstring(response.body)
		print lxml.html.tostring(root)