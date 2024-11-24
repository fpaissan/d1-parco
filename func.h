#ifndef __FUNC_H__
#define __FUNC_H__

bool checkSym(float* M, const long N){
  for(int i=0; i < N / 2 + 1; i++){
    for(int j=0; j < N / 2 + 1; j++){
      // avoid if to remove N^2 conditions
      if(M[i*N + j] != M[j* N + i]) return false;
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

