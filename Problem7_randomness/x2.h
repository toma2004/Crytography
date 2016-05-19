/*
 * x2.h
 *
 *  Created on: Apr 19, 2016
 *      Author: ntran
 */

#ifndef X2_H_
#define X2_H_

#include "x1.h"

/*
 * Supplemental function to calculate x2 given n, n0, n1, n00, n01, n10, n11
 * @param: int n, n1, n0, n00, n01, n10, n11
 * @return: x2 = (4/(n-1))*(n00^2 + n01^2 + n10^2 + n11^2) - ((2/n)*(n0^2 + n1^2)) + 1
 */
double calculate_x2(int n, int n0, int n1, int n00, int n01, int n10, int n11);

/*
 * x2 - serial test
 * @param: FILE pointer to bit stream
 * @output: x2 test outcome
 */
void x2(FILE *fp);

#endif /* X2_H_ */
