### Effective Memory Bandwidth

This code can be used to measure the effective memory bandwidth of a computing system. It moves a fixed amount of data around, and measures how much time it took to complete the task.

Depending on the target system, you can tune the amount of data modifying line 6: 

```
#define SIZE (unsigned long) pow(2, 20)
```

and the number of averages taken to reduce noise in measurements

```c
#define AVG_NUMBER 1000
```

The code was tested after compile via

```bash
gcc -o bench -O3 -march=native mem_bandwidth.c 
```
