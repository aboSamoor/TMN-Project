#! /usr/bin/env python

import httplib
import json
import sys, os
import settings
import store
import urllib
import Queue
import threading

def search(query):
    try:
        c = httplib.HTTPConnection('boss.yahooapis.com')
        query = query.encode('utf-8')
        url = '/ysearch/web/v1/'+query+'?appid='+settings.bossID+'&count=1&lang=en&style=raw'
        c.request('GET', url)
        d = c.getresponse()
        c.close()
    except:
        return ''
    if int(d.status) == 200:
        results = json.load(d)
        totalHits = str(results["ysearchresponse"]["totalhits"])
        return totalHits
    else:
        print "Error:start"
        print d.status, d.reason
        print d.read()
        print "Error:end"
        return ''

engine = store.store(search, os.path.abspath(sys.argv[1]))

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print "usage: $yahooResults.py repository [word(s)|file]" 
        sys.exit()
    if os.path.isfile(sys.argv[2]):
        fName = os.path.abspath(sys.argv[2])
        items = []
        for line in open(fName, 'r').read().splitlines():
            q = '+'.join(sorted(filter(lambda x: x,line.split(' '))))
            items.append(q)
        engine.prepare(items)

    else:
        for word in sys.argv[2:]:
            print word, engine.get(word)
    print engine.dictionary
