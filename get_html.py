# -*- coding: utf-8 -*-
from scrapy.spider import BaseSpider
import lxml.etree
import lxml.html

class GetHtmlSpider(BaseSpider):
	name = "getHtml"
	start_urls = ["https://www.google.fr/webhp?ie=utf-8&oe=utf-8&gws_rd=cr&ei=b_7xV5GtOIHiU63whPgI#q=coucou"]

	def parse(self,response):
		root = lxml.html.fromstring(response.body)
		print lxml.html.tostring(root)