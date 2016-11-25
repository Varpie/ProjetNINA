# -*- coding: utf-8 -*-
import scrapy
from scrapy.crawler import CrawlerProcess
import lxml.etree
import lxml.html
import lxml.html.clean
from scrapy.item import Item, Field

class parsedItem(Item):
    parsed_html = Field()

class GetHtmlSpider(scrapy.Spider):
    name = "getHtml"
    def __init__(self, var_url=None, *args, **kwargs):
        super(GetHtmlSpider, self).__init__(*args, **kwargs)
        self.start_urls = [var_url]
    def parse(self,response):
        root = lxml.html.fromstring(response.body)
        root = root.body
        cleaner = lxml.html.clean.Cleaner()
        cleaner.javascript = True
        cleaner.style = True
        root = cleaner.clean_html(root)
        item['parsed_html'] = lxml.html.tostring(root)

item = parsedItem()
def runspider_with_url(var_url):
    process = CrawlerProcess({
        'USER_AGENT': 'Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)'
    })
    process.crawl(GetHtmlSpider,var_url=var_url)
    process.start()
    return item['parsed_html']
