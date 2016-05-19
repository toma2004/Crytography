/*
 * x4.h
 *
 *  Created on: Apr 23, 2016
 *      Author: ntran
 */

#ifndef X4_H_
#define X4_H_

#include "x3.h"

struct mynk_x4{
	int n;
	int k;
	int isValid;
};

struct myBG{
	int B;
	int G;
};

struct mydata{
	unsigned char* data;
	int total_byte;
	int isValid;
};

/*
 * Supplemental function to read in bit stream and stored it internally for processing
 * @param: FILE pointer points file name
 * @return: struct mydata containing data and total byte count if valid
 */
struct mydata store_data(char *bitStream);


/*
 * Supplemental function to find k so that k (n-k+3)/2^k+1 >= 5
 * @param:char * points to file name of file containing bit stream
 * @return: n and k
 */
struct mynk_x4 calculate_nk(char *bitStream);

/*
 * Supplemental function to find ei
 * @param: n and i
 * @return: double ei = (n-i+3)/2^i+2
 */
double calculate_e(int n, int i);

/*
 * Supplemental funciton to calculate B and G for each i where 1 <= i <= k
 * @param: struct mydata stream and int i
 * @return: value of B and G
 */
struct myBG calculate_bg(struct mydata x4_data, int i);

/*
 * x4 - runs test
 * @param: File pointer points to bit stream
 * @output: print out x4 test process and result
 */
void x4(FILE *fp);

#endif /* X4_H_ */
