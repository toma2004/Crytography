/*
 * crypt.h
 *
 *  Created on: Feb 16, 2016
 *      Author: ntran
 */

#ifndef CRYPT_H_
#define CRYPT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Function to check if a keyfile is in the right format
 * @param file pointer to a keyfile
 * @return: 0 - for invalid keyfile and 1 - for valid keyfile
 */
int checkKeyfile (FILE *fp);

/*
 * Function to encrypt or decrypt a file given a keyfile
 * @param: fp - keyfile, fp2 - file to be encrypted/decrypted
 * @output: encrypted/decrypted ASCII text
 */
void crypt(FILE *fp, char* keyfile, FILE *fp2);

#endif /* CRYPT_H_ */
