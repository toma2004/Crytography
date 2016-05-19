/*
 * histo.h
 *
 *  Created on: Feb 17, 2016
 *      Author: ntran
 */

#ifndef HISTO_H_
#define HISTO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct histoElement {
	int count;
	int index;
};

/*
 * Function to generate the histogram of ciphertext based on which and period argument
 * @param: period, which, and file pointer to stdin or file input
 * @output: histogram of cipher text
 */
void histo(int period, int which, FILE *fp);

/*
 * Modified Merge sort to sort the array of struct hisoElement
 * @param: array of struct histoElement to be sorted, and total number of elements in the array
 * @output: sorted array based on count, if count is the same, based on index
 */
void mergesort(struct histoElement *array, int total);

/*
 * Merge function to compare the element and merge them into 1 array for mergesort
 */
void merge(struct histoElement *array, struct histoElement *subLeft, int countLeft, struct histoElement *subRight, int countRight);

#endif /* HISTO_H_ */
