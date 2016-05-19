/*
 * primes.h
 *
 *  Created on: Mar 30, 2016
 *      Author: ntran
 */

#ifndef PRIMES_H_
#define PRIMES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <netinet/in.h>

struct mymaxval{
	int isValid;
	uint32_t maxval;
};

/*
 * Supplemental function to check if a given maxval is valid.
 * @param: pointer to maxval char
 * @return:
 * If it is, return maxval in uint32_t format and isValid = 1, else return isValid = 0
 */
struct mymaxval check_and_return_maxval(char *maxval);

/*
 * Supplemental function to re-implement "ceil" function math.h as this is not defined on nunki
 * This function will round up a double to the nearest integer that is greater than or equal to given input
 * @param: input of type double, and int of that same number to compare against
 * @return: output of type int representing the round-up
 */
int ceil_func(double num, int d);

/*
 * print all primes number from 2 to maxval using Sieve of Eratosthenes algorithm
 * @param: maxval with conditin that maxval is in range 2-2^24
 * @output: print all prime numbers from 2 to maxval
 */
void primes(uint32_t maxval);

#endif /* PRIMES_H_ */
