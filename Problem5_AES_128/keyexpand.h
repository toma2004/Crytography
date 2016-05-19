/*
 * keyexpand.h
 *
 *  Created on: Mar 15, 2016
 *      Author: ntran
 */

#ifndef KEYEXPAND_H_
#define KEYEXPAND_H_

#include "tablecheck_AES.h"

#define Nk 4
#define Nb 4
#define Nr 10

struct mykey{
	int isValid;
	uint8_t key[16]; //16 byte key represented in hex string
};

struct mysubkeys{
	uint32_t subkeys[44]; //used to store 44 subkeys
};


/*
 * Supplemental function to check if a given key string is valid
 * @param: char pointer to key string
 * @output: isValid = 1 if key is valid and key string is stored in key[16], else isValid = 0
 */
struct mykey isValidKey(char *key);

/*
 * Supplemental function to rotate a word. Example: given 0x09cf4f3c => output = 0xcf4f3c09
 * @param: uint32_t word
 * @output: uint32_t word rotated
 */
uint32_t rotate_word(uint32_t inputWord);

/*
 * Supplemental function to substitute word using S-box from tablefile
 * @param: uint32_t word, and tablefile containing S-box
 * @output: uint32_t word substituted using S-box
 */
uint32_t sub_word(uint32_t inputWord, struct tablefile mytable);

/*
 * Suuplemental function to get Rcon[i/Nk] given i/Nk
 * @param: value of i/Nk
 * @output: uint32_t Rcon using the formula {02}^i-1, {00},{00},{00}
 */
uint32_t get_Rcon(int i_Nk);

/*
 * Keyexpand function to generate 44 4-byte subkeys from given 16-byte key
 * @param:struct mykey containing the key, struct tablefile in order to supply s-box for sub_word operation, and a flag to indicate if we need to print output to stdout
 * @output: 44 subkeys printed out in stdout and return struct mysubkeys containing all 44 subkeys
 */
struct mysubkeys keyexpansion(struct mykey key, struct tablefile mytable, int toPrint);

#endif /* KEYEXPAND_H_ */
