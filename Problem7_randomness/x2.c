/*
 * x2.c
 *
 *  Created on: Apr 19, 2016
 *      Author: ntran
 */

#include "x2.h"

/*
 * Supplemental function to calculate x2 given n, n0, n1, n00, n01, n10, n11
 * @param: int n, n1, n0, n00, n01, n10, n11
 * @return: x2 = (4/(n-1))*(n00^2 + n01^2 + n10^2 + n11^2) - ((2/n)*(n0^2 + n1^2)) + 1
 */
double calculate_x2(int n, int n0, int n1, int n00, int n01, int n10, int n11)
{
	double part1 = ((double)4/(n-1)) * (pow((double)n00,(double)2) + pow((double)n01,(double)2) + pow((double)n10,(double)2) + pow((double)n11,(double)2));
	double part2 = ((double)2/n) * (pow((double)n0,(double)2) + pow((double)n1,(double)2));

	double result = part1 - part2 + 1;
	return result;
}

/*
 * x2 - serial test
 * @param: FILE pointer to bit stream
 * @output: x2 test outcome
 */
void x2(FILE *fp)
{
	int numbits = 0;
	int n0 = 0;
	int n1 = 0;
	int n00 = 0;
	int n01 = 0;
	int n10 = 0;
	int n11 = 0;

	int firstBit = 1;
	int prev = -1;
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
				if(firstBit)
				{
					prev = 1; //first bit is 1
					firstBit = 0;
				}
				else
				{
					if(prev) //if prev is 1, and cur bit is 1
					{
						n11 += 1;
					}
					else
					{
						n01 += 1;
					}
					//update prev
					prev = 1;
				}
				n1 += 1;
			}
			else
			{
				if(firstBit)
				{
					prev = 0; //first bit is 0
					firstBit = 0;
				}
				else
				{
					if(prev) //if prev is 1, and cur bit is 0
					{
						n10 += 1;
					}
					else
					{
						n00 += 1;
					}
					//update prev
					prev = 0;
				}
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
				if(firstBit)
				{
					prev = 1; //first bit is 1
					firstBit = 0;
				}
				else
				{
					if(prev) //if prev is 1, and cur bit is 1
					{
						n11 += 1;
					}
					else
					{
						n01 += 1;
					}
					//update prev
					prev = 1;
				}
				n1 += 1;
			}
			else
			{
				if(firstBit)
				{
					prev = 0; //first bit is 0
					firstBit = 0;
				}
				else
				{
					if(prev) //if prev is 1, and cur bit is 0
					{
						n10 += 1;
					}
					else
					{
						n00 += 1;
					}
					//update prev
					prev = 0;
				}
				n0 += 1;
			}
			bitTH -= 1;
		}
	}

	//Check if we have enough total bit count
	if(numbits < 21)
	{
		fprintf(stderr, "n = %d\n", numbits);
		fprintf(stderr, "x2: insufficient data\n");
	}
	else
	{
		fprintf(stdout, "n = %d\n", numbits);
		fprintf(stdout, "n0 = %d\n", n0);
		fprintf(stdout, "n1 = %d\n", n1);
		fprintf(stdout, "n00 = %d\n", n00);
		fprintf(stdout, "n01 = %d\n", n01);
		fprintf(stdout, "n10 = %d\n", n10);
		fprintf(stdout, "n11 = %d\n", n11);

		//calculate x2
		double result = calculate_x2(numbits, n0, n1, n00, n01, n10, n11);
		fprintf(stdout, "x2 = %.6f\n", result);
		if(result - 5.9915 < 0.0000001) //X2 threshold is 5.9915
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
