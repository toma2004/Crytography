/*
 * rc4.h
 *
 *  Created on: Apr 17, 2016
 *      Author: ntran
 */

#ifndef RC4_H_
#define RC4_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <openssl/md5.h>

#define MD5_DIGEST_LENGTH  16

/*
 * Supplemental function to check if a given pphrase is valid
 * @param: char pointer to a pass phrase
 * @return: 1 if isValid; else return 0
 */
int check_pphrase(char *pphrase);

/*
 * Supplemental function to check if a given length is valid
 * @param: char pointer to len
 * @return:
 * 0 if invalid
 * actual len in int format
 */
int check_len(char *len);

/*
 * Supplemental function to generate key given passphrase using MD5 algorithm
 * @param: char *pointer to passphrase
 * @return: 256-byte key (run MD15 16 times since each MD5 function call produces 16-byte key)
 */
unsigned char *generate_key(char *pphrase);

/*
 * Key scheduling algorithm
 * @param: generated key using MD5
 * @return: unsigned char* state containing the final state of key scheduling
 */
unsigned char* key_schedule(unsigned char* key);

/*
 * RC4 algorithm
 * @param: pass phrase, and an optional length
 * @output: print to stdout RC4 output with specified length if given
 */
void rc4(char* pphrase_input, int len);

#endif /* RC4_H_ */
