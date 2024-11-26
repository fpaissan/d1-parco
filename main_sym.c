#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<float.h> // i'm using this to get the random range
#include <sys/time.h>

#include "func.h"
#include "utils.h"

#define REPEAT_MEASURES 10

int main(int argc, char*argv[])
{

    ExpSetup set = parse_setup(argc, argv);

    if(set.DEBUG) printf("DEBUG: Allocating matrix M of size %ld.\n", set.N);
    float* M = (float*)malloc(set.N * set.N * sizeof(float));

    if(M == NULL)
    {
        fprintf(stderr, "Could not initial matrix of size %s.\n", argv[1]);
    }

    if(set.DEBUG) printf("DEBUG: Initializing M to be symmetric.\n");
    generate_symmetric_matrix(M, set.N);

    // if(set.DEBUG) printf("DEBUG: Initializing M with random values.\n");
    // generate_uniform_random(M, set.N);

    double cpu_time_used;
    struct timeval start, end;


    if(set.DEBUG) printf("DEBUG: Starting Symmetry Check.\n");

    long seconds, microseconds;
    double elapsed;
    bool sym;
    for(int i=0; i<REPEAT_MEASURES; i++)
    {
        gettimeofday(&start, NULL);
        if(set.STRAT == SEQUENTIAL)
            sym = checkSym(M, set.N);
        else if(set.STRAT == IMPLICIT)
            sym = checkSymImp(M, set.N);
        else if(set.STRAT == OMP)
            sym = checkSymOMP(M, set.N);
        gettimeofday(&end, NULL);

        seconds = end.tv_sec - start.tv_sec;
        microseconds = end.tv_usec - start.tv_usec;
        elapsed += seconds + 1e-6*microseconds;
    }
    elapsed /= REPEAT_MEASURES;

    if (set.DEBUG)
    {
        if(sym)
        {
            printf("DEBUG: Matrix is symmetric.\n");
        }
        else
        {
            printf("DEBUG: Matrix is NOT symmetric.\n");
        }
    }

    printf("Elapsed: %.6f s\n", elapsed);

    free(M);

    return 0;
}
