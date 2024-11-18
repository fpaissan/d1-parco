#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<float.h> // i'm using this to get the random range
#include <time.h>

#define REPEAT_MEASURES 100

bool checkSym(const int N, const float M[N][N]){
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      if(M[i][j] != M[j][i]){ return false; }
    }
  }

  return true;
}

void matTranspose(const int N, const float M[N][N], float ret[N][N]){
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      ret[i][j] = M[j][i];
    }
  }
}

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
  clock_t start, end;
  // print 100 wall-clock time measures for checkSym
  for(int i=0; i<REPEAT_MEASURES; i++){

	  start = clock();
    checkSym(N, M);
	  end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f\n", cpu_time_used);
  }

  // print 100 wall-clock time measures for matTranspose
  for(int i=0; i<REPEAT_MEASURES; i++){

	  start = clock();
    matTranspose(N, M, T);
	  end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f\n", cpu_time_used);
  }


  return 0;
}
