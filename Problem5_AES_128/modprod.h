/*
 * modprod.h
 *
 *  Created on: Mar 13, 2016
 *      Author: ntran
 */

#ifndef MODPROD_H_
#define MODPROD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct mypoly{
	uint8_t poly1[4];
	uint8_t poly2[4];
	int isValid;
};

struct modprod_result{
	uint8_t modprod_poly[4];
};

/*
 * Supplemental function to check if the poly inputs are valid. If so, store it into proper array
 * @param: char pointer to poly 1 and poly 2
 * @output: valid = 1 if both poly inputs are valid, else valid = 0. If valid, poly values are stored in struct mypoly
 */
struct mypoly check_and_store_poly_input(char *poly1, char *poly2);

/*
 * supplemental function to perform xtime(b) operation
 * @param: 2 uint8_t number
 * @return: uint8_t number as a result of {xx} bigdot {yy}
 */
uint8_t xtimeb(uint8_t bx, uint8_t x);

/*
 * modprod program to perform circleX operation of 2 poly
 * @param: 2 poly, and 1 flag for printing output
 * @return: 1 poly as a result of circleX operation of 2 input poly
 */
struct modprod_result modprod(uint8_t poly1[], uint8_t poly2[], int printFlag);

#endif /* MODPROD_H_ */
