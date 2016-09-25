# -*- coding: utf-8 -*-
import scrapy


class TestSpider(scrapy.Spider):
    name = "test"
    allowed_domains = ["google.com"]
    start_urls = (
        'http://www.google.com/',
    )

    def parse(self, response):
        pass
