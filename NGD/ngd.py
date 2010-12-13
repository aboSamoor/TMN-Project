#! /usr/bin/python

import re
import sys
import math
import threading
from lxml.html import fromstring

results = []
queries = []
cache = {}

class entry:
    def __init__(self,q=[],t="",o="U"):
        self.query = q
        self.time = t
        self.owner = o
        self.url = url(q)
    def __repr__(self):
        return ', '.join([" ".join(self.query), self.owner, self.time])

def parse_page(html):
	if html:
		matches = re.findall("About ([\d+,*]+?) results",html)
		if matches:
			return int(matches[0].replace(',',''))
	return -1

def lg2(x):
	return math.log(x,2)

def calc(x,y,z):
	m = lg2(1E12)
	return (lg2(max(x,y))-lg2(z))/(m-lg2(min(x,y)))

def parse_queries(fname):
    filp = open(fname, 'r')
    text = filp.readlines()
    filp.close()
    lines = []
    lines = filter( lambda x: len(x) > 2, [line[:-1].split(',') for line in text])
    queries = [entry(line[0].split(' '),line[2],line[1]) for line in lines]
    return queries


def parse_urls_stored(fname):
	filp = open(fname, 'r')
	text = filp.readlines()
	filp.close()
	urls = filter( lambda x: len(x) > 1, [line[:-1].split(',') for line in text])
	return urls

def url(q):
	q1='+'.join(q)
	return "http://www.google.com/search?q="+q1

def store(l,i,j):
	if not cache.has_key(url(queries[i])):
		cache[url(queries[i])] = l[0]
#		print i, l[0]
	if not cache.has_key(url(queries[j])):
		cache[url(queries[j])] = l[1]
#		print j, l[1]
	results[i][j] = l[2]
	print (i,j),l[2]
	
	#results[i][j] = calc(l[0],l[1],[2])
	#print i,j,results[i][j]

def requested_urls(fname):
    queries = []
    queries = parse_queries(fname)
    print len(queries)
    requested_urls = []
    for i in range(len(queries)):
        requested_urls.append([queries[i]])
        for j in range(i+1,len(queries)):
            requested_urls[-1].append(entry(queries[i].query+queries[j].query,"mixed","mixed"))
    return requested_urls

def first_col(stored_urls):
    for url in stored_urls:
        yield url[0]

def check_for_missing(urls, stored_urls):
    missing = []
    cache = {}

    for done_url in stored_urls:
        cache[done_url[0]]= done_url[1]

    for i in range(len(urls)):
        for j in range(len(urls[i])):
            if not cache.has_key(urls[i][j].url):
                missing.append(urls[i][j].url)
    return missing

def lg2(x):
    return math.log(x,2)

def eq(x,y,z):
    m = lg2(1E12)
    return (lg2(max(x,y))-lg2(z))/(m-lg2(min(x,y)))

if __name__=="__main__":
    urls = []
    urls = requested_urls(sys.argv[1])

    stored_urls = []
    stored_urls = parse_urls_stored(sys.argv[2])
    missing = check_for_missing(urls, stored_urls)
    if missing:
        filp = open('missing', 'w')
        filp.write('\n'.join(missing))
        filp.close()

    n = len(urls[0])
    print n 
    results= []
    for i in range(n):
        results.append([]) 
        for j in range(n):
            results[i].append(0)
            
    for done_url in stored_urls:
        cache[done_url[0]]= float(done_url[1])
    
    for i in range(n):
        for j in range(i+1,n):
            if cache.has_key(urls[i][0].url):
                x = cache[urls[i][0].url] +2
            else:
                x = 2

            if cache.has_key(urls[j][0].url):
                y = cache[urls[j][0].url] +2
            else:
                y = 2

            if cache.has_key(urls[i][j-i].url):
                z = cache[urls[i][j-i].url] +2
            else:
                z = 2
            results[i][j] = eq(x,y,z)
            results[j][i] = results[i][j]
    filp = open('result', 'w')
    tags = [urls[i][0].owner for i in range(n)]
    filp.write(",".join(tags)+"\n")
    k = 0
    for line in results:
        line_str = urls[k][0].owner+", "+", ".join([str(num) for num in line])+"\n"
        filp.write(line_str)
        k+=1
    filp.close()
