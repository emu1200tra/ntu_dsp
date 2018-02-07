import re
import string

dictionary = dict()
comment = re.compile(';;;*')
split = re.compile('(.*)  (.*)\n')
digit = re.compile('\d')

with open('cmudict-0.7b-utf8.txt','r') as fp:
    ss = fp.readlines()
    for s in ss:
        if comment.match(s): continue
        k,v = split.match(s).group(1,2)
        v = digit.sub('',v)
        dictionary[k]=v+' $'

def w2p(w):
    return dictionary[w.upper()]
