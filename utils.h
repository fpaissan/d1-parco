/*
 * Author: Francesco Paissan, 2024
 *
 */
#ifndef __UTILS_H__
#define __UTILS_H__

#include<string.h>
#include<stdint.h>

bool isPowerTwo(long N) {
    return (N != 0) && ((N & (N - 1)) == 0);
}

typedef enum {
    SEQUENTIAL, IMPLICIT, OMP, TILED, TILEDOMP, CO
} strat;

typedef struct {
    const bool DEBUG; // handles printing schemes
    const long N; // size of the initialized matrices
    const strat STRAT;
} ExpSetup;


ExpSetup parse_setup(int argc, char*argv[]) {
    char *endptr; // to check for conversion errors
    const long N = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "Invalid number: %s\n", argv[1]);
        exit(1);
    } else if (!isPowerTwo(N)) {
        fprintf(stderr, "Number should be a power of two, and is now: %s\n", argv[1]);
        exit(1);
    }

    const char *strategy = NULL;
    for (int i = 1; i < argc; i++) {
        // Handle "--strategy=value" format
        if (strncmp(argv[i], "--strategy=", 11) == 0) {
            strategy = argv[i] + 11; // Get the value part
        }
        // Handle "--strategy value" format
        else if (strcmp(argv[i], "--strategy") == 0 && i + 1 < argc) {
            strategy = argv[i + 1];
            i++; // Skip the value argument
        }
    }
    strat S;

    if (strcmp(strategy, "IMP") == 0) {
        S = IMPLICIT;
    } else if (strcmp(strategy, "OMP") == 0) {
        S = OMP;
    } else if (strcmp(strategy, "SEQ") == 0) {
        S = SEQUENTIAL;
    } else if (strcmp(strategy, "TILED") == 0) {
        S = TILED;
    } else if (strcmp(strategy, "TILEDOMP") == 0) {
        S = TILEDOMP;
    } else if (strcmp(strategy, "CO") == 0) {
        S = CO;
    } else {
        fprintf(stderr, "Arguments are messed up. Strategy can be IMP / OMP / SEQ for SymCheck and IMP / OMP / SEQ / TILED / TILEDOMP / CO for Tranpose.\n");
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

    return (ExpSetup) {
        .DEBUG = debug, .N = N, .STRAT = S
    };
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

void generate_uniform_random(float* matrix, const long N) {
    // uniform init on float range
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            matrix[i * N + j] = (float) rand() / (RAND_MAX) * (FLT_MAX - FLT_MIN) + FLT_MIN;
        }
    }
}

bool checkTranspose(float *A, float* B, const long N) {
    // Verify the result
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (B[j * N + i] != A[i * N + j]) {
                return false;
            }
        }
    }

    return true;
}

void check_alignment(void *ptr, size_t alignment) {
    if ((uintptr_t)ptr % alignment == 0) {
        printf("Pointer is aligned to %zu bytes.\n", alignment);
    } else {
        printf("Pointer is NOT aligned to %zu bytes.\n", alignment);
    }
}

#endif // !__UTILS_H__
