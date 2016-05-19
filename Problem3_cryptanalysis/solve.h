/*
 * solve.h
 *
 *  Created on: Feb 20, 2016
 *      Author: ntran
 */

#ifndef SOLVE_H_
#define SOLVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ciphertextInfo {
	char *ciphertext;
	int total_char;
	int total_alphabet_char;
};

/*
 * Supplemental function to read ciphertext file input and store it internally for post process
 * @param: file pointer to input file
 *@return: struct that contains char pointer to ciphertext input and total char count
 */
struct ciphertextInfo read_ciphertext (FILE *fp);


/*
 * Use Kasiski method to find repetitive phrase with certain length in ciphertext
 * @param: struct that contains char pointer to ciphertext input and total char count
 * @output: Kasis Method report to show the index of repetitive phrase with certain length
 * EX: len=4, i=41, j=131, j-i=90, zstv
 */
void findKasiski(struct ciphertextInfo ciphertext);

/*
 * Calculate and tabulate average index of coincidence
 * @param:struct that contains char pointer to ciphertext input and total char count
 * @param: maximum number of periods
 * @output: table of average index of coincidence
 */
void avgIndexCoincidence(struct ciphertextInfo ciphertext, int max_t);

/*
 * Auto-correlation Index of Coincidence
 * @param:struct that contains char pointer to ciphertext input and total char count
 * @param: maximum number of periods
 * @output: tablet of Aauto-correlation Method
 */
void autoCorrelation(struct ciphertextInfo ciphertext, int max_t);

/*
 * solve function to call sub function to do Kasiski, average index of coincidence, and auto-correlation of coincidence
 * @param: file pointer to input file and max_t period
 * @output: Kasiski, average index of coincidence, and auto-correlation of coincidence outputs
 */
void solve(FILE *fp, int max_t);

#endif /* SOLVE_H_ */
