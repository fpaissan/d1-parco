#!/bin/bash


compilation_flags=("O0" "O1" "O2" "O3")
strategies=("SEQ" "IMP" "OMP" "TILED" "TILEDOMP" "CO")

for strat in "${strategies[@]}"; do
    echo "Running $strat..."

    if [[ $strat = "OMP" || $strat = "TILEDOMP" ]]; then
      threads=(5 15 30 40)
    else
      threads=(1)
    fi

    for num_t in "${threads[@]}"; do
      for flag in "${compilation_flags[@]}"; do
          echo "Testing with flag: $flag"
          
          # Create experiment folder
          mkdir -p results/tra_results_$flag
          mkdir -p logs
      
          gcc -o tra -$flag -fopt-info-optimized="logs/${flag}_${strat}_tra.log" -fopenmp -march=native main_tra.c
          echo gcc -o tra -$flag -fopt-info-optimized="logs/${flag}_${strat}_tra.log" -march=native -fopenmp main_tra.c
          
          # Loop through the powers of 2 from 2^4 to 2^12, as per requirements
          for i in $(seq 4 12); do
              power_of_two=$((2**i))  # Calculate 2^i
              echo "Running command for 2^$i = $power_of_two"
          
              output_file="results/tra_results_$flag/${strat}_${power_of_two}_t${num_t}.txt"
              OMP_NUM_THREADS=$num_t "./tra" $power_of_two --strategy=$strat > "$output_file"
          done
      done
    done
done
