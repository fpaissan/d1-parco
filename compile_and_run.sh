#!/bin/bash
gcc -o sym \
  -fopt-info-optimized -fopt-info-all -fverbose-asm \
  -march=native \
  -O1 \
  main_sym.c

