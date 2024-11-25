#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define AVG_NUMBER 100
#define SIZE (unsigned long) pow(2, 20)

int main(){

  int *array = (int*)malloc(sizeof(int) * SIZE);
  if(array == NULL){
    printf("Memory allocation failed.\n");
    return 1;
  }

  double cpu_time_used;
  clock_t start, end;

  start = clock();
  for ( long i = 0; i < SIZE ; i++) {
    array[i] = i ;
  }

  for ( long i = 0; i < SIZE ; i++) {
    int value = array [ i ];
  }
  end = clock();

  cpu_time_used = (double)(end - start) / CLOCKS_PER_SEC;

  double moved = 2.0 * sizeof(int) * SIZE;
  double rate = moved / (cpu_time_used * 1e9);

  printf("It took %.6f s to move %f B.\n", cpu_time_used, moved);
  printf("The rate is %f GB/s.\n", rate);

}
