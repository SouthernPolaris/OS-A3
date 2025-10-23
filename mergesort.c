/**
 * This file implements parallel mergesort.
 */

#include <stdio.h>
#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include "mergesort.h"

/* this function will be called by mergesort() and also by parallel_mergesort(). */
void merge(int leftstart, int leftend, int rightstart, int rightend){

	int i, j, k;
	i = leftstart;
	j = rightstart;
	k = 0;

	// Merge elements into B in sorted order
	while(i <= leftend && j <= rightend){
		if(A[i] <= A[j]){
			B[k++] = A[i++];
		} else {
			B[k++] = A[j++];
		}
	}

	while(i <= leftend){
		B[k++] = A[i++];
	}

	while(j <= rightend){
		B[k++] = A[j++];
	}

	for(i = leftstart, j = 0; i <= rightend; i++, j++){
		A[i] = B[j];
	}
}

/* this function will be called by parallel_mergesort() as its base case. */
void my_mergesort(int left, int right){
	if (left > right) {
		return;
	}

	int mid = (left + right) / 2;

	my_mergesort(left, mid);
	my_mergesort(mid + 1, right);

	merge(left, mid, mid + 1, right);
}

/* this function will be called by the testing program. */
void * parallel_mergesort(void *arg){
	struct argument *args = (struct argument *)arg;
	int left = args->left;
	int right = args->right;
	int level = args->level;

	// free argument list 
	free(args); 

	if (left > right) {
		if (level >= cutoff) { 
			// base case: use serial my_mergesort
			my_mergesort(left, right);
		} else {
			// divide the array and create threads 
			int mid = left + (right - left) / 2;

			// build arguments for left and right subarrays
			struct argument *leftArg = buildArgs(left, mid, level + 1);
			struct argument *rightArg = buildArgs(mid + 1, right, level + 1);

			// create threads for left and right subarrays
			pthread_t leftThread, rightThread;

			int leftCreateStat = pthread_create(&leftThread, NULL, parallel_mergesort, (void *)leftArg);
			int rightCreateStat = pthread_create(&rightThread, NULL, parallel_mergesort, (void *)rightArg);

			// hand thread creation failure 
			if (leftCreateStat || rightCreateStat) {
				fprintf(stderr, "Error creating threads\n");
				exit(1);
			}

			// wait for both threads to finish
			pthread_join(leftThread, NULL);
			pthread_join(rightThread, NULL);

			// merge the sorted subarrays
			merge(left, mid, mid + 1, right);

		}
	}

	return NULL; 
	
}

/* we build the argument for the parallel_mergesort function. */
struct argument * buildArgs(int left, int right, int level){
	struct argument *arg = (struct argument *)malloc(sizeof(struct argument));
	if (!arg) {
		// handle memory allocation failure
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}

	// set the fields of the argument struct
	arg->left = left;
	arg->right = right;
	arg->level = level;
	return arg;
}