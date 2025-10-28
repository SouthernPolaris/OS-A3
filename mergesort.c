/**
 * This file implements parallel mergesort.
 */

#include <stdio.h>
#include <string.h> /* for memcpy */
/* Hide libc's mergesort symbol name in this .c file only */
#define mergesort __dont_conflict_with_libc_mergesort
#include <stdlib.h> /* for malloc */
#undef mergesort

#include "mergesort.h"
#include <pthread.h>


/* serial mergesort: this function will be called by mergesort() and also by parallel_mergesort(). */
void merge(int leftstart, int leftend, int rightstart, int rightend){
    // Assumes A[leftstart..leftend] and A[rightstart..rightend] are already sorted.
	int i, j, k;
	i = leftstart;
	j = rightstart;
	k = leftstart;

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

	/* Copy merged range back to A (copy from B[leftstart] */
    size_t count = (size_t)(rightend - leftstart + 1);
    memcpy(&A[leftstart], &B[leftstart], count * sizeof(int));
}

/* serial mergesort (recursive) */
void mergesort(int left, int right) {
	if (left >= right) return;
	int mid = left + (right - left) / 2;
	mergesort(left, mid);
	mergesort(mid+1, right); 
	merge(left, mid, mid + 1, right); 

}

/* this function will be called by parallel_mergesort() as its base case. */
void my_mergesort(int left, int right){
	mergesort(left, right); 
}

/* this function will be called by the testing program. */
void * parallel_mergesort(void *arg){
	struct argument *args = (struct argument *)arg;
	if (!args) return NULL; 

	int left = args->left;
	int right = args->right;
	int level = args->level;

	if (left >= right) return NULL;

	// Base case: reached cutoff depth -> use serial mergesort
	if (level >= cutoff) {
		mergesort(left, right);
		return NULL;
	}

	int mid = left + (right - left) / 2;

	// Prepare child args 
	struct argument *leftArg = buildArgs(left, mid, level + 1);
	struct argument *rightArg = buildArgs(mid + 1, right, level + 1);

	pthread_t tL, tR; 

	// Create two threads fo rhte two halves 
	// If creation fails, fall back to serial on that half 
	if (leftArg && pthread_create(&tL, NULL, parallel_mergesort, leftArg) != 0) {
		// fallback serial 
		if (leftArg) free (leftArg);
		mergesort(left, mid);
		tL = (pthread_t)0; // mark as "no thread" 
	}

	if (rightArg && pthread_create(&tR, NULL, parallel_mergesort, rightArg) != 0) {
        
        if (rightArg) free(rightArg);
        mergesort(mid + 1, right);
        tR = (pthread_t)0;
    }

    // Join any threads we successfully created; also free child args we allocated
    if ((unsigned long)tL != 0UL) {
        pthread_join(tL, NULL);
        free(leftArg);
    }
    if ((unsigned long)tR != 0UL) {
        pthread_join(tR, NULL);
        free(rightArg);
    }

    // Merge the two sorted halves
    merge(left, mid, mid + 1, right);

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