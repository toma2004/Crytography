/*
 * keygen.h
 *
 *  Created on: Feb 14, 2016
 *      Author: ntran
 */

#ifndef KEYGEN_H_
#define KEYGEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/*
 * Generate key and print it out to stdout
 */
void generate_key(char *pphrase, int period);

/*
 * User simple stream cipher from HW2 to generate randon number X
 */
unsigned char *random_integer(char *pphrase, char *s, int hash_len, int i, unsigned char md5_buf[]);

#endif /* KEYGEN_H_ */
