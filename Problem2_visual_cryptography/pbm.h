/*
 * pbm.h
 *
 *  Created on: Feb 2, 2016
 *      Author: ntran
 */

#ifndef PBM_H_
#define PBM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * struct PBM to hold the pointer to my pbm image in memory
 */
struct PBM {
		char *myImagePointer;
		char *myContinuePointer;
		int image_len;
};

/*
 * struct PBM_input holds pointer to my width and length of a given PBM image
 */
struct PBM_input {
	char *width;
	char *height;
	char *buffer; //used to hold the entire width height line in input
};

/*
 * Create an empty pbm image with given length, width, and height
 * @return: a struct with 2 pointers
 */
struct PBM create_empty_image(int len, char *width, char *height);


/*
 * Check an input pbm for validation
 * @param: FILE pointer
 * @return: pointers to width and height
 */
struct PBM_input checkPBM (FILE *fp);

/*
 * Set bit in the pbm file
 * @param: row and col in image to be set, number of byte per row, and struct PBm image pointer
 */
void setPBMBit (int row, int col, int byte_per_row, struct PBM *image);

/*
 * Set bit in the pbm file
 * @param: row and col in image to be set, number of byte per row, and struct PBm image
 */
int getPBMBit (int row, int col, int byte_per_row, struct PBM image);

/*
 * write an image to a file with given name
 * @param: struct image pointer and file name
 */
void write_image_file (struct PBM *image, char *filename);

#endif /* PBM_H_ */
