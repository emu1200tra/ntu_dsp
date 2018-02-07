#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "hmm.h"

int main(int argc , char* argv[]){
  //iteration for Baum-Welch algorithm 
  int iteration = atoi(argv[1]);
  //init model
  char model_init[50];
  strcpy(model_init , argv[2]);
  char path[50] = "";
  strcat(path , model_init);

  HMM hmm_initial;
  loadHMM( &hmm_initial, path );

  //observed seq
  char seq_model[50];
  strcpy(seq_model , argv[3]);
  char observe_seq[10000][50]; //10000 , 50 in seq_model
  
  FILE* file;
  strcpy(path , "");
  strcat(path , seq_model);
  file = fopen(path , "r");
  char tmp;
  int index = 0;
  int count_file1 = 0 , count_file2 = 0;
  while (fscanf(file , "%c" , &tmp) == 1){    
    if(tmp != '\n'){
      observe_seq[count_file1][count_file2] = tmp;
      count_file2++;
    }
    else{
      count_file1++;
      index = count_file2;
      count_file2 = 0;
    }
  }

  fclose(file);
  
  double count = 0.0;
  double *pi = malloc(hmm_initial.state_num * sizeof(double));
  double **sig_erth = malloc(hmm_initial.state_num * sizeof(double*));
  for(int i = 0 ; i < hmm_initial.state_num ; i++){
    sig_erth[i] = malloc(hmm_initial.state_num * sizeof(double));
  }
  double **sig_ok_gam = malloc(hmm_initial.state_num * sizeof(double*));
  for(int i = 0 ; i < hmm_initial.state_num ; i++){
    sig_ok_gam[i] = malloc(hmm_initial.state_num * sizeof(double));
  }
  double *sig_gam = malloc(hmm_initial.state_num * sizeof(double));
  double *sig_gam1 = malloc(hmm_initial.state_num * sizeof(double));
  double** alpha; //hmm_initial.state_num = 6 in sample
  alpha = malloc(index * sizeof(double*));
  for(int i = 0 ; i < index ; i++){
    alpha[i] = malloc(hmm_initial.state_num*sizeof(double));
  }
  double** beta; //hmm_initial.state_num = 6 in sample
  beta = malloc(index * sizeof(double*));
  for(int i = 0 ; i < index ; i++){
    beta[i] = malloc(hmm_initial.state_num*sizeof(double));
  }
  double** gamma; //hmm_initial.state_num = 6 in sample
  gamma = malloc(index * sizeof(double*));
  for(int i = 0 ; i < index ; i++){
    gamma[i] = malloc(hmm_initial.state_num*sizeof(double));
  }
  double*** erthlon; //hmm_initial.state_num = 6 in sample
  erthlon = malloc(index * sizeof(double**));
  for(int i = 0 ; i < index ; i++){
    erthlon[i] = malloc(hmm_initial.state_num*sizeof(double*));
    for(int j = 0 ; j < hmm_initial.state_num ; j++){
      erthlon[i][j] = malloc(hmm_initial.state_num*sizeof(double));
    }
  }

  for(int times = 0 ; times < iteration ; times++){
    for(int i = 0 ; i < hmm_initial.state_num ; i++){
      pi[i] = 0;
    }
    for(int i = 0 ; i < hmm_initial.state_num ; i++){
      for(int j = 0 ; j < hmm_initial.state_num ; j++){
        sig_erth[i][j] = 0;
      } 
    }
    for(int i = 0 ; i < hmm_initial.state_num ; i++){
      for(int j = 0 ; j < hmm_initial.state_num ; j++){
        sig_ok_gam[i][j] = 0;
      }
    }
    for(int i = 0 ; i < hmm_initial.state_num ; i++){
      sig_gam[i] = 0;
    }
    for(int i = 0 ; i < hmm_initial.state_num ; i++){
      sig_gam1[i] = 0;
    }

    for(int itera = 0 ; itera < count_file1 ; itera++){
      for(int i = 0 ; i < index ; i++){
        for(int j = 0 ; j < hmm_initial.state_num ; j++){
          for(int k = 0 ; k < hmm_initial.state_num ; k++){
            erthlon[i][j][k] = 0;
            alpha[i][j] = 0;
            beta[i][j] = 0;
            gamma[i][j] = 0;
          }
        }
      }


      for(int i = 0 ; i < hmm_initial.state_num ; i++){
        alpha[0][i] = hmm_initial.initial[i]*hmm_initial.observation[observe_seq[itera][0]-'A'][i];
      }


      for(int i = 0 ; i < index-1 ; i++){
        for(int j = 0 ; j < hmm_initial.state_num ; j++){
          count = 0.0;
          for(int k = 0 ; k < hmm_initial.state_num ; k++){
            count += alpha[i][k]*hmm_initial.transition[k][j];
          }

          alpha[i+1][j] = count*hmm_initial.observation[observe_seq[itera][i+1]-'A'][j];
        }
      }
      //calculate beta (backward procedure)
      
      for(int i = 0 ; i < hmm_initial.state_num ; i++){
        beta[index-1][i] = 1;
      }
      for(int i = index-2 ; i >= 0 ; i--){
        for(int j = 0 ; j < hmm_initial.state_num ; j++){
          count = 0.0;
          for(int k = 0 ; k < hmm_initial.state_num ; k++){
            count += hmm_initial.transition[j][k]*hmm_initial.observation[observe_seq[itera][i+1]-'A'][k]*beta[i+1][k];
          }
          beta[i][j] = count;
        }
      }

      //calculate gamma      
      
      for(int i = 0 ; i < index ; i++){
        count = 0.0;
        for(int k = 0 ; k < hmm_initial.state_num ; k++){
          count += alpha[i][k]*beta[i][k];
        }
        for(int j = 0 ; j < hmm_initial.state_num ; j++){
          
          gamma[i][j] = ((alpha[i][j] * beta[i][j]) / count);// here
        }
      }
      //calculate erthlon
      
      for(int i = 0 ; i < index-1 ; i++){
        count = 0.0;
        for(int l = 0 ; l < hmm_initial.state_num ; l++){
          for(int m = 0 ; m < hmm_initial.state_num ; m++){
            count += alpha[i][l]*hmm_initial.transition[l][m]*hmm_initial.observation[observe_seq[itera][i+1]-'A'][m]*beta[i+1][m];
          }
        }

        for(int j = 0 ; j < hmm_initial.state_num ; j++){
          for(int k = 0 ; k < hmm_initial.state_num ; k++){
            
            erthlon[i][j][k] = ((alpha[i][j]*hmm_initial.transition[j][k]*hmm_initial.observation[observe_seq[itera][i+1]-'A'][k]*beta[i+1][k]) / count);
          }
        }
      }
      
      for(int i = 0 ; i < hmm_initial.state_num ; i++){
        pi[i] += gamma[0][i];
        for(int j = 0 ; j < hmm_initial.state_num ; j++){
          for(int k = 0 ; k < index-1 ; k++){
            sig_erth[i][j] += erthlon[k][i][j];
          }
        }
        for(int j = 0 ; j < index ; j++){
          sig_gam[i] += gamma[j][i];
          for(int k = 0 ; k < hmm_initial.state_num ; k++){
            if((observe_seq[itera][j] - 'A') == k){
              sig_ok_gam[i][k] += gamma[j][i];
            }  
          }
        }
        for(int j = 0 ; j < index-1 ; j++){
          sig_gam1[i] += gamma[j][i];
        }
      }
    }
    for(int i = 0 ; i < hmm_initial.state_num ; i++){
      hmm_initial.initial[i] = pi[i] / count_file1;
      for(int j = 0 ; j < hmm_initial.state_num ; j++){
        hmm_initial.transition[i][j] = sig_erth[i][j] / sig_gam1[i];
        hmm_initial.observation[j][i] = sig_ok_gam[i][j] / sig_gam[i];
      }
    }
  }
  strcpy(path , argv[4]);
  
  strcpy(hmm_initial.model_name , path);
  FILE* output_model;
  output_model = fopen(hmm_initial.model_name , "w");
  dumpHMM( output_model , &hmm_initial);
  fclose(output_model);
  free(alpha);  free(beta); free(gamma); free(erthlon);
  free(sig_gam); free(sig_ok_gam); free(sig_erth); free(pi);
  return 0;
}
