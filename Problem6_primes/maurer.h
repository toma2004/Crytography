/*
 * maurer.h
 *
 *  Created on: Apr 11, 2016
 *      Author: ntran
 */

#ifndef MAURER_H_
#define MAURER_H_


#include "rndsearch.h"

/*
 * Piece of code below is copied from Professor's HW6 website
 */
#define round(X) (((X) >= 0) ? (int)((X) + 0.5) : (int)((X) - 0.5))
/*
 * End of code borrowed
 */


/*
 * Supplemental function to act as RndOddNum wrapper
 * @param: k, and FILE pointer to fp_rndfile
 * @return: BIGNUM *bn_n
 */
BIGNUM *RndOddNum_wrapper(int k, FILE *fp_rndfile);

/*
 * Supplemental function to read a byte from rndfile
 * @param: FILE pointer to fp_rndfile
 * @return: unsigned char of byte read
 */
unsigned char *RndByte(FILE *fp_rndfile);

/*
 * Supplemental function to calculate gcd(x,y)
 * @param: BIGNUM *bn_x, BIGNUM *bn_y
 * @return: gcd(x,y)
 */
BIGNUM *gcd(BIGNUM *bn_x, BIGNUM *bn_y);

/*
 * Supplemental function to calculate BIGNUM * I = floor(2^(k-2) /q)
 * @param: int (k-2), BIGNUM *q
 * @return: BIGNUM *I
 */
BIGNUM *calculate_I(int k2, BIGNUM *q);

/*
 * Supplemental function to calculate BIGNUM *R = (R mod I) + I + 1
 * @param: BIGNUM *R and *I
 * @return: BIGNUM *R
 */
BIGNUM *calculate_R(BIGNUM *R, BIGNUM *I);


/*
 * Supplemental function to calculate n = 2*R*q + 1
 * @param: BIGNUM *R, *q
 * @return: BIGNUM *n
 */
BIGNUM *calculate_n(BIGNUM *R, BIGNUM *q);

/*
 * Supplemental function to help compare if BIGNUM *a <= 1 or *a >= n-1
 * @param: BIGNUM *a, and *n
 * @return: 0 if false, 1 if true
 */
int cmp_a(BIGNUM *a, BIGNUM *bn_n);

/*
 * Supplemental function to calculate b where b = a^{n-1} mod n
 * @param: BIGNUM *a and *bn_n
 * @return: BIGNUM *b
 */
BIGNUM *calculate_b(BIGNUM *a, BIGNUM *bn_n);

/*
 * Supplemental function to calculate b where b = a^{2R} mod n
 * @param: BIGNUM *a and *bn_n, *R
 * @return: BIGNUM *b
 */
BIGNUM *calculate_b_1(BIGNUM *a, BIGNUM *bn_n, BIGNUM *R);

/*
 * Supplemental function to calculate b-1
 * @param: BIGNUM *b
 * @reutrn: BIGNUM *b-1
 */
BIGNUM *sub_1(BIGNUM *b);

/*
 * Supplemental function to convert binary into decimal to print
 * @param: unsigned char* point to the binary
 * @return: decimal representation of the given binary
 */
uint32_t bin_2_dec(unsigned char* bin);


/*
 * Maurer algorithm
 * @param: k numbits, level to print to stdout, FILE pointer to primesfile, FILE pointer to rndfile
 * @output: generate and print numbits-bit provable prime using Maurer algorithm
 */
BIGNUM* maurer(int k, int level, FILE *fp, FILE *fp_rndfile);

/*
 * Supplemental function to wrap around Maurer function
 * @param: k,  FILE pointer to primesfile, FILE pointer to rndfile
 * @output: print Maurer processes to stdout
 */
void maurer_wrapper(int k, FILE *fp, FILE *fp_rndfile);


#endif /* MAURER_H_ */
