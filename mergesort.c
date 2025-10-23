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
		return NULL;
}

/* we build the argument for the parallel_mergesort function. */
struct argument * buildArgs(int left, int right, int level){
		return NULL;
}

