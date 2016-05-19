/*
 * x5.h
 *
 *  Created on: Apr 24, 2016
 *      Author: ntran
 */

#ifndef X5_H_
#define X5_H_

#include "x4.h"

/*
 * Supplemental function to calculate A(d)
 * @param: n and dval and bit stream
 * @return: A(d)
 */
int calculate_A(int n, int dval, unsigned char* data);

/*
 * Supplemental function to calculate x5
 * @param: n, dval, A(d)
 * @return: x5 value
 */
double calculate_x5(int n, int dval, int A);

/*
 * x5 - autocorrelation test
 * @param: dval and FILE pointer points to bit stream
 * @output: x5 test process and result
 */
void x5(int dval, FILE *fp);

#endif /* X5_H_ */
