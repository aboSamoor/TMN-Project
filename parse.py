#! /usr/bin/python

import sys
import re
import urllib2
import string

class entry:
	def __init__(self,q="",t="",o="U"):
		self.query = q
		self.time = t
		self.owner = o
	def __repr__(self):
		return ', '.join([self.query, self.owner, self.time])

def cleanup(text):
	exclude = set(string.punctuation)
	text = ''.join(ch for ch in text if ch not in exclude)
	return text

def extra_format(text):
	text=urllib2.unquote(text).replace('+',' ')
	return text
	
def remove_redundancy1(queries):
	collection={}
	for i in reversed(range(len(queries))):
		if not collection.has_key(queries[i][0][0]):
			collection[queries[i][0][0]] = queries[i][0][1]
	return collection

#watch that queries with the same string in different times will be
#deleted this is a bug for simplicity ;).
def remove_redundancy2(collection):
	collection2={}
	for key in collection.keys():
		if not collection2.has_key(collection[key]):
			collection2[collection[key]] = key
	return collection2

def parse_proxy_log(fname):
	filp = open(fname,'r')
	text = filp.readlines()
	filp.close()
	queries = lambda l: re.findall("(\d+:\d+:\d+)-.*&q=(.*?)&",l)
	tmp = filter(lambda x: x!=[],[queries(l) for l in text])
	tmp2 = remove_redundancy2(remove_redundancy1(tmp))
	return [entry(extra_format(item),tmp2[item],"U") for item in tmp2]

def parse_tmn_log(fname):
	filp = open(fname,'r')
	text = filp.readlines()
	filp.close()
	queries = lambda l: re.findall("query='(.*?)'.*(\d\d:\d\d:\d\d)",l)
	res = filter(lambda x: len(x) > 0,[queries(l) for l in text])
	return [entry(e[0][0],e[0][1],"T") for e in res]

if __name__=="__main__":
	items1 = parse_proxy_log(sys.argv[1])
	if len(sys.argv) > 3 and sys.argv[3] == '-d':
		print "extracted from the proxy log ----------------"
		print items1
	
	items2 = parse_tmn_log(sys.argv[2])
	if len(sys.argv) > 3 and sys.argv[3] == '-d':
		print "extracted from TMN log ----------------"
		print items2
	
	for e in items2:
		tmp = filter(lambda x: x.query == e.query or x.time[3:] == e.time[3:], items1)
		for i in tmp:
			i.owner = "T"
	
	if len(sys.argv) > 3 and sys.argv[3] == '-d':
		print "After merging ............................"
	
	for i in items1:
		print cleanup(i.query)+", "+i.owner+", "+i.time
