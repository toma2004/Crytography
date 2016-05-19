/*
 * x3.h
 *
 *  Created on: Apr 19, 2016
 *      Author: ntran
 */

#ifndef X3_H_
#define X3_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>

struct mymk{
	int m;
	int k;
	int n;
	int isValid;
};

/*
 * Supplemental function to store bit stream internally for post processing
 * @param: FILE pointer points to bit stream
 * @return: unsigned char pointer points to beginning of bit stream stored internally
 */
char *store_bit_stream(FILE *fp);

/*
 * Supplemental function to calculate value of m and k
 * @param: char pointer point to bit stream stored in a file
 * @output: struct mymk containing value of m and k
 */
struct mymk calculate_mk(char* bitStream);

/*
 * Supplemental function to print hex dump
 * @param: char pointer points to bit stream stored in a file
 * @output: hex dump the first 256 bytes of bit stream if possible
 */
void print_hexdump(char* bitStream);

/*
 * Supplemental function to convert binary into decimal to print
 * @param: unsigned char* point to the binary, and size of bin (how many bytes)
 * @return: decimal representation of the given binary
 */
int bin_2_dec(unsigned char* bin, int size);

/*
 * Supplemental function to convert decimal into binary to print
 * @param: int dec and value of m
 * @return: string containing binary representation of given int
 */
char* dec_2_bin(int dec, int m);

/*
 * Supplemental function to calculate ni given m and bit stream
 * @param: value of m and bit stream
 * @return: int* points to n_array containing value of ni where 1 <= i <= 2^m
 */
int* calculate_ni(int m, char* bitStream);

/*
 * Supplemental function to re-implement "ceil" function math.h as this is not defined on nunki
 * This function will round up a double to the nearest integer that is greater than or equal to given input
 * @param: input of type double, and int of that same number to compare against
 * @return: output of type int representing the round-up
 */
int ceil_func(double num, int d);

/*
 * Supplemental function to print out all nxxx = <integer> to stdout
 * @param: m and int* points to ni array
 * @output: print nxxx= <integer> to stdout and ensure that xxx = m bits
 */
void print_nxx(int m, int* ni);

/*
 * Supplemental function to calculate x3 based on formula
 * x3 = (2^m/k)* (sum of (ni^2 where i=1 to 2^m) - k
 * @param: value of m and k, int* point to ni array
 * @return: double x3
 */
double calculate_x3(int m, int k, int* ni);

/*
 * x3 - Poker test
 * @param: FILE pointer points to bit stream
 * @output: x3 test outcome
 */
void x3(FILE *fp);

#endif /* X3_H_ */
