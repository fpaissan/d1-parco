#ifndef __FUNC_H__
#define __FUNC_H__

#include<stdbool.h>
#ifdef _OPENMP
#include<omp.h>
#endif

bool checkSym(float* M, const long N)
{
    /* BENCHMARKING NOTES:
     * To fairly benchmark this function, we need
     * M to be a symmetric matrix. Otherwise, the kernel
     * might exist before checking all the combinations. */
    for(int i=0; i < N / 2; i++)
    {
        for(int j=0; j < i; j++)
        {
            // avoid if to remove N^2 conditions
            if(M[i*N + j] != M[j* N + i]) return false;
        }
    }

    return true;
}

bool checkSymImp(float* M, const long N)
{
    bool sym = true;
#pragma GCC unroll 2 // Suggest unrolling for improved performance
    for (int i = 0; i < N / 2; i=i+2)
    {
// #pragma simd
// #pragma ivdep // Hint: ignore dependencies for vectorization
#pragma GCC unroll 2 // Suggest unrolling for improved performance
        for (int j = 0; j < i; j=j+2)
        {
            // __builtin_prefetch(&M[i * N + j + 1], 0, 1);
            // __builtin_prefetch(&M[j * N + i + 1], 0, 1);

            // Compare symmetric elements
            if (M[i * N + j] != M[j * N + i] || M[(i+1) * N + j + 1] != M[(j + 1) * N + (i+1)] )
                sym = false;
        }
    }

    return sym;
}

bool checkSymOMP(float* M, const long N)
{
    bool sym = true;
    #pragma omp parallel for collapse(2)
    for(int i=0; i < N / 2; i++)
    {
        for(int j=0; j < i; j++)
        {
            // avoid if to remove N^2 conditions
            if(M[i*N + j] != M[j* N + i])
            {
                #pragma omp critical
                {
                    sym = false;
                }
            }
        }
    }

    return sym;
}

void matTranspose(float* M, float* T, const long N)
{
    for(int j=0; j < N; j++)
    {
        for(int i=0; i < N; i++)
        {
            T[i*N + j] = M[j*N + i];
        }
    }
}

void matTransposeImp(float* M, float* T, const long N)
{
    #pragma ivdep
    #pragma unroll 2
    for(int j=0; j < N; j=j+2)
    {
        #pragma ivdep
        #pragma unroll 2
        for(int i=0; i < N; i=i+2)
        {
            T[i*N + j] = M[j*N + i];
            T[(i+1)*N + j] = M[j*N + (i+1)];
            T[(i+1)*N + j+1] = M[(j+1)*N + (i+1)];
            T[i*N + j+1] = M[(j+1)*N + i];
        }
    }
}

void matTranspose_CO(float* matrix, float* result,
                     long size, long row_start, long row_end,
                     long col_start, long col_end)
{
    int row_size = row_end - row_start;
    int col_size = col_end - col_start;

    // Base case: small enough to transpose directly
    if (row_size <= 32 && col_size <= 32)
    {
        for (int i = row_start; i < row_end; i++)
        {
            for (int j = col_start; j < col_end; j++)
            {
                result[j * size + i] = matrix[i * size + j];
            }
        }
        return;
    }

    // Recursive case: split into quadrants
    int row_mid = (row_start + row_end) / 2;
    int col_mid = (col_start + col_end) / 2;

    // Transpose each quadrant recursively
    matTranspose_CO(matrix, result, size, row_start, row_mid, col_start, col_mid);
    matTranspose_CO(matrix, result, size, row_start, row_mid, col_mid, col_end);
    matTranspose_CO(matrix, result, size, row_mid, row_end, col_start, col_mid);
    matTranspose_CO(matrix, result, size, row_mid, row_end, col_mid, col_end);
}

void matTransposeTiled(float* matrix, float* result,
                       const long size, const int tile_size)
{
    // First process diagonal tiles
    for (int i = 0; i < size; i += tile_size)
    {
        // Process diagonal tile; separate to avoid conflict with other tiles
        for (int ti = i; ti < i + tile_size && ti < size; ti++)
        {
            for (int tj = i; tj < i + tile_size && tj < size; tj++)
            {
                result[tj * size + ti] = matrix[ti * size + tj];
            }
        }

        // Process pairs of off-diagonal tiles
        for (int j = i + tile_size; j < size; j += tile_size)
        {
            // Process tile (i,j) and (j,i) in parallel to maximize cache usage
            for (int ti = i; ti < i + tile_size && ti < size; ti++)
            {
                for (int tj = j; tj < j + tile_size && tj < size; tj++)
                {
                    // Transpose element (i,j)
                    result[tj * size + ti] = matrix[ti * size + tj];
                    // Transpose element (j,i)
                    result[ti * size + tj] = matrix[tj * size + ti];
                }
            }
        }
    }
}

void matTransposeOMP(float* M, float* T, const long N)
{
    #pragma omp parallel for collapse(2)
    for(int j=0; j < N; j++)
    {
        for(int i=0; i < N; i++)
        {
            T[i*N + j] = M[j*N + i];
        }
    }
}

void matTransposeTiledOMP(float* matrix, float* result,
                          const long size, const int tile_size)
{
    // First process diagonal tiles
    #pragma omp parallel for
    for (int i = 0; i < size; i += tile_size)
    {
        // Process diagonal tile; separate to avoid conflict with other tiles
        for (int ti = i; ti < i + tile_size && ti < size; ti++)
        {
            for (int tj = i; tj < i + tile_size && tj < size; tj++)
            {
                result[tj * size + ti] = matrix[ti * size + tj];
            }
        }

        // Process pairs of off-diagonal tiles
        for (int j = i + tile_size; j < size; j += tile_size)
        {
            // Process tile (i,j) and (j,i) in parallel to maximize cache usage
            for (int ti = i; ti < i + tile_size && ti < size; ti++)
            {
                for (int tj = j; tj < j + tile_size && tj < size; tj++)
                {
                    // Transpose element (i,j)
                    result[tj * size + ti] = matrix[ti * size + tj];
                    // Transpose element (j,i)
                    result[ti * size + tj] = matrix[tj * size + ti];
                }
            }
        }
    }
}

#endif

