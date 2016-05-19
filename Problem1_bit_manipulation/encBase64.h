/*
 * encBase64.h
 *
 *  Created on: Jan 26, 2014
 *      Author: ntran
 */

#ifndef ENCBASE64_H_
#define ENCBASE64_H_

#include <inttypes.h>
#include <stdio.h>

void encBase64(void *location, int size, int remainder);

#endif /* ENCBASE64_H_ */
