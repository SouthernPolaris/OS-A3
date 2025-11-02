#!/bin/bash

# Exit on error
set -e

# Compile the mergesort program
make all
echo "Successfully compiled"

sizes=(2 4 10 1000 1000000)
cutoffs=(0 1 3 6 8 10)
seed=10

# File to store all outputs
OUTPUT_FILE="test_output.log"

# Clear previous output file
: > "$OUTPUT_FILE"

# Iterate through sizes and cutoffs
for n in "${sizes[@]}"; do
    
    for c in "${cutoffs[@]}"; do
        echo "Running test-mergesort with size=$n, cutoff=$c, seed=$seed"

        # Temporary file to hold output
        temp_file=$(mktemp)

        # Run the test-mergesort program and write output to temp file
        ./test-mergesort "$n" "$c" "$seed" >> "$temp_file" 2>> "$OUTPUT_FILE"
        # Append the output to the main output file
        echo "Test with size=$n, cutoff=$c" >> "$OUTPUT_FILE"
        cat "$temp_file" >> "$OUTPUT_FILE"
        echo "---------------------------------" >> "$OUTPUT_FILE"

        # Check if output is sorted
        if sort -n -c "$temp_file"; then
            echo "Test passed for size=$n, cutoff=$c"
        else
            echo "Test failed for size=$n, cutoff=$c"
        fi

        echo "---------------------------------"

    done
done

echo "All tests completed"

# Check if valgrind is installed
if !command -v valgrind &> /dev/null; then
    echo "Valgrind not found, skipping memory leak check"
    exit 0
fi

# Check for memory leaks using valgrind
echo "Memory leak check with valgrind"

# Run valgrind on a sample test (no output to keep terminal clean)
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./test-mergesort 1000 5 10 > /dev/null 2>&1
echo "----------------------"
# Check valgrind exit status
if [ $? -eq 0 ]; then
    echo "Valgrind check passed"
else
    echo "Valgrind detected memory leaks"
fi