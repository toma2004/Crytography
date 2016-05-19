/*
 * millerrabin.h
 *
 *  Created on: Apr 6, 2016
 *      Author: ntran
 */

#ifndef MILLERRABIN_H_
#define MILLERRABIN_H_

#include "trialdiv.h"

struct myIntInput
{
	int isValid;
	int intInput;
};

struct mysr{
	int isValid;
	int s;
	BIGNUM *r;
	BIGNUM *nsub1;
};

/*
 * Supplemental function to check and store maxitr input
 * @param: char pointer to maxitr
 * @output: struct myIntInput containing maxitr and isValid = 1 if input maxitr is valid, else isValid = 0
 */
struct myIntInput check_and_store_maxitr(char *inputMaxItr);

/*
 * convert into to char pointer
 * @param: int n
 * @output: char *
 */
char *itoa(int n);

/*
 * Supplemental function to find 2 and r in formula: n-1 = (2^s)*r such that r is odd
 * @param: BIGNUM *n
 * @output: struct mysr with int s and BIGNUM *r
 */
struct mysr find_s_r(BIGNUM *n);

/*
 * Miller-Rabin function to test the primality of number
 * @param: numer and maxitr, and FILE fp pointing to primesfile, and optional print space
 * @output: return 1 if n is prime, else return 0 for other cases. Report error by returning -1
 * Return message if number is composite or prime. Report if failure occurs
 */
int millerrabin(BIGNUM *n, int maxitr, FILE *fp, int spaces);

#endif /* MILLERRABIN_H_ */
