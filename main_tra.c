#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<float.h> // i'm using this to get the random range
#include <sys/time.h>

#include "func.h"
#include "utils.h"

#define REPEAT_MEASURES 1

int main(int argc, char*argv[]){

  ExpSetup set = parse_setup(argc, argv);

  if(set.DEBUG) printf("DEBUG: Allocating matrix M of size %ld.\n", set.N);
  float* M = (float*)malloc(set.N * set.N * sizeof(float));
  float* T = (float*)malloc(set.N * set.N * sizeof(float));

  if(M == NULL || T == NULL){
    fprintf(stderr, "Could not initial matrix of size %s.\n", argv[1]);
  }

  if(set.DEBUG) printf("DEBUG: Initializing M to be symmetric.\n");
  generate_symmetric_matrix(M, set.N);

  // if(set.DEBUG) printf("DEBUG: Initializing M with random values.\n");
  // generate_uniform_random(M, set.N);

  double cpu_time_used;
  struct timeval start, end;


  if(set.DEBUG) printf("DEBUG: Starting Matrix Transposition Check.\n");

  gettimeofday(&start, NULL);
  matTranspose_CO(M, T, set.N, 0, set.N, 0, set.N);
  gettimeofday(&end, NULL);

  long seconds = end.tv_sec - start.tv_sec;
  long microseconds = end.tv_usec - start.tv_usec;
  double elapsed = seconds + 1e-6*microseconds;

  printf("Cache-oblivious Elapsed: %.6f s\n", elapsed);

  gettimeofday(&start, NULL);
  matTranspose(M, T, set.N);
  gettimeofday(&end, NULL);

  seconds = end.tv_sec - start.tv_sec;
  microseconds = end.tv_usec - start.tv_usec;
  elapsed = seconds + 1e-6*microseconds;

  printf("Baseline Elapsed: %.6f s\n", elapsed);

  gettimeofday(&start, NULL);
  matTransposeImp(M, T, set.N);
  gettimeofday(&end, NULL);

  seconds = end.tv_sec - start.tv_sec;
  microseconds = end.tv_usec - start.tv_usec;
  elapsed = seconds + 1e-6*microseconds;

  printf("Imp Elapsed: %.6f s\n", elapsed);

  gettimeofday(&start, NULL);
  // Tile size should be balanced. I want to avoid overhead due to small tiles
  // If tiles are to big I get cache thrashing
  matTransposeTiled(M, T, set.N, 64);
  gettimeofday(&end, NULL);

  seconds = end.tv_sec - start.tv_sec;
  microseconds = end.tv_usec - start.tv_usec;
  elapsed = seconds + 1e-6*microseconds;

  printf("Tiled Elapsed: %.6f s\n", elapsed);

  free(M);
  free(T);
  

  return 0;
}
