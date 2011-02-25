#! /usr/bin/env python

import httplib
import json
import sys, os
import settings
import store
import urllib
import Queue
import threading

Q = Queue.Queue()


def search(query):
    c = httplib.HTTPConnection('boss.yahooapis.com')
    query = query.encode('utf-8')
    url = '/ysearch/web/v1/'+query+'?appid='+settings.bossID+'&count=1&lang=en&style=raw'
    c.request('GET', url)
    d = c.getresponse()
    c.close()
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

def worker():
    global engine
    while True:
        item = Q.get()
        res = engine.get(item)
        if res != '':
            print threading.current_thread().name, item, res
            Q.task_done()
            if Q.qsize%1000 == 0:
                engine.save()

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print "usage: $yahooResults.py repository [word(s)|file]" 
        sys.exit()
    if os.path.isfile(sys.argv[2]):
        fName = os.path.abspath(sys.argv[2])
        for line in open(fName, 'r').read().splitlines():
            q = '+'.join(sorted(filter(lambda x: x,line.split(' '))))
            Q.put(q)
        for i in range(100):
            t = threading.Thread(target=worker)
            t.daemon = True
            t.start()

    else:
        for word in sys.argv[2:]:
            print word, engine.get(word)
    Q.join()
    engine.save()
    print engine.dictionary
