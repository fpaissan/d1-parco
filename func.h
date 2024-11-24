#ifndef __FUNC_H__
#define __FUNC_H__

bool checkSym(float* M, const long N){
  /* BENCHMARKING NOTES: 
   * To fairly benchmark this function, we need
   * M to be a symmetric matrix. Otherwise, the kernel
   * might exist before checking all the combinations. */
  for(int i=0; i < N / 2; i++){
    for(int j=0; j < N / 2; j++){
      // avoid if to remove N^2 conditions
      if(M[i*N + j] != M[j* N + i]) return false;
    }
  }

  return true;
}

bool checkSymImp(float* M, const long N){
  /* BENCHMARKING NOTES: 
   * To fairly benchmark this function, we need
   * M to be a symmetric matrix. Otherwise, the kernel
   * might exist before checking all the combinations. */
  for(int j=0; j < N / 2; j++){
    for(int i=0; i < N / 2; i++){
      if(M[i*N + j] != M[j* N + i]) return false;
    }
  }

  return true;
}

void matTranspose_CO(float* matrix, float* result,
                       int size, int row_start, int row_end,
                       int col_start, int col_end) {
    int row_size = row_end - row_start;
    int col_size = col_end - col_start;
    
    // Base case: small enough to transpose directly
    if (row_size <= 32 && col_size <= 32) {
        for (int i = row_start; i < row_end; i++) {
            for (int j = col_start; j < col_end; j++) {
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


#endif

