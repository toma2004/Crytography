/*
 * encrypt3.h
 *
 *  Created on: Mar 4, 2016
 *      Author: ntran
 */

#ifndef ENCRYPT3_H_
#define ENCRYPT3_H_

#include "encrypt.h"
#include "decrypt.h"

struct key3{
	int isValid;
	unsigned char key3[3][8]; //8-byte for each part of key (total 24 bytes)
};

/*
 * Supplemental function to check if key is valid
 * @param: pointer to key file
 * @reutrn: struct key containing key and 0 if invalid, and 1 if valid
 */
struct key3 isValid_Key3(char *input_key3);

/*
 * encrypt3 function to encrypt the plaintext using EDE 3DES
 * @param: tablefile, key, and file input pointer
 * @output: ciphertext
 */
void encrypt3(struct tablefile mytable, struct key3 mykey3, FILE *fp);

#endif /* ENCRYPT3_H_ */
