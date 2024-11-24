#ifndef __UTILS_H__
#define __UTILS_H__

#include<strings.h>

bool isPowerTwo(long N){
  return (N != 0) && ((N & (N - 1)) == 0);
}

typedef struct {
  bool DEBUG; // handles printing schemes
  const long N; // size of the initialized matrices
} ExpSetup;


ExpSetup parse_setup(int argc, char*argv[]){
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <long-integer>\n", argv[0]);
    exit(1);
  }

  char *endptr; // to check for conversion errors
  const long N = strtol(argv[1], &endptr, 10);

  if (*endptr != '\0') {
    fprintf(stderr, "Invalid number: %s\n", argv[1]);
    exit(1);
  } else if (!isPowerTwo(N)){
    fprintf(stderr, "Number should be a power of two, and is now: %s\n", argv[1]);
    exit(1);
  }
  
  // parsing DEBUG mode for verbosity
  bool debug = false;
  char *debug_env = getenv("DEBUG");

  if (debug_env != NULL) {
    if (strcmp(debug_env, "1") == 0 || strcasecmp(debug_env, "true") == 0) {
      debug = true;
    } else if (strcmp(debug_env, "0") == 0 || strcasecmp(debug_env, "false") == 0) {
      debug = true;
    } else {
      fprintf(stderr, "DEBUG has an invalid value ('%s'). Please use '1', '0', 'true', or 'false'.\n", debug_env);
    }
  }

  return (ExpSetup) {.DEBUG = debug, .N = N};
}


void generate_symmetric_matrix(float* matrix, const long N) {
  for (int i = 0; i < N; i++) {
    for (int j = i; j < N; j++) {
      float val = (float)rand() / RAND_MAX;
      matrix[i * N + j] = val;
      matrix[j * N + i] = val;
    }
  }
}

void generate_uniform_random(float* matrix, const long N){
  // uniform init on float range
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      matrix[i * N + j] = (float) rand() / (RAND_MAX) * (FLT_MAX - FLT_MIN) + FLT_MIN;
    }
  }
}


#endif // !__UTILS_H__
