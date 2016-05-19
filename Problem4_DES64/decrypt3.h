/*
 * decrypt3.h
 *
 *  Created on: Mar 5, 2016
 *      Author: ntran
 */

#ifndef DECRYPT3_H_
#define DECRYPT3_H_

#include "encrypt3.h"

/*
 * decrypt3 function to decrypt the ciphertext using EDE 3DES
 * @param: tablefile, key, and file input pointer
 * @output: original plaintext
 */
void decrypt3(struct tablefile mytable, struct key3 mykey3, FILE *fp);

#endif /* DECRYPT3_H_ */
