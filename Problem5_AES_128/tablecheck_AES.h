/*
 * tablecheck_AES.h
 *
 *  Created on: Mar 12, 2016
 *      Author: ntran
 */

#ifndef TABLECHECK_AES_H_
#define TABLECHECK_AES_H_

#include "modprod.h"

struct table{
	int tableIndex;
	int isValid;
};

struct tablefile{
	uint8_t S[16][16];
	uint8_t INVS[16][16];
	uint8_t P[4];
	uint8_t INVP[4];
	int isValid;
};

struct s_box_coor{
	int row;
	int col;
};

/*
 * Check if a given tablefile is valid
 * @param: file pointer to tablefile
 * @return: struct tablefile containing all info if it's a valid. Otherwise, valid is set to 0
 */
struct tablefile isValidTablefile (FILE *fp);

/*
 * Suuplemental function to compute inverse of S table
 * @param: pointer to tablefile struct which contains S table
 * @output: computed inverse S table
 */
void computeSinv(struct tablefile *table);

/*
 * Supplemental function to translate of 2 int inputs into 1 uint8_t output
 * @param: 2 int input, give that they are within range 0-15
 * @output: uint8_t
 */
uint8_t translateIntsToUint8(int i, int k);

/*
 * Supplemental function to translate a uint8_t into 2 int representing row and col of S-box
 * @param: uint8_t value
 * @output: struct s_box_coor containing the row and col in order to locate the value in s_box
 */
struct s_box_coor translateUint8toInt(uint8_t input);

/*
 * Supplemental function to validate key=value of each line in tablefile
 * @param: pointer to line of char, and reference to my struct tablefile
 * @return: struct table with table index and 0 if invalid and 1 if valid
 */
struct table isValidLine (char *line, struct tablefile *table);

#endif /* TABLECHECK_AES_H_ */
