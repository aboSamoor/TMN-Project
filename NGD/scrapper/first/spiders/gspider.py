from scrapy.spider import BaseSpider
import re
import os.path
import urllib2

store = "/home/eid/SB/courses/f10/SS/project/TMN-Project/store"
cache = {}

def build_cache():
    filp = open(store, 'r')
    lines = filp.readlines()
    filp.close()
    for line in lines:
        [url,value] = line.split(',')
        cache[url] = value
    return cache
    


def get_qs(csvFile):
    filp = open(csvFile,'r')
    qs=filp.readlines()
    filp.close()
    qs = [line[:-1] for line in qs]
    return qs

def parse_page(html):
	if html:
		matches = re.findall("About ([\d+,*]+?) results",html)
		if matches:
			return matches[0].replace(',','')
	return "-1"

class GspiderSpider(BaseSpider):
    name = "gspider"
    allowed_domains = ["google.com"]
    tmp_urls = get_qs("/home/eid/SB/courses/f10/SS/project/TMN-Project/urls")
    start_urls = []
    cache = build_cache()
    for query in tmp_urls:
        if not cache.has_key(query):
            start_urls.append(query)


    def parse(self, response):
        if cache.has_key(response.url):
            print "!!! repeatition !!!"
        elif "sorry.google.com" in response.url:
            print "Goooooooooooooooooooogle"
        else:    
            text= response.url+","+parse_page(response.body)+"\n"
            filp = open(store, 'a')
            filp.write(text)
            filp.close()
        pass 
