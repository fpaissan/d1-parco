#!/bin/bash
# Author: Francesco Paissan
#
gcc -o sym \
  -fopt-info-optimized -fopt-info-all -fverbose-asm \
  -march=native \
  -O1 \
  main_sym.c

