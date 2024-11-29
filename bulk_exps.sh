#!/bin/bash


compilation_flags=("O0" "O1" "O2" "O3")
strategies=("SEQ" "IMP" "OMP")

for strat in "${strategies[@]}"; do
    echo "Running $strat..."

    for flag in "${compilation_flags[@]}"; do
        echo "Testing with flag: $flag"
        
        # Create experiment folder
        mkdir -p symm_results_$flag
    
        gcc -o sym -$flag -fopt-info-optimized="${flag}_${strat}_sym.log" -fopenmp -march=native main_sym.c
        echo gcc -o sym -$flag -fopt-info-optimized="${flag}_${strat}_sym.log" -march=native -fopenmp main_sym.c
        
        # Loop through the powers of 2 from 2^4 to 2^12, as per requirements
        for i in $(seq 4 12); do
            power_of_two=$((2**i))  # Calculate 2^i
            echo "Running command for 2^$i = $power_of_two"
        
            output_file="symm_results_$flag/strat_${power_of_two}.txt"
            "./sym" $power_of_two --strategy=$strat > "$output_file"
        done
    done
done
