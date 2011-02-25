#! /usr/bin/env python

import os, sys
import itertools

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "usage: getQueries.py csvFile\n"
    csvFile = os.path.abspath(sys.argv[1])
    exts = csvFile.split('.')
    txt = open(csvFile, 'r').read()

    words = []
    for line in txt.splitlines():
        cols = line.split(',')
        for word in cols[0].split(' '):
            words.append(word)
    words = list(set(words))
    pairs = itertools.combinations(words,2)
    text = [x+' '+y for (x,y) in pairs]
    text.extend(words)
    txt = '\n'.join(text)
    fh = open(exts[0]+".q",'w')
    fh.write(txt)
    fh.close()
