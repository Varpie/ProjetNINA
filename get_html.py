# -*- coding: utf-8 -*-
import scrapy
class GethtmlSpider(scrapy.Spider):
   name = "getHtml"

	def parse_page(self, response):
    	return scrapy.Request("ttp://www.google.com",
                          callback=self.parse)
    
  #  def make_requests_from_url(url):
#    	yield scrapy.Request(url=url, callback=self.parse)
    def parse(self, response):
     	return response.body
