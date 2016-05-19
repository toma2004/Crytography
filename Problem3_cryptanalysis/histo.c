/*
 * histo.c
 *
 *  Created on: Feb 17, 2016
 *      Author: ntran
 */

#include "histo.h"

/*
 * Merge function to compare the element and merge them into 1 array for mergesort
 */
void merge(struct histoElement *array, struct histoElement *subLeft, int countLeft, struct histoElement *subRight, int countRight)
{
	int arr_index = 0;
	int subLeft_index = 0;
	int subRight_index = 0;

	while(subLeft_index < countLeft && subRight_index < countRight)
	{
		if (subLeft[subLeft_index].count > subRight[subRight_index].count)
		{
			array[arr_index] = subLeft[subLeft_index];
			subLeft_index += 1;
		}
		else if (subLeft[subLeft_index].count == subRight[subRight_index].count)
		{
			if (subLeft[subLeft_index].index < subRight[subRight_index].index) //the smaller the index, the bigger the character in the alphabet
			{
				array[arr_index] = subLeft[subLeft_index];
				subLeft_index += 1;
			}
			else
			{
				array[arr_index] = subRight[subRight_index];
				subRight_index += 1;
			}
		}
		else
		{
			array[arr_index] = subRight[subRight_index];
			subRight_index += 1;
		}
		arr_index += 1;
	}

	//copy leftover elements
	while(subLeft_index < countLeft)
	{
		array[arr_index] = subLeft[subLeft_index];
		arr_index += 1;
		subLeft_index += 1;
	}
	while(subRight_index < countRight)
	{
		array[arr_index] = subRight[subRight_index];
		arr_index += 1;
		subRight_index += 1;
	}
}

/*
 * Modified Merge sort to sort the array of struct hisoElement
 * @param: array of struct histoElement to be sorted, and total number of elements in the array
 * @output: sorted array based on count, if count is the same, based on index
 */
void mergesort(struct histoElement *array, int total)
{
	int mid;
	if(total < 2) return;

	mid = total / 2;
	struct histoElement *subLeft = malloc(mid * sizeof(struct histoElement));
	struct histoElement *subRight = malloc((total-mid) * sizeof(struct histoElement));

	//now copy element by element from original array into 2 sub arrays
	for(int i = 0; i < mid; i++)
	{
		subLeft[i] = array[i];
	}

	for(int i = mid; i <  total; i++)
	{
		subRight[i-mid] = array[i];
	}

	//recursive function to keep dividing the array into half
	mergesort(subLeft,mid);
	mergesort(subRight,total-mid);
	//Call merge function to merge 2 array into the original array sorted based on count and if count is the same, based on index
	merge(array,subLeft,mid,subRight,total-mid);

	free(subLeft);
	free(subRight);
}

/*
 * Function to generate the histogram of ciphertext based on which and period argument
 * @param: period, which, and file pointer to stdin or file input
 * @output: histogram of cipher text
 */
void histo(int period, int which, FILE *fp)
{
	char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
	struct histoElement *result = malloc(26 * sizeof(struct histoElement));
	//initialize result array of struct histoElement
	int i = 0;
	while(i < 26)
	{
		(result+i)->count = 0;
		(result+i)->index = i;
		i += 1;
	}

	char *mychar = malloc(1);
	int byte = 0;
	int count = 0;
	int L = 0; //count total valid character given which and period values
	while((byte = fread(mychar,1,1,fp)) > 0)
	{
		//check if malform input file
		if (*mychar != '\n' && (*mychar < 0x20 || *mychar > 0x7e))
		{
			fprintf(stderr, "ERROR: detect undefined character in input file\n");
			free(mychar);
			free(result);
			return;
		}
		if (count == (which - 1))
		{
			//check if char is in range a-z
			if (*mychar >= 'a' && *mychar <= 'z')
			{
				(result+((*mychar - 71) % 26))->count += 1;
				L += 1;
				//printf("my element index = %d, mychar = %c\n", (result+((*mychar - 71) % 26))->index, *mychar);
			}
			//fwrite(mychar,1,1,stdout);
			which += period;
		}
		count += 1;
	}

	//Now need to sort the result array of struct based on count. If element has the same count, we sort index.
	mergesort(result,26);

	i = 0;
	//print total valid character given which and period values
	fprintf(stdout,"L=%d\n", L);
	while(i < 26)
	{
		fprintf(stdout,"%c: %d (%.2f%%)\n",alphabet[(result+i)->index], (result+i)->count, (float)(result+i)->count/L * 100);
		i += 1;
	}

	free(mychar);
	free(result);
}
