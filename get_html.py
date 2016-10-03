# -*- coding: utf-8 -*-
from scrapy.spider import BaseSpider
import lxml.etree
import lxml.html

class GetHtmlSpider(BaseSpider):
	name = "getHtml"
	start_urls = ["https://www.wikipedia.org"]

	def parse(self,response):
		root = lxml.html.fromstring(response.body)
		print lxml.html.tostring(root)