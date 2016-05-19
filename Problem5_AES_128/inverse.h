/*
 * inverse.h
 *
 *  Created on: Mar 20, 2016
 *      Author: ntran
 */

#ifndef INVERSE_H_
#define INVERSE_H_

#include "tablecheck_AES.h"

struct inverse_poly{
	uint8_t inv_poly[4];
	int isValid;
};

struct lookup_table{
	uint8_t multiplicative_table[16][16];
};

struct long_division{
	uint8_t quo[4];
	uint8_t rem[4];
};

/*
 * Supplemental function to read poly, check if it is valid, and store it internally for further use
 * @param: char pointer to a poly
 * @return: struct inverse_poly with isValid=1 if poly is valid and inv_poly[] stores the poly; else, isValid=0
 */
struct inverse_poly readPoly(char *poly);

/*
 * Supplemental function to print inverse process output to stdout
 * @param: int round, uint8_t rem[], uint8_t quo[], uint8_t aux[]
 * @output: prinnt "i=round, rem[i]={xx}{xx}{xx}{xx}, quo[i]={xx}{xx}{xx}{xx}, aux[i]={xx}{xx}{xx}{xx}"
 */
void printOutput_inverse(int round, uint8_t rem[], uint8_t quo[], uint8_t aux[]);

/*
 * Supplemental function to look up for multiplicative of a given uint8_t byte
 * @papram: uint8_t poly to be looked up for its multiplicative, and a lookup table
 * @output: uint8_t poly multiplicative of input
 */
uint8_t lookup_multiplicative_inverse(uint8_t poly, struct lookup_table mytable);

/*
 * Supplemental function to construct a lookup_table for multiplicative inverse
 * @param: none
 * @return: struct lookup_table contain 16x16 2D array for all multiplicative inverse of 00-ff
 */
struct lookup_table construct_table();

/*
 * Supplemental function to perform long-hand division
 * @param: dividend, divisor (dividend / divisor), lookup table, and a flag to handle the the first case which is a special case where we have poly x^4
 * @return: struct long_division containing quotient and reminder
 */
struct long_division divide(uint8_t dividend[], uint8_t divisor[], struct lookup_table mytable, int isFirstCase);

/*
 * Function to find the multiplicative inverse of a given poly
 * @param: a valid poly
 * @output: multiplicative inverse of poly if it has one and print out the processes to stdout
 */
void inverse(uint8_t inv_poly[]);

#endif /* INVERSE_H_ */
