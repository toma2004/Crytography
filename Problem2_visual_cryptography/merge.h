/*
 * merge.h
 *
 *  Created on: Feb 8, 2016
 *      Author: ntran
 */

#ifndef MERGE_H_
#define MERGE_H_

#include "encrypt.h"
#include "pbm.h"

//merge 2 pbm files
//@param: 2 PBM images
void mergePBMFiles (FILE *fp, FILE *fp2, int width, int height);

#endif /* MERGE_H_ */
