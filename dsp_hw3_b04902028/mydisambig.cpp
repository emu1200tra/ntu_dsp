//done bigram ; but still not good enough.

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "Ngram.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

struct mapping_vec{
    string key;
    vector<string>value;
};


int main(int argc , char* argv[]){
  
  //error checking

  string check("-text");
  if (check.compare(argv[1]) != 0){
    perror("error: text error");
  }
  check = "-map";
  if (check.compare(argv[3]) != 0){
    perror("error: map error");
  }
  check = "-lm";
  if (check.compare(argv[5]) != 0){
    perror("error: lm error");
  }
  check = "-order";
  if (check.compare(argv[7]) != 0){
    perror("error: order error");
  }


  //read file
  //read map file
  ifstream infile1(argv[4]);
  string line;
  vector<mapping_vec>mapping_key;
  while (getline(infile1, line)){
    mapping_vec tmp;
    string r1(1, line[0]);
    string r2(1, line[1]);
    tmp.key = (r1+r2);

    for(int i = 3 ; i < line.length() ; i++){
      if(line[i] != ' ' && (i % 3) == 0 && line[i] != '\n'){
        string tmp_word1(1 , line[i]);
        string tmp_word2(1 , line[i+1]);
        string tmp_word = tmp_word1 + tmp_word2;
        tmp.value.push_back(tmp_word);    
      }
    }
    mapping_key.push_back(tmp);  
  }

  //read lm file
  int ngram_order = atoi(argv[8]);
  Vocab voc;
  Ngram lm( voc, ngram_order );

  char* lm_filename = argv[6];
  File lmFile( lm_filename, "r" );
  lm.read(lmFile);
  lmFile.close();

  //read text file and process
  ifstream infile(argv[2]);
  while (getline(infile, line)){
    vector<int> record_place;
    vector<string>line_seg;
    //find place of every key and value
    for(int i = 0 ; i < line.length() ; i++){
      if(line[i] != ' '){
        line_seg.push_back(string() + line[i] + line[i+1]);
        int place = 0;
        for (int j = 0 ; j < mapping_key.size() ; j++){
          if(mapping_key[j].key.compare(string() + line[i] + line[i+1]) == 0){
            place = j;
            break;
          }
          else{
            place = -1;
          }
        }    
        record_place.push_back(place);
        i++;
      }
    }
    vector< vector<float> >delta_prob;
    vector<string>place_word;
    
    //find prob
    //init prob delta[0]
    vector<float>tmp;
    for(int i = 0 ; i < mapping_key[record_place[0]].value.size() ; i++){
      VocabIndex wid = voc.getIndex(mapping_key[record_place[0]].value[i].c_str());
      if(wid == Vocab_None) {
        wid = voc.getIndex(Vocab_Unknown);
      }
      VocabIndex context[] = {Vocab_None};
      tmp.push_back(lm.wordProb(wid, context));
    }
    delta_prob.push_back(tmp);
    tmp.clear();

    //viterbi delta[i]
    for(int i = 1 ; i < line_seg.size() ; i++){
      if (ngram_order == 2){
        int index = distance(delta_prob[i-1].begin() , max_element(delta_prob[i-1].begin() , delta_prob[i-1].end()));
        place_word.push_back(mapping_key[record_place[i-1]].value[index]);
        for(int j = 0 ; j < mapping_key[record_place[i]].value.size() ; j++){
          VocabIndex wid1 = voc.getIndex(place_word[i-1].c_str());
          VocabIndex wid2 = voc.getIndex(mapping_key[record_place[i]].value[j].c_str());
          if(wid1 == Vocab_None)  {
            wid1 = voc.getIndex(Vocab_Unknown);
          }
          if(wid2 == Vocab_None)  {
            wid2 = voc.getIndex(Vocab_Unknown);
          }
          VocabIndex context[] = { wid1, Vocab_None };
          tmp.push_back(lm.wordProb( wid2, context) + delta_prob[i-1][index]);    
        }
        delta_prob.push_back(tmp);
        tmp.clear();
      }
      else if (ngram_order == 3){
        if(i == 1){
          int index = distance(delta_prob[i-1].begin() , max_element(delta_prob[i-1].begin() , delta_prob[i-1].end()));
          place_word.push_back(mapping_key[record_place[i-1]].value[index]);
          for(int j = 0 ; j < mapping_key[record_place[i]].value.size() ; j++){
            VocabIndex wid1 = voc.getIndex(place_word[i-1].c_str());
            VocabIndex wid2 = voc.getIndex(mapping_key[record_place[i]].value[j].c_str());
            if(wid1 == Vocab_None)  {
              wid1 = voc.getIndex(Vocab_Unknown);
            }
            if(wid2 == Vocab_None)  {
              wid2 = voc.getIndex(Vocab_Unknown);
            } 
            VocabIndex context[] = { wid1, Vocab_None };
            tmp.push_back(lm.wordProb( wid2, context) + delta_prob[i-1][index]);          
          }
          delta_prob.push_back(tmp);
          tmp.clear();
        }
        else{
          int index = distance(delta_prob[i-1].begin() , max_element(delta_prob[i-1].begin() , delta_prob[i-1].end()));
          place_word.push_back(mapping_key[record_place[i-1]].value[index]);
          for(int j = 0 ; j < mapping_key[record_place[i]].value.size() ; j++){
            VocabIndex wid1 = voc.getIndex(place_word[i-2].c_str());
            VocabIndex wid2 = voc.getIndex(place_word[i-1].c_str());
            VocabIndex wid3 = voc.getIndex(mapping_key[record_place[i]].value[j].c_str());
            if(wid1 == Vocab_None)  {
                wid1 = voc.getIndex(Vocab_Unknown);
              }
            if(wid2 == Vocab_None)  {
                wid2 = voc.getIndex(Vocab_Unknown);
              }
            if(wid3 == Vocab_None) {
              wid3 = voc.getIndex(Vocab_Unknown);
            }
            VocabIndex context[] = { wid2, wid1, Vocab_None };
            tmp.push_back(lm.wordProb( wid3, context ) + delta_prob[i-1][index]); 
          }
          delta_prob.push_back(tmp);
          tmp.clear();
        }
      }
      else{
        perror("error Ngram!!");
        return -1;
      }
    }
    //find final word
    int index = distance(delta_prob[line_seg.size()-1].begin() , max_element(delta_prob[line_seg.size()-1].begin() , delta_prob[line_seg.size()-1].end()));
    place_word.push_back(mapping_key[record_place[line_seg.size()-1]].value[index]);

    //print out
    cout << "<s> ";
    for(int i = 0 ; i < place_word.size() ; i++){
      cout << place_word[i] << " ";
    }
    cout << "</s>" << endl;
  }
  perror("done!");
}