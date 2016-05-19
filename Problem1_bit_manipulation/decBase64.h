/*
 * decBase64.h
 *
 *  Created on: Jan 28, 2014
 *      Author: ntran
 */

#ifndef DECBASE64_H_
#define DECBASE64_H_

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

void decBase64(void *location, int size, int remainder, int done);
void printBits(size_t const size, void const * const ptr);
uint8_t convert (unsigned char c);	// convert a char of encoding to 8-bit binary number corresponding to its index in encoding table

#endif /* DECBASE64_H_ */
