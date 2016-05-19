/*
 * encrypt.h
 *
 *  Created on: Mar 16, 2016
 *      Author: ntran
 */

#ifndef ENCRYPT_H_
#define ENCRYPT_H_

#include "keyexpand.h"

struct myinput{
	uint8_t input[16];
	int isValid;
};

struct myholder{
	uint8_t holder[16]; //to hold 4 uint32_t numbers
};

/*
 * Supplemental function to read in 16 byte input and store internally for AES-128 encryption
 * @param: input file pointer
 * @return: struct my input with isValid = 1 if input is valid and uint8_t arr will contain all 16 byte input, else isValid = 0
 */
struct myinput readInput(FILE *fp);

/*
 * Supplemental function to print encrypt output to stdout
 * @param: round index, round type, content, and flag to signal of this function is used to print output for encrypt or decrypt
 * @output: print "round[round_index}.round_type   round_content" to stdout
 */
void printOutput(int round_index, char* round_type, uint8_t round_content[], int isEncryption);

/*
 * Supplemental function to translate 4 uint32_t numbers into 16 uint8_t numbers
 * @param: struct mysubkeys containing all 44 subkeys, from & to index to indicate which 4 subkeys to use
 * @return: struct myholder containing 16 uint8_t numbers constructed from 4 uint32_t numbers
 */
struct myholder getRoundKey(struct mysubkeys subkeys, int from, int to);

/*
 * Supplental function to perform subByte
 * @param: uint8_t byte array containing 16 bytes to be substituted, struct mytable containing s-box table, and flag to signal of this function is used for encrypt or decrypt
 * @output: struct myholder containing substituted byte using s-box
 */
struct myholder subByte(uint8_t toBeSub[], struct tablefile mytable, int isEncryption);

/*
 * Supplemental function to perform shiftrows
 * @param: uint8_t byte array to be under shift row operation, and flag to signal of this function is used for encrypt or decrypt
 * @output struct myholder containing result
 */
struct myholder shiftrows(uint8_t toBeShiftedRows[], int isEncryption);

/*
 * Supplemental function to perform mix coumns
 * @param: uint8_t byte array to be under mixed col operation, table file in order to get P - a(x) or INVP - a^-1(x), and flag to signal of this function is used for encrypt or decrypt
 * @output struct myholder containing result
 */
struct myholder mixcols(uint8_t toBeMixedCol[], struct tablefile mytable, int isEncryption);

/*
 * encrypt input using AES-128
 * @param: pointer to input file, struct tablefile containing tables, and struct mykey containing key
 * @output: encrypted cipher and print to stdout each round processes
 */
void encrypt(struct tablefile mytable, struct mykey key, FILE *fp);

#endif /* ENCRYPT_H_ */
