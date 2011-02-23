#! /usr/bin/env python

import httplib
import json
import sys, os
import settings
import store
import urllib

c = httplib.HTTPSConnection('www.googleapis.com')

def search(queries):
    global c
    query = queries.encode('utf-8')
    url = '/customsearch/v1?key='+settings.key+'&cx='+settings.cx+'&q='+query+'&num=2'
    c.request('GET', url)
    d = c.getresponse()
    if int(d.status) == 200:
        results = json.load(d)
        return str(results["queries"]["request"][0]["totalResults"])
    else:
        print "Error:start"
        print d.status, d.reason
        print d.read()
        print "Error:end"
        return ''

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print "usage: $googleResults.py repository [word|file]" 
        sys.exit()
    engine = store.store(search, os.path.abspath(sys.argv[1]))
    if os.path.isfile(sys.argv[2]):
        fName = os.path.abspath(sys.argv[2])
        for line in open(fName, 'r').read().splitlines():
            q = '+'.join(sorted(line.split(' ')))
            print q, engine.get(q)
    else:
        for word in sys.argv[2:]:
            print word, engine.get(word)
    engine.save()
    print engine.dictionary
    c.close()
