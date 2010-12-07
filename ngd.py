#! /usr/bin/python

import re
import xgoogle.search as s
import sys
import math
import threading
from twisted.internet.defer import *
from twisted.web.client import getPage
from lxml.html import fromstring
from twisted.internet import reactor

results = []
queries = []
cache = {}
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

def parse(fname):
	filp = open(fname, 'r')
	text = filp.readlines()
	filp.close()
	first_col = filter( lambda x: x, [line.split(',')[0] for line in text])
	queries = [query.split(' ') for query in first_col]
	return queries

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
def errorHandler(error):
    '''
    This is an 'errback' function, added to the Deferred which will call
    it in the event of an error
    '''

    # this isn't a very effective handling of the error, we just print it out:
    print "An error has occurred: <%s>" % str(error)
    # and then we stop the entire process:
    reactor.stop()


if __name__=="__main__":
	queries = parse(sys.argv[1])
	for query in queries:
		print url(query)
	args=[]
	for i in range(len(queries)):
		results.append([])
		for j in range(len(queries)):
			results[i].append(-1)
	for i in range(len(queries)):
		for j in range(len(queries)):
				print url(queries[i]+queries[j])
"""
	for i in range(30):
		for j in range(i+1,30):
			if not cache.has_key(url(queries[i])):
				d1 = getPage(url(queries[i]))
				x = d1.addCallback(parse_page)
				d1.addErrback(errorHandler)
			else:
				x = cache[url(queries[i])]
			
			if not cache.has_key(url(queries[j])):
				d2 = getPage(url(queries[j]))
				y  = d2.addCallback(parse_page)
				d2.addErrback(errorHandler)
			else:
				y = cache[url(queries(j))]
			
			d3 = getPage(url(queries[i]+queries[j]))
			z = d3.addCallback(parse_page)
			d3.addErrback(errorHandler)
			
			gatherResults([x,y,z]).addCallback(store,i,j)
			
	reactor.run() 

"""
