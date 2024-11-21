#ifndef __FUNC_H__
#define __FUNC_H__

bool checkSym(const int N, const float M[N][N]){
  bool sym = false;
  // I am checking the matrix symmetrically to reduce the iterations to N^2/4 
  // this might be useful for explicit parallelization as well
  for(int i=0; i < N / 2 + 1; i++){
    for(int j=0; j < N / 2 + 1; j++){
      // avoid if to remove N^2 conditions
      sym = M[i][j] != M[j][i] || M[N - i - 1][N - j - 1] != M[j][i] || sym;
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


#endif

