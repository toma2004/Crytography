/*
 * pbm.c
 *
 *  Created on: Feb 5, 2016
 *      Author: ntran
 */

#include "pbm.h"

/*
 * Create an empty pbm image with given length, width, and height
 * @return: a struct with 2 pointers
 */
struct PBM create_empty_image(int len, char *width, char *height)
{
	int mycount = 0;

	int width_len = strlen(width);
	int height_len = strlen(height);
	//create a structure to return the pointer to the beginning of the image file and also pointer to where we should write next bit
	struct PBM mypbm;
	mypbm.image_len = 3 + width_len + 1 + height_len + 1 + len; //Total length of my image = 3 byte of P4\n + width + 1 space char + height + 1 \n + image

	char *myimage = (char *) malloc(mypbm.image_len);
	char *first = myimage;

	//make sure the first 3 byte are P4\n
	*myimage = 'P';
	myimage += 1;
	mycount += 1;
	*myimage = '4';
	myimage += 1;
	mycount += 1;
	*myimage = '\n';
	myimage += 1;
	mycount += 1;

	//then ensure to have correct width and height format as <width>[space]<height>\n
	for (int i = 0; i < width_len; i++)
	{
		*myimage = *width;
		myimage += 1;
		width += 1;
		mycount += 1;
	}
	*myimage = ' ';
	myimage += 1;
	mycount += 1;

	for (int i = 0; i < height_len; i++)
	{
		*myimage = *height;
		myimage += 1;
		height += 1;
		mycount += 1;
	}
	*myimage = '\n';
	myimage += 1;
	char*continuePointer = myimage;
	mycount += 1;

	//initialize all bits to be null. We will set bit accordingly later based on input and key bits
	for (int i = mycount; i < mypbm.image_len; i++)
	{
		*myimage = '\0';
		myimage += 1;
	}

	mypbm.myImagePointer = first;
	mypbm.myContinuePointer = continuePointer;

	return mypbm; //will be freed upon the function calling it
}

/*
 * Check an input pbm for validation
 * @param: FILE pointer
 * @return: pointers to width and height
 */
struct PBM_input checkPBM (FILE *fp)
{
	struct PBM_input myPBMinput;

	char pbm_check[3];
	int byte = 0;
	byte = fread(pbm_check,1,sizeof(pbm_check),fp);
	//fwrite(pbm_check,1,sizeof(pbm_check), stdout);

	if (byte < 3)
	{
		//Error: incorrect pbm file format
		fprintf(stderr, "ERROR: incorrect pbm file format\n");
		exit(1);
	}
	else
	{
		if (strncmp(pbm_check,"P4\n", sizeof(pbm_check)) == 0)
		{
			//continue to read to check height and width
			int size = 256;
			char *buffer = (char *)malloc(size);
			char *mytemp = buffer;
			myPBMinput.buffer = buffer;
			char temp[1];
			int count;
			while ((byte = fread(temp,1,sizeof(temp), fp)) > 0)
			{
				count += 1;
				if (strncmp(temp,"\n",1) == 0)
				{
					*mytemp = '\0';
					break;
				}
				*mytemp = temp[0];
				mytemp += 1;
				if (count == 256)
				{
					//Error: incorrect pbm file format
					fprintf(stderr, "ERROR: incorrect pbm file format\n");
					exit(1);
				}
			}
			//determine width and height
			char *val = strchr(buffer, ' ');
			if (val == NULL)
			{
				//Error: incorrect pbm file format
				fprintf(stderr, "ERROR: incorrect pbm file format\n");
				exit(1);
			}
			else
			{
				myPBMinput.width = buffer;
				*val = '\0';
				val += 1;
				myPBMinput.height = val;

				return myPBMinput; //buffer will be freed upon calling this function

			}

		}
		else
		{
			//Error: incorrect pbm file format
			fprintf(stderr, "ERROR: incorrect pbm file format\n");
			exit(1);
		}
	}
}

/*
 * Set bit in the pbm file
 * @param: row and col in image to be set, number of byte per row, and struct PBm image pointer
 */
void setPBMBit (int row, int col, int byte_per_row, struct PBM *image)
{
	//From row and col, figure out what byte and bit that needs to be set
	//Initially, need to find out which byte is the beginning of this row
	int byteTH_beginning = row * byte_per_row;

	//now find out which byte and bit in this row to be set
	int byteTH = byteTH_beginning + (col / 8);
	int bitTH = 7 - (col % 8);

	//printf ("Here is what I set: row = %d, col =%d, byte_per_row=%d, byteTH beginning = %d, byteTH = %d, bitTH = %d \n", row, col, byte_per_row, byteTH_beginning, byteTH, bitTH);

	//Error check to be safe
	if (byteTH > (byteTH_beginning + byte_per_row -1))
	{
		//ERROR
		fprintf (stderr, "ERROR: Trying to set a byte that is not in a row, byte = %d, byte_per_row = %d\n",byteTH, byte_per_row);
		exit(1);
	}
	else
	{
		*(image->myContinuePointer + byteTH) |= ((0x01) << bitTH);
	}
}

/*
 * write an image to a file with given name
 * @param: struct image pointer and file name
 */
void write_image_file (struct PBM *image, char *filename)
{
	FILE *fp;
	int orig_str_len = strlen(filename);

	int mylen = orig_str_len + 5; // 5 for ".pbm\0"
	char *myfilename = malloc(mylen);

	char *myfirst = myfilename;

	for (int i = 0; i < orig_str_len; i++)
	{
		*myfilename = *filename;
		myfilename += 1;
		filename += 1;
	}

	*myfilename = '.';
	myfilename += 1;
	*myfilename = 'p';
	myfilename += 1;
	*myfilename = 'b';
	myfilename += 1;
	*myfilename = 'm';
	myfilename += 1;
	*myfilename = '\0';

	fp = fopen(myfirst, "w+");
	fwrite(image->myImagePointer, 1, image->image_len, fp);

	free(myfirst);
	fclose(fp);
}

/*
 * Set bit in the pbm file
 * @param: row and col in image to be set, number of byte per row, and struct PBm image
 */
int getPBMBit (int row, int col, int byte_per_row, struct PBM image)
{
	//From row and col, figure out what byte and bit that needs to be set
	//Initially, need to find out which byte is the beginning of this row
	int byteTH_beginning = row * byte_per_row;

	//now find out which byte and bit in this row to be set
	int byteTH = byteTH_beginning + (col / 8);
	int bitTH = 7 - (col % 8);

	//printf ("Here is what I set: row = %d, col =%d, byte_per_row=%d, byteTH beginning = %d, byteTH = %d, bitTH = %d \n", row, col, byte_per_row, byteTH_beginning, byteTH, bitTH);

	//Error check to be safe
	if (byteTH > (byteTH_beginning + byte_per_row -1))
	{
		//ERROR
		fprintf (stderr, "ERROR: Trying to get a byte that is not in a row, byte = %d, byte_per_row = %d\n",byteTH, byte_per_row);
		exit(1);
	}
	else
	{
		//*(image->myContinuePointer + byteTH) |= ((0x01) << bitTH);
		if ((*(image.myContinuePointer + byteTH) & ((0x01) << bitTH)) == 0) //bit is not set
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
