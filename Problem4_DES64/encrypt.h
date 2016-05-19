/*
 * encrypt.h
 *
 *  Created on: Feb 25, 2016
 *      Author: ntran
 */

#ifndef ENCRYPT_H_
#define ENCRYPT_H_

#include <stdint.h>
#include "tablecheck.h"

struct key{
	int isValid;
	unsigned char key[8]; //64bit key
};

struct subkeys{
	unsigned char subkeys[16][6];
	int isValid;
};

struct LR{
	uint32_t l0;
	uint32_t r0;
};

struct Exp{
	unsigned char r0_expand[6];
};

/*
 * Supplemental function to check if key is valid
 * @param: pointer to key file
 * @reutrn: struct key containing key and 0 if invalid, and 1 if valid
 */
struct key isValidKey(char *key);

//convert into to char pointer
char *itoa(int n);

/*
 * Construct 16 48-bit subkeys from PC1, PC2, and key
 * @param: struct tablefile containig table PC1 and PC2, and key
 * @output: 2-D array pointing to 16 subkeys
 */
struct subkeys keySchedule(struct tablefile mytable, struct key mykey);

/*
 * sub-function to perform IP table
 * @param: 64-bit input char, and tablefile
 * @output: uint32_t l0 and r0
 */
struct LR performIP(struct tablefile mytable, char *input);

/*
 * sub-function to perform expansion
 * @param tablefile, and r0
 * @output: 48-bit expansion of 32-bit r0
 */
struct Exp performExp(struct tablefile mytable, uint32_t r0);

/*
 * sub-function to perform S-boxes substitution
 * @param: unsigned char 48 bits, and tablefile
 * @output: 32-bit uint32_t
 */
uint32_t performSbox(struct tablefile mytable,unsigned char xor_ki[]);

/*
 * sub-function to perform permutation for table P
 * @param: tablefile, and uint32_t number from the result of SBox
 * @output: 32-bit number permutated from table P
 */
uint32_t performP(struct tablefile mytable, uint32_t result_sBox);

/*
 * sub-function to perform IPInv
 * @param: tablefile, uint31_t R16, uint32_t L16
 * @output: 64-bit ciphertext
 */
unsigned char *performIPInv(struct tablefile mytable, uint32_t r16, uint32_t l16);

/*
 * encrypt block input text of 8 byte at a time using DES table driven method
 * @param: struct tablefile containing all tables, subkeys struct containing all necessary subkeys,input text
 * @output: print encrypted cipher text to stdout
 */
void encrypt(struct tablefile mytable, struct subkeys mysubkeys, FILE *fp);

/*
 * supplemental encryption function to support encrypt3 using EDE 3DES
 * @param: struct tablefile containing all tables, subkeys struct containing all necessary char array of 8-byte input, key part
 * @output: char* pointer to the encrypted ciphertext
 */
unsigned char *sup_encrypt(struct tablefile mytable, struct subkeys mysubkeys, char *myinput, int key_part);

#endif /* ENCRYPT_H_ */
