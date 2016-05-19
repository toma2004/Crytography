/*
 * decrypt.c
 *
 *  Created on: Feb 9, 2016
 *      Author: ntran
 */

#include "decrypt.h"

//Decrypt an image that is already merged
void decrypt(FILE *fp)
{
	//Check image format
	struct PBM_input imageCheck = checkPBM(fp); //Will be freed when function finishes
	//Getting width and height
	int input_width = atoi(imageCheck.width);
	int input_height = atoi(imageCheck.height);

	//Calculate the correct size of output image
	int outputW_bit = input_width / 2;
	int outputH_bit = input_height / 2;

	int output_byte_per_row = 0;
	if (outputW_bit % 8 == 0)
	{
		output_byte_per_row = outputW_bit / 8;
	}
	else
	{
		output_byte_per_row = (outputW_bit / 8) + 1;
	}
	int output_total_byte = outputH_bit * output_byte_per_row;

	//Generate output image based on the calculated size
	struct PBM myimage = create_empty_image(output_total_byte, itoa(outputW_bit), itoa(outputH_bit)); //itoa function is implemented in encrypt.c | myimage will be freed after function finishes

	//determine how many byte we should read per row in input
	int read_byte_per_row = 0;
	if (input_width % 8 == 0)
	{
		read_byte_per_row = input_width / 8;
	}
	else
	{
		read_byte_per_row = (input_width / 8) + 1;
	}

	char myrow0[read_byte_per_row];
	char myrow1[read_byte_per_row];

	//Read in 2 rows at a time
	int byte_read0 = 0;
	int byte_read1 = 0;

	//initialize place holder for querying bit result
	int bit0 = -1;
	int bit1 = -1;
	int bit2 = -1;
	int bit3 = -1;

	//keep track of bit and byte in output file
	int bitTH = 7;
	int byteTH = 0;
	int numBit = 0;

	while(((byte_read0 = fread(myrow0, 1, read_byte_per_row, fp)) > 0) && ((byte_read1 = fread(myrow1, 1, read_byte_per_row, fp)) > 0))
	{
		//do error check to be safe
		if (byte_read0 != byte_read1)
		{
			//we expect to read in the same number of bytes for every row.
			break;
		}
		//Go through each byte we read in
		for(int i = 0; i < read_byte_per_row; i++)
		{
			//Check last byte of each row
			if ((input_width % 8 != 0) && (i == read_byte_per_row - 1))
			{
				//go through every 2 bits
				int b = 7;
				while (b >= (8-(input_width%8)))
				{
					bit0 = myrow0[i] & ((0x01) << b);
					bit1 = myrow0[i] & ((0x01) << (b-1));
					bit2 = myrow1[i] & ((0x01) << b);
					bit3 = myrow1[i] & ((0x01) << (b-1));
					if (bit0 && bit1 && bit2 && bit3) //all 1 meaning black pixel
					{
						*(myimage.myContinuePointer + byteTH) |= ((0x01) << bitTH);
					}
					b -= 2;
					bitTH -= 1;
					numBit += 1;
					//reset bit and increase byte when all bits of a byte is used
					if (bitTH < 0)
					{
						bitTH = 7;
						byteTH += 1;
					}

					//Also reset bit and go to next byte when we reach the end of row in outputfile
					if (numBit == outputW_bit)
					{
						bitTH = 7;
						byteTH += 1;
						numBit = 0;
					}
				}
			}
			else
			{
				//go through every 2 bits
				int b = 7;
				while (b > 0)
				{
					bit0 = myrow0[i] & ((0x01) << b);
					bit1 = myrow0[i] & ((0x01) << (b-1));
					bit2 = myrow1[i] & ((0x01) << b);
					bit3 = myrow1[i] & ((0x01) << (b-1));
					if (bit0 && bit1 && bit2 && bit3) //all 1 meaning black pixel
					{
						*(myimage.myContinuePointer + byteTH) |= ((0x01) << bitTH);
					}
					b -= 2;
					bitTH -= 1;
					numBit += 1;
					//reset bit and increase byte when all bits of a byte is used
					if (bitTH < 0)
					{
						bitTH = 7;
						byteTH += 1;
					}

					//Also reset bit and go to next byte when we reach the end of row in outputfile
					if (numBit == outputW_bit)
					{
						bitTH = 7;
						byteTH += 1;
						numBit = 0;
					}
				}
			}
		}
	}
	fwrite(myimage.myImagePointer, 1, myimage.image_len, stdout);

	//Free all pointers
	free(imageCheck.buffer);
	free(myimage.myImagePointer);

}
