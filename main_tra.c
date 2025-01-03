/*
 * Author: Francesco Paissan, 2024
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<float.h> // i'm using this to get the random range
#include <sys/time.h>

#include "func.h"
#include "utils.h"

#define REPEAT_MEASURES 2000
#define ALIGNMENT 32  // bytes

int main(int argc, char*argv[]) {

    ExpSetup set = parse_setup(argc, argv);

    if(set.DEBUG) printf("DEBUG: Allocating matrix M of size %ld.\n", set.N);
    // float* M = (float*)malloc(set.N * set.N * sizeof(float));
    // float* T = (float*)malloc(set.N * set.N * sizeof(float));
    float* M = (float*)aligned_alloc(ALIGNMENT, set.N * set.N * sizeof(float));
    float* T = (float*)aligned_alloc(ALIGNMENT, set.N * set.N * sizeof(float));

    if(M == NULL || T == NULL) {
        fprintf(stderr, "Could not initial matrix of size %s.\n", argv[1]);
    }

    if(set.DEBUG) {
        printf("DEBUG: Checking alignment for M:");
        check_alignment(M, 32);
        printf("DEBUG: Checking alignment for T:");
        check_alignment(T, 32);
    }

    if(set.DEBUG) printf("DEBUG: Initializing M to be symmetric.\n");
    generate_symmetric_matrix(M, set.N);

    double cpu_time_used;
    struct timeval start, end;


    if(set.DEBUG) printf("DEBUG: Starting Matrix Transposition Check.\n");


    long seconds, microseconds;
    double elapsed, delta;
    for(int i=0; i<REPEAT_MEASURES; i++) {
        if(set.STRAT == SEQUENTIAL) {
            gettimeofday(&start, NULL);
            matTranspose(M, T, set.N);
            gettimeofday(&end, NULL);
        } else if(set.STRAT == IMPLICIT) {
            gettimeofday(&start, NULL);
            matTransposeImp(M, T, set.N);
            gettimeofday(&end, NULL);
        } else if(set.STRAT == TILED) {
            gettimeofday(&start, NULL);
            matTransposeTiled(M, T, set.N, 64);
            gettimeofday(&end, NULL);
        } else if(set.STRAT == TILEDOMP) {
            gettimeofday(&start, NULL);
            matTransposeTiledOMP(M, T, set.N, 64);
            gettimeofday(&end, NULL);
        } else if(set.STRAT == OMP) {
            gettimeofday(&start, NULL);
            matTransposeOMP(M, T, set.N);
            gettimeofday(&end, NULL);
        } else if(set.STRAT == CO) {
            gettimeofday(&start, NULL);
            matTranspose_CO(M, T, set.N, 0, set.N, 0, set.N);
            gettimeofday(&end, NULL);
        }

        seconds = end.tv_sec - start.tv_sec;
        microseconds = end.tv_usec - start.tv_usec;
        delta = seconds + 1e-6*microseconds;
        if(!set.DEBUG) printf("%f\n", delta);
        elapsed += delta;
    }
    elapsed /= REPEAT_MEASURES;

    if(set.DEBUG) printf("Elapsed: %.6f s\n", elapsed);

    if(set.DEBUG) {
        if(checkTranspose(M, T, set.N)) {
            printf("DEBUG: Transpose successful.\n");
        } else {
            printf("DEBUG: Something is off in transpose. DO NOT IGNORE THIS!\n");
        }
    }

    free(M);
    free(T);


    return 0;
}
