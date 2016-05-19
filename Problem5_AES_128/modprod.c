/*
 * modprod.c
 *
 *  Created on: Mar 13, 2016
 *      Author: ntran
 */

#include "modprod.h"

/*
 * Supplemental function to check if the poly inputs are valid. If so, store it into proper array
 * @param: char pointer to poly 1 and poly 2
 * @output: valid = 1 if both poly inputs are valid, else valid = 0. If valid, poly values are stored in struct mypoly
 */
struct mypoly check_and_store_poly_input(char *poly1, char *poly2)
{
	struct mypoly returnPoly;
	returnPoly.isValid = 1; //assume both poly values are valid

	char *temp_poly1 = poly1;
	char *temp_poly2 = poly2;
	int count_poly1 = 0;
	int count_poly2 = 0;
	unsigned char *store_poly1 = malloc(1);
	unsigned char *store_poly2 = malloc(1);
	uint8_t poly1_firstBits = 0x0;
	uint8_t poly1_lastBits = 0x0;
	uint8_t poly1_holder = 0x0;
	int poly1_index = 0;

	uint8_t poly2_firstBits = 0x0;
	uint8_t poly2_lastBits = 0x0;
	uint8_t poly2_holder = 0x0;
	int poly2_index = 0;

	while(*temp_poly1 != '\0' && *temp_poly1 != '\n' && *temp_poly2 != '\0' && *temp_poly2 != '\n')
	{
		//take care of poly 1
		if(*temp_poly1 >= '0' && *temp_poly1 <= '9')
		{
			*store_poly1 = (unsigned char)*temp_poly1 - 48; //representation of char 0-9 to hex of 0-9
			count_poly1 += 1;
		}
		else if(*temp_poly1 >= 'a' && *temp_poly1 <= 'f')
		{
			*store_poly1 = (unsigned char)*temp_poly1 - 87; //representation of char a-f to hex of 10-15
			count_poly1 += 1;
		}
		else
		{
			fprintf(stderr,"Detect invalid character '%c' in poly1 which should be represented in hexstring\n", *temp_poly1);
			returnPoly.isValid = 0;
			break;
		}
		//Take care of poly 2
		if(*temp_poly2 >= '0' && *temp_poly2 <= '9')
		{
			*store_poly2 = (unsigned char)*temp_poly2 - 48; //representation of char 0-9 to hex of 0-9
			count_poly2 += 1;
		}
		else if(*temp_poly2 >= 'a' && *temp_poly2 <= 'f')
		{
			*store_poly2 = (unsigned char)*temp_poly2 - 87; //representation of char a-f to hex of 10-15
			count_poly2 += 1;
		}
		else
		{
			fprintf(stderr,"Detect invalid character '%c' in poly2 which should be represented in hexstring\n", *temp_poly2);
			returnPoly.isValid = 0;
			break;
		}

		if (count_poly1 > 8) //expect to have 8 chars or 4 bytes for a(x) coefficient
		{
			fprintf(stderr,"Poly1 has too many values\n");
			returnPoly.isValid = 0;
			break;
		}
		if (count_poly2 > 8) //expect to have 8 chars or 4 bytes for a(x) coefficient
		{
			fprintf(stderr,"Poly2 has too many values\n");
			returnPoly.isValid = 0;
			break;
		}
		//Process poly 1
		if (count_poly1 % 2 !=  0)
		{
			poly1_firstBits = (uint8_t)*store_poly1;
		}
		else
		{
			poly1_lastBits = (uint8_t)*store_poly1;
			int temp1_index = 3;
			int temp2_index = 3;
			//now set the first 4 bits of holder to be poly1_firstBits, and last 4 bits of holder to be poly2_lastBits
			for(int k = 7; k >= 0; k--)
			{
				if(temp1_index >= 0)
				{
					if((poly1_firstBits & ((0x01) << temp1_index)) != 0) //bit is set, then set it in holder
					{
						poly1_holder |= ((0x01) << k);
					}
					temp1_index -= 1;
				}
				else
				{
					if((poly1_lastBits & ((0x01) << temp2_index)) != 0) //bit is set, then set it in holder
					{
						poly1_holder |= ((0x01) << k);
					}
					temp2_index -= 1;
				}
			}
			//Store this byte value into our poly1 array of struct mypoly.
			returnPoly.poly1[poly1_index] = poly1_holder;
			poly1_index += 1;
			//reset holder for next round
			poly1_holder = 0x0;
		}
		temp_poly1 += 1;

		//Process poly 2
		if (count_poly2 % 2 !=  0)
		{
			poly2_firstBits = (uint8_t)*store_poly2;
		}
		else
		{
			poly2_lastBits = (uint8_t)*store_poly2;
			int temp1_index = 3;
			int temp2_index = 3;
			//now set the first 4 bits of holder to be poly2_firstBits, and last 4 bits of holder to be poly2_lastBits
			for(int k = 7; k >= 0; k--)
			{
				if(temp1_index >= 0)
				{
					if((poly2_firstBits & ((0x01) << temp1_index)) != 0) //bit is set, then set it in holder
					{
						poly2_holder |= ((0x01) << k);
					}
					temp1_index -= 1;
				}
				else
				{
					if((poly2_lastBits & ((0x01) << temp2_index)) != 0) //bit is set, then set it in holder
					{
						poly2_holder |= ((0x01) << k);
					}
					temp2_index -= 1;
				}
			}
			//Store this byte value into our poly2 array of struct mypoly.
			returnPoly.poly2[poly2_index] = poly2_holder;
			poly2_index += 1;
			//reset holder for next round
			poly2_holder = 0x0;
		}
		temp_poly2 += 1;
	}
	if (count_poly1 < 8 && returnPoly.isValid == 1)
	{
		fprintf(stderr, "poly 1 has too few values\n");
		returnPoly.isValid = 0;
	}
	if (count_poly2 < 8 && returnPoly.isValid == 1)
	{
		fprintf(stderr, "poly 2 has too few values\n");
		returnPoly.isValid = 0;
	}
	if (count_poly1 == 8 && ((*temp_poly1 >= 'a' && *temp_poly1 <= 'z') || (*temp_poly1 >= '0' && *temp_poly1 <= '9')))
	{
		fprintf(stderr, "poly 1 has too many values\n");
		returnPoly.isValid = 0;
	}
	if (count_poly2 == 8 && ((*temp_poly2 >= 'a' && *temp_poly2 <= 'z') || (*temp_poly2 >= '0' && *temp_poly2 <= '9')))
	{
		fprintf(stderr, "poly 2 has too many values\n");
		returnPoly.isValid = 0;
	}

	free(store_poly1);
	free(store_poly2);

	return returnPoly;
}

/*
 * supplemental function to perform xtime(b) operation
 * @param: 2 uint8_t number
 * @return: uint8_t number as a result of {xx} bigdot {yy}
 */
uint8_t xtimeb(uint8_t bx, uint8_t x)
{
	uint8_t myreturn = 0x0;
	int keepTrack_ones[8]; //array is used to keep track of which location is x the bit is set
	for(int k = 0; k < 8; k++)
	{
		keepTrack_ones[k] = 0; //initialize keepTrack_ones array
	}
	//check x to see what is the highest poly required
	int foundFirstOne = 0;
	int v_array_size = 0;
	for(int k=7; k >= 0; k--)
	{
		if((x & ((0x01) << k)) != 0)
		{
			if (!foundFirstOne)
			{
				foundFirstOne = 1;
				v_array_size = k+1;
			}
			keepTrack_ones[k] = 1;
		}
	}
	//allocate memory for array v
	uint8_t *arrayV = malloc(v_array_size);
	//initialize v[0] to be bx
	arrayV[0] = bx;
	for(int k=1; k < v_array_size; k++)
	{
		arrayV[k] = arrayV[k-1] << 1;
		//check if the most significant bit in previous v is set, if so, (XOR) with {1B}. Else, just do left shift
		if ((arrayV[k-1] & ((0x01) << 7)) != 0)
		{
			arrayV[k] = arrayV[k] ^ 0x1b;
		}
	}
	//XOR all the uint8_t value in the arrayV whose index is corresponding to the index of keepTrack_ones set to 1
	foundFirstOne = 0;
	for(int k = 7; k >= 0; k--)
	{
		if(keepTrack_ones[k] == 1)
		{
			if(!foundFirstOne)
			{
				myreturn = arrayV[k];
				foundFirstOne = 1;
			}
			else
			{
				myreturn ^= arrayV[k];
			}
		}
	}
	free(arrayV);
	return myreturn;
}

/*
 * modprod program to perform circleX operation of 2 poly
 * @param: 2 poly, and 1 flag for printing output
 * @return: 1 poly as a result of circleX operation of 2 input poly
 */
struct modprod_result modprod(uint8_t poly1[], uint8_t poly2[], int printFlag)
{
	struct modprod_result myresult;
	myresult.modprod_poly[0] = xtimeb(poly1[0], poly2[3]) ^ xtimeb(poly1[1], poly2[2]) ^ xtimeb(poly1[2], poly2[1]) ^ xtimeb(poly1[3],poly2[0]);
	myresult.modprod_poly[1] = xtimeb(poly1[1], poly2[3]) ^ xtimeb(poly1[2], poly2[2]) ^ xtimeb(poly1[3], poly2[1]) ^ xtimeb(poly1[0],poly2[0]);
	myresult.modprod_poly[2] = xtimeb(poly1[2], poly2[3]) ^ xtimeb(poly1[3], poly2[2]) ^ xtimeb(poly1[0], poly2[1]) ^ xtimeb(poly1[1],poly2[0]);
	myresult.modprod_poly[3] = xtimeb(poly1[3], poly2[3]) ^ xtimeb(poly1[0], poly2[2]) ^ xtimeb(poly1[1], poly2[1]) ^ xtimeb(poly1[2],poly2[0]);

	//print output of modprod
	if(printFlag == 1)
	{
		fprintf(stdout,"{%02x}{%02x}{%02x}{%02x} CIRCLEX {%02x}{%02x}{%02x}{%02x} = {%02x}{%02x}{%02x}{%02x}\n",poly1[0],poly1[1],poly1[2],poly1[3],poly2[0],poly2[1],poly2[2],poly2[3],myresult.modprod_poly[0],myresult.modprod_poly[1],myresult.modprod_poly[2],myresult.modprod_poly[3]);
	}
	return myresult;
}
