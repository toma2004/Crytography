/*
 * decrypt.h
 *
 *  Created on: Mar 20, 2016
 *      Author: ntran
 */

#ifndef DECRYPT_H_
#define DECRYPT_H_


#include "encrypt.h"

/*
 * decrypt input using AES-128
 * @param: pointer to input file, struct tablefile containing tables, and struct mykey containing key
 * @output: decrypted plaintext and print to stdout each round processes
 */
void decrypt(struct tablefile mytable, struct mykey key, FILE *fp);


#endif /* DECRYPT_H_ */
