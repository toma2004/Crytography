/*
 * decrypt.h
 *
 *  Created on: Mar 2, 2016
 *      Author: ntran
 */

#ifndef DECRYPT_H_
#define DECRYPT_H_

#include "encrypt.h"

/*
 * decrypt block input text of 8 byte at a time using DES table driven method
 * @param: struct tablefile containing all tables, subkeys struct containing all necessary subkeys,input text
 * @output: print original plaintext to stdout
 */
void decrypt(struct tablefile mytable, struct subkeys mysubkeys, FILE *fp);

/*
 * supplemental decryption function to support decrypt3 using EDE 3DES
 * @param: struct tablefile containing all tables, subkeys struct containing all necessary char array of 8-byte input, key part
 * @output: char* pointer to the encrypted ciphertext
 */
unsigned char *sup_decrypt(struct tablefile mytable, struct subkeys mysubkeys, char *myinput, int key_part);

#endif /* DECRYPT_H_ */
