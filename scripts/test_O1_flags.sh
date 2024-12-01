#!/bin/bash
# Author: Francesco Paissan

# List of flags to test
FLAGS=(
  -fauto-inc-dec -fbranch-count-reg -fcombine-stack-adjustments -fcompare-elim -fcprop-registers -fdce -fdefer-pop
  -fdelayed-branch -fdse -fforward-propagate -fguess-branch-probability -fif-conversion -fif-conversion2
  -finline-functions-called-once -fipa-modref -fipa-profile -fipa-pure-const -fipa-reference -fipa-reference-addressable
  -fmerge-constants -fmove-loop-invariants -fomit-frame-pointer -freorder-blocks -fshrink-wrap -fshrink-wrap-separate
  -fsplit-wide-types -fssa-backprop -fssa-phiopt -ftree-bit-ccp -ftree-ccp -ftree-ch -ftree-coalesce-vars -ftree-copy-prop
  -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre -ftree-phiprop -ftree-pta -ftree-scev-cprop
  -ftree-sink -ftree-slsr -ftree-sra -ftree-ter -funit-at-a-time
)

# Source file
SOURCE_FILE="main_sym.c"

# Output directory for results
OUTPUT_DIR="./flag_test_results"
mkdir -p "$OUTPUT_DIR"

# Loop over each flag
for FLAG in "${FLAGS[@]}"; do
  # Create a unique directory for this flag
  FLAG_DIR="${OUTPUT_DIR}/${FLAG//[^a-zA-Z0-9_-]/_}" # Replace invalid characters with _
  mkdir -p "$FLAG_DIR"

  # Compile with the current flag
  echo "Compiling with flag: $FLAG"
  gcc -o "${FLAG_DIR}/program" $FLAG -march=native "$SOURCE_FILE" 2> "${FLAG_DIR}/compile.log"
  
  # Check if compilation was successful
  if [[ $? -eq 0 ]]; then
    echo "Running program for flag: $FLAG"
    # Run the compiled binary and save the output
    "${FLAG_DIR}/program" 4096 --strategy=SEQ > "${FLAG_DIR}/output.log" 2>&1
    cat "${FLAG_DIR}/output.log"
  else
    echo "Compilation failed for flag: $FLAG. Check ${FLAG_DIR}/compile.log for details."
  fi
done

echo "All tests completed. Results saved in $OUTPUT_DIR."

