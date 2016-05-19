/*
 * encrypt.c
 *
 *  Created on: Feb 1, 2016
 *      Author: ntran
 */

#include "encrypt.h"


//Supplemental function to convert an inteter to char pointer
char *itoa (int n)
{
	char *myreturn = (char *) malloc(32);
	char *mytemp = myreturn;
	char *temp = (char *) malloc(32);
	char *tempfirst = temp;

	while (n > 0)
	{
		*(temp) =  (n % 10) + '0';
		temp += 1;
		n = n / 10;
	}

	temp -= 1;
	while(tempfirst <= temp)
	{
		*mytemp = *(temp);
		mytemp += 1;
		temp -= 1;
	}
	*(mytemp) = '\0';
	free (tempfirst);

	return myreturn; //Will be freed upon function calling it
}

//Encrypting function. Expand 1 pixel in original image into 4 pixels in shared images
void encrypt(char *pphrase, char *out, FILE *fp)
{
	struct PBM_input myPBMInput = checkPBM(fp); //Will be freed after function finishes

	int mywidth = atoi(myPBMInput.width);
	int myheight = atoi(myPBMInput.height);


	//Calculate how many bytes total per sub pbm file and how many byte per row
	int byte_total = 0;
	int byte = 0;
	int byte_per_row = 0;
	if (mywidth % 8 == 0)
	{
		byte_per_row = mywidth/8;
	}
	else
	{
		byte_per_row = (mywidth/8) + 1;
	}
	byte_total = myheight * byte_per_row;

	//Generate 2 output files with appropriate size based on the original size
	int mybit_per_row_output = (mywidth*2);
	int mybyte_per_row_output = 0;
	if (mybit_per_row_output % 8 == 0)
	{
		mybyte_per_row_output = mybit_per_row_output / 8;
	}
	else
	{
		mybyte_per_row_output = (mybit_per_row_output / 8) + 1;
	}
	int total_output_bytes = mybyte_per_row_output * (myheight*2);
	char *output_width = itoa(mywidth*2);
	char *output_height = itoa(myheight*2);
	struct PBM myOutputImage1 = create_empty_image(total_output_bytes, output_width, output_height); //WILL NEED TO BE FREED
	struct PBM myOutputImage2 = create_empty_image(total_output_bytes, output_width, output_height);

	//Generate key stream with correct key len
	int byte_key = 0;
	int total_key_bits = mywidth * myheight;
	if (total_key_bits % 8 == 0)
	{
		byte_key = total_key_bits / 8;
	}
	else
	{
		byte_key = (total_key_bits / 8) + 1;
	}
	char *mykey = generate_key(pphrase, byte_key); //WILL NEED TO BE FREED


	/*
	 * START ENCRYPTING PROCESS
	 */
	char myrow[byte_per_row];
	int mycount = 0;
	int myKeyBitUsed = 0;
	int total_bit_perRow_output = mywidth * 2;
	int total_bit_perCol_output = myheight * 2;


	//use these 4 variables to keep track of which bit we want to set
	//initialized to be the first square in output file
	int row0 = 0;
	int row1 = 1;
	int col0 = 0;
	int col1 = 1;

	//keep track of bit and byte of key
	int keyByteTH = 0;
	int keyBitTH = 0;


	while ((byte = fread(myrow, 1, byte_per_row, fp)) > 0)
	{
		mycount += byte;
		//do error checking to be safe from malformed input
		if (mycount > byte_total || myKeyBitUsed >= total_key_bits || row1 >= total_bit_perCol_output)
		{
			break; //break out of while loop due to unexpected error
		}

		//Go through each bit of the the bytes we read in
		for (int i = 0; i < byte_per_row; i++)
		{
			if (mywidth % 8 != 0 && i == (byte_per_row-1))
			{
				//Last byte, and only take mywidth%8 beginning bits
				for (int k = 7; k >= (8-(mywidth%8)); k--)
				{

					if (keyBitTH == 8)
					{
						keyByteTH += 1; //go to next byte
						keyBitTH = 0; //reset key bit to the most significant bit of next byte

					}
					//Check if this bit is set or not
					if ((myrow[i] & ((0x01) << k)) == 0) //bit is not set
					{
						//check key bit if it's set or not
						if ((*(mykey + keyByteTH)  & ((0x01) << (7 - keyBitTH))) == 0) //key bit is not set
						{
							setPBMBit(row0, col0, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row1, col1, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row0, col0, mybyte_per_row_output, &myOutputImage2);
							setPBMBit(row1, col1, mybyte_per_row_output, &myOutputImage2);
							if (col1 + 1 >= total_bit_perRow_output)
							{
								//we know that this is the last bit in this row, reset col, and increment row
								col0 = 0;
								col1 = 1;
								row0 += 2;
								row1 += 2;
							}
							else
							{
								col0 += 2;
								col1 += 2;
							}
						}
						else
						{
							setPBMBit(row0, col1, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row1, col0, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row0, col1, mybyte_per_row_output, &myOutputImage2);
							setPBMBit(row1, col0, mybyte_per_row_output, &myOutputImage2);
							if (col1 + 1 >= total_bit_perRow_output)
							{
								//we know that this is the last bit in this row, reset col, and increment row
								col0 = 0;
								col1 = 1;
								row0 += 2;
								row1 += 2;
							}
							else
							{
								col0 += 2;
								col1 += 2;
							}
						}
						myKeyBitUsed += 1;
						keyBitTH += 1;
					}
					else
					{
						//check key bit if it's set or not
						if ((*(mykey + keyByteTH)  & ((0x01) << (7 - keyBitTH))) == 0) //key bit is not set
						{
							setPBMBit(row0, col0, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row1, col1, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row0, col1, mybyte_per_row_output, &myOutputImage2);
							setPBMBit(row1, col0, mybyte_per_row_output, &myOutputImage2);
							if (col1 + 1 >= total_bit_perRow_output)
							{
								//we know that this is the last bit in this row, reset col, and increment row
								col0 = 0;
								col1 = 1;
								row0 += 2;
								row1 += 2;
							}
							else
							{
								col0 += 2;
								col1 += 2;
							}
						}
						else
						{
							setPBMBit(row0, col1, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row1, col0, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row0, col0, mybyte_per_row_output, &myOutputImage2);
							setPBMBit(row1, col1, mybyte_per_row_output, &myOutputImage2);
							if (col1 + 1 >= total_bit_perRow_output)
							{
								//we know that this is the last bit in this row, reset col, and increment row
								col0 = 0;
								col1 = 1;
								row0 += 2;
								row1 += 2;
							}
							else
							{
								col0 += 2;
								col1 += 2;
							}
						}
						myKeyBitUsed += 1;
						keyBitTH += 1;
					}
				}
			}
			else
			{
				//Go through each bit of a byte
				for (int k = 7; k >= 0; k--)
				{
					if (keyBitTH == 8)
					{
						keyByteTH += 1; //go to next byte
						keyBitTH = 0; //reset key bit to the most significant bit of next byte

					}

					//Check if this bit is set or not
					if ((myrow[i] & (0x01 << k)) == 0) //bit is not set
					{
						//check key bit if it's set or not
						if ((*(mykey + keyByteTH) & ((0x01) << (7 - keyBitTH))) == 0) //key bit is not set
						{
							//printf ("I think input is NOT set and key is NOT set \n");
							setPBMBit(row0, col0, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row1, col1, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row0, col0, mybyte_per_row_output, &myOutputImage2);
							setPBMBit(row1, col1, mybyte_per_row_output, &myOutputImage2);
							if (col1 + 1 >= total_bit_perRow_output)
							{
								//we know that this is the last bit in this row, reset col, and increment row
								col0 = 0;
								col1 = 1;
								row0 += 2;
								row1 += 2;
							}
							else
							{
								col0 += 2;
								col1 += 2;
							}
						}
						else
						{
							//printf ("I think input is NOT set and key is set \n");
							setPBMBit(row0, col1, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row1, col0, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row0, col1, mybyte_per_row_output, &myOutputImage2);
							setPBMBit(row1, col0, mybyte_per_row_output, &myOutputImage2);
							if (col1 + 1 >= total_bit_perRow_output)
							{
								//we know that this is the last bit in this row, reset col, and increment row
								col0 = 0;
								col1 = 1;
								row0 += 2;
								row1 += 2;
							}
							else
							{
								col0 += 2;
								col1 += 2;
							}
						}
						myKeyBitUsed += 1;
						keyBitTH += 1;
					}
					else
					{
						//check key bit if it's set or not
						if ((*(mykey + keyByteTH) & ((0x01) << (7 - keyBitTH))) == 0) //key bit is not set
						{
							//printf ("I think input is set and key is NOT set \n");
							setPBMBit(row0, col0, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row1, col1, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row0, col1, mybyte_per_row_output, &myOutputImage2);
							setPBMBit(row1, col0, mybyte_per_row_output, &myOutputImage2);
							if (col1 + 1 >= total_bit_perRow_output)
							{
								//we know that this is the last bit in this row, reset col, and increment row
								col0 = 0;
								col1 = 1;
								row0 += 2;
								row1 += 2;
							}
							else
							{
								col0 += 2;
								col1 += 2;
							}
						}
						else
						{
							//printf ("I think input is set and key is set \n");
							setPBMBit(row0, col1, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row1, col0, mybyte_per_row_output, &myOutputImage1);
							setPBMBit(row0, col0, mybyte_per_row_output, &myOutputImage2);
							setPBMBit(row1, col1, mybyte_per_row_output, &myOutputImage2);
							if (col1 + 1 >= total_bit_perRow_output)
							{
								//we know that this is the last bit in this row, reset col, and increment row
								col0 = 0;
								col1 = 1;
								row0 += 2;
								row1 += 2;
							}
							else
							{
								col0 += 2;
								col1 += 2;
							}
						}
						myKeyBitUsed += 1;
						keyBitTH += 1;
					}
				}
			}
		}
	}

	//Check to see if there is enough byte as claimed by height x width

	if (mycount != byte_total || myKeyBitUsed != total_key_bits)
	{
		//ERROR: missing byte in pbm file
		fprintf(stderr, "ERROR: incorrect pbm file format\n");
		/*
		 * Free all pointers before exit
		 */
		//Free buffer that hold the <width> <height>\n line from input
		free(myPBMInput.buffer);
		//Free key stream
		free(mykey);
		//free char allocation from itoa function
		free(output_width);
		free(output_height);

		//free all my images
		free(myOutputImage1.myImagePointer);
		free(myOutputImage2.myImagePointer);

		exit(1);
	}

	//write image to file
	char *fileNum1 = ".1";
	char *fileNum2 = ".2";

	char *resulting_fileName1 = malloc (strlen(fileNum1) + strlen(out));
	char *resulting_fileName2 = malloc (strlen(fileNum2) + strlen(out));
	strcpy(resulting_fileName1, out);
	strcat(resulting_fileName1, fileNum1);
	strcpy(resulting_fileName2, out);
	strcat(resulting_fileName2, fileNum2);

	write_image_file (&myOutputImage1, resulting_fileName1);
	write_image_file (&myOutputImage2, resulting_fileName2);


	//Free buffer that hold the <width> <height>\n line from input
	free(myPBMInput.buffer);

	//Free key stream
	free(mykey);

	//free file name
	free(resulting_fileName1);
	free(resulting_fileName2);

	//free char allocation from itoa function
	free(output_width);
	free(output_height);

	//free all my images
	free(myOutputImage1.myImagePointer);
	free(myOutputImage2.myImagePointer);
}


