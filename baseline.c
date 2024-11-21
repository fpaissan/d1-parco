#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<float.h> // i'm using this to get the random range
#include <sys/time.h>

#include "func.h"

#define REPEAT_MEASURES 1

int main(int argc, char*argv[]){
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <long-integer>\n", argv[0]);
    return 1;
  }

  char *endptr; // to check for conversion errors
  const long N = strtol(argv[1], &endptr, 10);

  if (*endptr != '\0') {
    fprintf(stderr, "Invalid number: %s\n", argv[1]);
    return 1; 
  }

  float M[N][N], T[N][N];

  // uniform init on float range
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      M[i][j] = (float) rand() / (RAND_MAX) * (FLT_MAX - FLT_MIN) + FLT_MIN;
    }
  }

  double cpu_time_used;
  struct timeval start, end;


  // wall-clock should account for mem allocation
  gettimeofday(&start, NULL);
  checkSym(N, M);
  gettimeofday(&end, NULL);

  long seconds = end.tv_sec - start.tv_sec;
  long microseconds = end.tv_usec - start.tv_usec;
  double elapsed = seconds + 1e-6*microseconds;

  printf("Elapsed: %.6f s\n", elapsed);
  

  return 0;
}
