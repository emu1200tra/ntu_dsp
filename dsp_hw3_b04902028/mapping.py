#!/usr/bin/env python
# -*- coding: big5hkscs -*-
import codecs
import sys
#filename = "../Big5-ZhuYin.map"

def check(data , word):
  for i in range(len(data)):
    if data[i] == word:
      return i
  return -1

def get_zhuyin(data , index):
  table = []
  for i in range(index):
    table.append(data[i][0])
  return table

filename = codecs.open( sys.argv[1] , 'rb', 'big5hkscs', 'replace')

lines = []
for line in filename:
  lines.append(line.split(' '))

zhuyin = [[] for i in range(37)]
word = []
index = 0

for i in range(len(lines)):
  word.append(lines[i][0])
  length = len(lines[i][1])

  for j in range(length):
    if j == 0 or ((j-1) > 0 and lines[i][1][j-1] == "/"):
      place = check(get_zhuyin(zhuyin , index) , lines[i][1][j])
      #print 'place:' , place , ';check:' , get_zhuyin(zhuyin , index) , lines[i][1][j]
      #sleep(1)
      if place != -1:
        flag = check((zhuyin[place][1:]) , lines[i][0])
        #print 'flag:' , flag
        #sleep(1)
        if flag == -1:
          zhuyin[place].append(lines[i][0])
          #print 'place2:' , place , ';check:' , zhuyin[place][-1]
          #sleep(1)
      else:
        zhuyin[index].append(lines[i][1][j])
        #print 'check append' , zhuyin[index] , zhuyin[index][0] , index
        zhuyin[index].append(lines[i][0])
        #print 'check zhuyin:' , zhuyin[index][0] , zhuyin[index][1]
        #sleep(1)
        index += 1



#print 'word:' , word
#print 'zhuyin' , zhuyin
filename.close()

filename2 = codecs.open( sys.argv[2] , 'w', 'big5hkscs', 'ignore')

for i in range(len(word)):
  if word[i] != []:
    filename2.write(word[i])
    filename2.write('\t')
    filename2.write(word[i])
    filename2.write('\n')


for i in range(len(zhuyin)):
  for j in range(len(zhuyin[i])):
    if j == 0:
      filename2.write(zhuyin[i][j])
      filename2.write('\t')
    else:
      filename2.write(zhuyin[i][j])
      filename2.write(' ')
  
  filename2.write('\n')

filename2.close()

