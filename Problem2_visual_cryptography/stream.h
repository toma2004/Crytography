/*
 * stream.h
 *
 *  Created on: Jan 30, 2016
 *      Author: ntran
 */

#ifndef STREAM_H_
#define STREAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/*
 * Generate simple stream cipher with passphrase and specified length
 */
void key_stream_generator(char *pphrase, int len);
char *generate_key(char *pphrase, int len);


#endif /* STREAM_H_ */
