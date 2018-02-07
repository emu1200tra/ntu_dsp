#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "hmm.h"

int main(int argc , char*argv[])
{
  char modellist[50];
  strcpy(modellist , argv[1]);
  char testing_data[50];
  strcpy(testing_data , argv[2]);
  char result[50];
  strcpy(result , argv[3]);
  HMM hmms[5];
  load_models(modellist, hmms, 5);
  char observe_seq[10000][50]; 
  
  FILE* file;
  file = fopen(testing_data , "r");
  char tmp1;
  int index = 0;
  int count_file1 = 0 , count_file2 = 0;
  while (fscanf(file , "%c" , &tmp1) == 1){    
    if(tmp1 != '\n'){
      observe_seq[count_file1][count_file2] = tmp1;
      count_file2++;
    }
    else{
      count_file1++;
      index = count_file2;
      count_file2 = 0;
    }
  }
  fclose(file);
  FILE* result_file;
  result_file = fopen(result , "w"); 
  double **delta = malloc(index * sizeof(double*));
  for(int i = 0 ; i < index ; i++){
    delta[i] = malloc(hmms[0].state_num * sizeof(double));
  }

  double pair[5];
  double tmp = 0.0;
  for(int times = 0 ; times < count_file1 ; times++){
    for(int count_five = 0 ; count_five < 5 ; count_five++){
      for(int i = 0 ; i < hmms[count_five].state_num ; i++){
        delta[0][i] = hmms[count_five].initial[i]*hmms[count_five].observation[observe_seq[times][0]-'A'][i];
      }
      for(int i = 1 ; i < index ; i++){
        for(int j = 0 ; j < hmms[count_five].state_num ; j++){
          tmp = 0.0;
          for(int k = 0 ; k < hmms[count_five].state_num ; k++){
            if (tmp < delta[i-1][k]*hmms[count_five].transition[k][j]){
              tmp = delta[i-1][k]*hmms[count_five].transition[k][j];
            }
          }
          delta[i][j] = tmp * hmms[count_five].observation[observe_seq[times][i]-'A'][j];
        }
      }
      tmp = 0.0;
      for(int i = 0 ; i < hmms[count_five].state_num ; i++){
        if(tmp < delta[index-1][i]){
          tmp = delta[index-1][i];
        }
      }
      pair[count_five] = tmp;
    }
    int max_model = 0;
    tmp = 0.0;
    for(int i = 0 ; i < 5 ; i++){
      if(tmp < pair[i]){
        tmp = pair[i];
        max_model = i;
      }
    }
    fprintf(result_file, "%s%d%s\t%g\n", "model_0" , max_model+1 , ".txt" , pair[max_model]);
  }
  fclose(result_file);
  if(strcmp(testing_data , "testing_data1.txt") == 0){
    FILE *answer = fopen("testing_answer.txt" , "r");
    FILE *result_file_cmp = fopen("result1.txt" , "r");
    FILE *acc = fopen("acc.txt" , "w");
    char cmp1[2][50] , cmp2[50];
    int counter1 = 0 , counter2 = 0;
    while(fscanf(answer , "%s\n" , cmp2) == 1){
      fscanf(result_file_cmp , "%s\t%s\n" , cmp1[0] , cmp1[1]);
      counter1++;
      if(strcmp(cmp1[0] , cmp2) == 0){
        counter2++;
      }
    } 
    fprintf(acc, "%g\n", (float)counter2/counter1);
    fclose(answer);
    fclose(result_file_cmp);
    fclose(acc);
  }

	
	return 0;
}
