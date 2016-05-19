/*
 * rndsearch.h
 *
 *  Created on: Apr 10, 2016
 *      Author: ntran
 */

#ifndef RNDSEARCH_H_
#define RNDSEARCH_H_

#include "primes.h"
#include "millerrabin.h"


/*
 * Supplemental function to return BIGNUM containing value n = RndOddNum(k) algorithm
 * @param: k numbits and char pointer to binary read in using x = ceil(k/8) bytes
 * @return: BIGNUM n
 */
BIGNUM *RndOddNum(int k, char *temp, int x_byte);

/*
 * rndsearch function to generate numbits-bit probable prime
 * @param: k numbits, maxitr, FILE pointer to primesfile, FILE pointer to rndfile
 * @output: Print out rndsearch process and generate a probable prime
 */
void rndsearch(int k, int maxitr, FILE *fp, FILE *fp_rndfile);

#endif /* RNDSEARCH_H_ */
