## OS Assignment 3 Group 100

* Authors: Asrith Atluri, Lalisa Thaiprasert, Elke Milne
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

Summarise the main features of your program. It is also appropriate to
instruct the user how to use your program.

## Testing

A bash script was created that runs this parallel mergesort on varying values of input size and cutoff level. This was then compared to a regular sorting algorithm provided by the bash terminal. The seed was kept the same across all tests for consistent testing.

Additionally, Valgrind was used to test for memory leaks, especially in the creation and deletion of threads. This test if skipped if Valgrind is not installed

## Reflection and Self Assessment

Discuss the issues you encountered during development and testing. What
problems did you have? What did you have to research and learn on your own?
What kinds of errors did you get? How did you fix them?

What parts of the project did you find challenging? Is there anything that
finally "clicked" for you in the process of working on this project? How well
did the development and testing process go for you?

## Sources Used

https://stackoverflow.com/questions/592620/how-can-i-check-if-a-program-exists-from-a-bash-script
https://stackoverflow.com/questions/5134891/how-do-i-use-valgrind-to-find-memory-leaks