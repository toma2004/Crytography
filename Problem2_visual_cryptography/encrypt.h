/*
 * encrypt.h
 *
 *  Created on: Feb 1, 2016
 *      Author: ntran
 */

#ifndef ENCRYPT_H_
#define ENCRYPT_H_

#include "stream.h"
#include "pbm.h"
//Encrypt the original image into 2 images with double size
void encrypt(char *pphrase, char *out, FILE *fp);

//convert into to char pointer
char *itoa(int n);

#endif /* ENCRYPT_H_ */
