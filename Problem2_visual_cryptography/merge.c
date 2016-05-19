/*
 * merge.c
 *
 *  Created on: Feb 8, 2016
 *      Author: ntran
 */

#include "merge.h"

//Merge 2 pbm files
//@param: 2 PBM images
void mergePBMFiles (FILE *fp, FILE *fp2, int width, int height)
{
	int byte_read = 0;
	if (width % 8 == 0)
	{
		byte_read = width / 8;
	}
	else
	{
		byte_read = (width / 8) + 1;
	}

	//Generate the output image after merge
	int byte_per_row = 0;
	int image_len = 0;
	if (width % 8 == 0)
	{
		byte_per_row = width / 8;
	}
	else
	{
		byte_per_row = (width / 8) + 1;
	}
	image_len = byte_per_row * height;

	struct PBM merge_image = create_empty_image(image_len, itoa(width), itoa(height)); //will be freed when function finishes

	char myrow0[byte_read];
	char myrow1[byte_read];
	int byte_file0 = 0;
	int byte_file1 = 0;

	int byteTh = 0;
	int byte = 0;

	while( ((byte_file0 = fread(myrow0,1,byte_read,fp)) > 0) && (byte_file1 = fread(myrow1,1,byte_read,fp2)) > 0)
	{
		byte += 1;
		//go through each byte of 1 row
		for (int i = 0; i < byte_read; i++)
		{
			//Check last byte of each row
			if ((width % 8 != 0) && (i == byte_read - 1))
			{
				//Only go through width % 8 bits of last byte
				for (int b = 7; b >= (8-(width%8)); b--)
				{
					//bit in image 1 and image 2 are NOT set
					if (((myrow0[i] & ((0x01) << b)) == 0) && ((myrow1[i] & ((0x01) << b)) == 0))
					{
						//Don't set the bit
						*(merge_image.myContinuePointer + byteTh) &= ~(0x01) << b;
					}
					//Any other cases the bit will get set
					else
					{
						//Set the bit in
						*(merge_image.myContinuePointer + byteTh) |= (0x01) << b;
					}
				}
				byteTh += 1; //go to next byte and ignore the remaining bits of this byte
			}
			else
			{
				//Go through every single bits
				for (int b = 7; b >= 0; b--)
				{
					//bit in image 1 and image 2 are NOT set
					if (((myrow0[i] & ((0x01) << b)) == 0) && ((myrow1[i] & ((0x01) << b)) == 0))
					{
						//Don't set the bit
						*(merge_image.myContinuePointer + byteTh) &= ~(0x01) << b;
					}
					//Any other cases the bit will get set
					else
					{
						//Set the bit in
						*(merge_image.myContinuePointer + byteTh) |= (0x01) << b;
					}
				}
				byteTh += 1; //go to next byte
			}
		}
	}
	//Finish and write the merge image to stdout
	fwrite(merge_image.myImagePointer,1,merge_image.image_len,stdout);

	free (merge_image.myImagePointer);
}
