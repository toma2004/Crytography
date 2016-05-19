/*
 * x1.c
 *
 *  Created on: Apr 18, 2016
 *      Author: ntran
 */
#include "x1.h"

/*
 * x1 test
 * @param: FILE pointer to binary stream
 * @output: x1 test outcome
 */
void x1(FILE *fp)
{
	//count how many 0s and 1s in the binary stream, keep track of total number of bits
	int numbits = 0;
	int n0 = 0;
	int n1 = 0;

	//Read in 1 byte at a time and check bits for each byte
	unsigned char *temp = malloc(1);
	int byte = 0;
	int count = 0;
	//print hexdump for the first 256 bytes if possible
	while((byte = fread(temp, 1, 1, fp)) > 0)
	{
		count += 1;
		//print out hex dump
		fprintf(stdout, "%02x", *temp);
		if(count % 16 == 0)
		{
			fprintf(stdout, "\n");
		}

		numbits += 8; //1 byte = 8 bits => each byte read increment number of bit count to 8
		int bitTH = 7;
		while(bitTH >= 0)
		{
			if(((*temp) & ((0x01) << bitTH)) != 0)
			{
				n1 += 1;
			}
			else
			{
				n0 += 1;
			}
			bitTH -= 1;
		}

		if(count == 256)
		{
			break; //stop when we have output 256 bytes
		}
	}

	//print a newline character for cases we don't have a multiple of 16 in the number of bytes of input file
	if(count % 16 != 0)
	{
		fprintf(stdout, "\n");
	}

	//Read the rest of bytes if available
	while((byte = fread(temp, 1, 1, fp)) > 0)
	{
		numbits += 8; //1 byte = 8 bits => each byte read increment number of bit count to 8
		int bitTH = 7;
		while(bitTH >= 0)
		{
			if(((*temp) & ((0x01) << bitTH)) != 0)
			{
				n1 += 1;
			}
			else
			{
				n0 += 1;
			}
			bitTH -= 1;
		}
	}

	//Check if we have enough total bit count
	if(numbits < 10)
	{
		fprintf(stderr, "n = %d\n", numbits);
		fprintf(stderr, "x1: insufficient data\n");
	}
	else
	{
		fprintf(stdout, "n = %d\n", numbits);
		fprintf(stdout, "n0 = %d\n", n0);
		fprintf(stdout, "n1 = %d\n", n1);

		//calculate x1
		double result = (pow((double)n0-n1, (double)2)) / (double)numbits;
		fprintf(stdout, "x1 = %.6f\n", result);
		if(result - 3.8415 < 0.0000001) //X1 threshold = 3.8415
		{
			fprintf(stdout, "pass/fail = %s\n", "pass");
		}
		else
		{
			fprintf(stdout, "pass/fail = %s\n", "fail");
		}
	}

	free(temp);
}
