/*
 * trialdiv.h
 *
 *  Created on: Apr 4, 2016
 *      Author: ntran
 */

#ifndef TRIALDIV_H_
#define TRIALDIV_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <netinet/in.h>
#include <openssl/bn.h>

struct myNum{
	int isValid;
	BIGNUM *bn_n;
};

/*
 * Supplemental function to read in number, verify if it is valid, and store it internally as BIGNUM type
 * @param: char pointer to number from args
 * @output: struct myNum where isValid=1 and BIGNUM containing the number when num is valid; else isValid = 0
 */
struct myNum read_verify_store_number(char *num);

/*
 * Supplemental function to find floor(sqrt(BIGNUM number)) using brute force (try to square all number from 2 to when we have result >= BIGNUM number
 * @param: BIGNUM *mynum
 * @output: BIGNUM *numsqrt
 */
BIGNUM *floor_sqrt_bn(BIGNUM *myNum);

/*
 * Version 2 of floor_sqrt_bn function using binary search to improve the run time
 * @param: BIGNUM *mynum
 * @output: floor(sqrt(mynum)
 */
BIGNUM *floor_sqrt_bn_ver2(BIGNUM *myNum);

/*
 * Perform trialdiv to determine if a given num is proved to be prime
 * @param: BIGNUM pointer points to bn_n containing given num and FILE pointer points to primesfile, optional print space
 * @output: 3 scenarios:
 * 1/ n passes trial division test
 * 2/ n passes trial division test (not enough primes)
 * 3/ n is composite by trial division (mod m = 0)
 * return 0 if failure, else return 1. Return -1 in case if error
 */
int trialdiv(BIGNUM *myNum, FILE *fp, int spaces);

#endif /* TRIALDIV_H_ */
