## OS Assignment 3 Group 100

* Authors: Asrith Atluri (a1889440), Lalisa Thaiprasert (a1900119), Elke Milne (a1885485)
* Group name: Group 100

## Overview

This program performs mergesort in a parallel manner, by creating new threads to sort each subarray of the mergesort algorithm. This calls parallel mergesort by recursively creating threads until the specified maximum level is reached or max thread count is exceeded. It then merges all these parallel sorting calls into a fully sorted array recursively.

## Manifest

A listing of source files and other non-generated files, and a brief
(one-line) explanation of the purpose of each file.

This project contains a header and an implementation file for the main parallel mergesort algorithm, a C file for manually running the mergesort, a Makefile to build the project, and a bash script to the program.

- `mergesort.h`: Header file for the parallel mergesort. Contains the argument struct and function definitions.
- `mergesort.c`: Implements the parallel mergesort algorithm.
- `test-mergesort.c`: Enables the user to run the parallel mergesort algorithm through terminal input.
- `Makefile`: Compiles the project into a single executable.
- `mergesort_tests.sh`: Runs sorting tests and Valgrind memory leak checks for `pthread`.

## Building the project

1. Clean the Project
```bash
make clean
```

2. Compile the Project
```bash
make all
```

An executable file `test-mergesort` should be in the project root directory ready for usage.

3. [Optional] (Recommended for Testing)
```
sudo apt install valgrind
```
- The test bash script uses Valgrind to check for memory leaks. If Valgrind is not installed on the system, it will skip this test, but for full test coverage, please ensure Valgrind is installed.


## Features and usage

This program implements both non-parallel and parallel merge sort using the POSIX threads (pthreads) library to demonstrate inter-thread synchronization and parellelism. 

### Key Features:
- Implements standard mergesort as a baseline algorithm 
- Implements parallel mergesort that dynamically creates threads based on a user-specified cutoff level. 
- Uses a global mutex and thread counter to safely manage concurrent thread creation
- Performs stable merging of sorted subarrays using a secondary buffer array 
- Supports command-line arguments for `array size`, recursion `cut off` depth, and random `seed` for reproducible testing. 
- Provides performance timing output to measure speedup as the cutoff increases. 
- Includes a comprehensive test script for automated benchmarking and memory validation using Valgrind. 

### Usage 
To compile and run: 
```
make clean
make all
./test-mergesort <array_size> <cutoff_level> <seed>
```
Examples:
```
# Non-parallel mergesort (no threading)
./test-mergesort 1000000 0 42

# Parallel mergesort up to 3 levels of threading
./test-mergesort 1000000 3 42
```
The output will display the time taken to sort the array:
```
Sorting 1000000 elements took 2.15 seconds.
```
As the cutoff level increases, the program recursively creates more threads up to the limit, leading to improved performance on multi-core processors.

## Testing

A bash script was created that runs this parallel mergesort on varying values of input size and cutoff level. This was then compared to a regular sorting algorithm provided by the bash terminal. The seed was kept the same across all tests for consistent testing.

Additionally, Valgrind was used to test for memory leaks, especially in the creation and deletion of threads. This test is skipped if Valgrind is not installed.

This process validates:
- Correctness: The array is sorted in ascending order for all tested configurations.
- Performance: Increasing the cutoff level results in measurable speedup compared to the non-parallel implementation.
- Memory Safety: No memory leaks or invalid accesses occur when threads are created, joined, or freed.

```bash 
./mergesort_tests.sh
```
Output summary is saved in `test_output.txt`.

## Reflection and Self Assessment

During development, several issues were encountered related to thread synchronisation and pointer management.

### Challenges and Learning Points:

#### Thread Argument Passing:
Initially, threads shared the same struct argument, leading to race conditions. The fix was to dynamically allocate memory for each argument using malloc() inside buildArgs() and free it after the thread completed.

#### Thread Explosion / Resource Limit:
Early versions spawned too many threads, causing system slowdown. To resolve this, a global thread counter with a mutex was introduced to cap the number of active threads and ensure safe concurrent updates.

#### Deadlocks and Incorrect Joins:
Some tests initially hung because pthread_join() was being called incorrectly or on uninitialised thread handles. The logic was corrected to only join successfully created threads.

#### Performance Tuning:
Through experimentation, the team learned that excessive thread creation at deep recursion levels leads to diminishing returns due to context-switch overhead. Adjusting the cutoff parameter provided a balance between parallel speedup and system stability.

#### Valgrind Debugging:
Running Valgrind helped identify a few unfreed memory allocations from buildArgs(). Adding explicit free(arg) after thread joins fixed all memory leaks.

#### Other Things We Leanred

Understanding that parallelism is not free threads must be carefully managed to avoid oversubscription and overhead that can actually slow down the program.

Realising the importance of synchronisation and data sharing safety when multiple threads operate on overlapping memory.

#### Overall Assessment:

The development process was iterative but productive. Each debugging session led to a deeper understanding of thread creation, joining, and synchronisation.

The final implementation met the performance goal (achieving significant exponential increased performance compared to standard mergesort on large arrays).

The testing framework and Valgrind analysis confirmed both correctness and memory safety.

## Sources Used

lhunath 2009, *How can I check if a program exists from a Bash script?*, StackOverflow, 24 March, viewed 2 November 2025, <https://stackoverflow.com/questions/592620/how-can-i-check-if-a-program-exists-from-a-bash-script>

user7851115 2017, *How do I use valgrind to find memory leaks?*, StackOverflow, 8 July, viewed 2 November 2025, <https://stackoverflow.com/questions/5134891/how-do-i-use-valgrind-to-find-memory-leaks>