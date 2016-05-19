/*
 * tablecheck.h
 *
 *  Created on: Feb 25, 2016
 *      Author: ntran
 */

#ifndef TABLECHECK_H_
#define TABLECHECK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct table{
	int tableIndex;
	int isValid;
};

struct tablefile{
	int IP[64];
	int IPInv[64];
	int E[48];
	int P[32];
	int S[8][64];
	int V[16];
	int PC1[56];
	int PC2[48];
	int isValid;
};

/*
 * Check if a given tablefile is valid
 * @param: file pointer to tablefile
 * @return: struct tablefile containing all info if it's a valid. Otherwise, valid is set to 0
 */
struct tablefile isValidTablefile (FILE *fp);

/*
 * Suuplemental function to compute inverse of IP table
 * @param: pointer to tablefile struct which contains IP table
 * @output: computed inverse IP table
 */
void computeIPinv(struct tablefile *table);

/*
 * Supplemental function to validate key=value of each line in tablefile
 * @param: pointer to line of char, and reference to my struct tablefile
 * @return: struct table with table index and 0 if invalid and 1 if valid
 */
struct table isValidLine (char *line, struct tablefile *table);

#endif /* TABLECHECK_H_ */
