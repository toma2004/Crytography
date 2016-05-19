/*
 * primes.c
 *
 *  Created on: Apr 2, 2016
 *      Author: ntran
 */

#include "primes.h"

/*
 * Supplemental function to check if a given maxval is valid.
 * If it is, return maxval in uint32_t format and isValid = 1, else return isValid = 0
 */
struct mymaxval check_and_return_maxval(char *maxval)
{
	struct mymaxval result;
	result.isValid = 1;

	/*
	 * Based on specs, maxval is expected to be in range 2-2^24
	 * 2^24 = 16777216 - 8 digits
	 */
	uint32_t holder = 0x00;
	int count = 0;
	char *temp = maxval;
	while(*temp != '\0' && *temp != '\n')
	{
		if(*temp >= '0' && *temp <= '9')
		{
			count += 1;
		}
		else
		{
			fprintf(stderr, "Detect a non-digit char '%c' in maxval, please try another maxval input\n", *temp);
			result.isValid = 0;
			return result;
		}
		temp += 1;
	}

	//Check count to ensure that we are in the specified range
	if(count < 1)
	{
		fprintf(stderr, "It looks like given maxval is an empty string. Please double check your maxval input\n");
		result.isValid = 0;
		return result;
	}
	else if(count == 1) //Ensure that maxval is at least 2
	{
		holder = (uint32_t)atoi(maxval);
		if(holder >= 2)
		{
			result.maxval = holder;
		}
		else
		{
			fprintf(stderr, "Given maxval is less than 2, please double check your maxval input\n");
			result.isValid = 0;
			return result;
		}
	}
	else if(count >= 2 && count <= 8)
	{
		holder = (uint32_t)atoi(maxval);
		if(holder > 16777216)
		{
			fprintf(stderr, "Given maxval is larger than 2^24, please double check your maxval input\n");
			result.isValid = 0;
			return result;
		}
		else
		{
			result.maxval = holder;
		}
	}
	else
	{
		fprintf(stderr, "Given maxval is larger than 2^24, please double check your maxval input\n");
		result.isValid = 0;
		return result;
	}
	return result;
}

/*
 * Supplemental function to re-implement "ceil" function math.h as this is not defined on nunki
 * This function will round up a double to the nearest integer that is greater than or equal to given input
 * @param: input of type double, and int of that same number to compare against
 * @return: output of type int representing the round-up
 */
int ceil_func(double num, int d)
{
	double d_double = (double)d;
	if(fabs(num-d_double) < 0.000000001)
	{
		return d;
	}
	else
	{
		return d+1;
	}
}

/*
 * print all primes number from 2 to maxval using Sieve of Eratosthenes algorithm
 * @param: maxval with conditin that maxval is in range 2-2^24
 * @output: print all prime numbers from 2 to maxval
 */
void primes(uint32_t maxval)
{
	/*
	 * Construct an array of uint32_t, and each bit of this array represent an integer from 2 to maxval to save space
	 */
	int arr_size = ceil_func((double)maxval/32, (int)maxval/32);

	uint32_t *se = malloc(arr_size*sizeof(uint32_t));
	uint32_t *holder = malloc(sizeof(uint32_t));
	//initialize all bits to 0
	for(int k = 0; k < arr_size; k++)
	{
		se[k] = 0x00;
	}

	//Initialize byte and bit
	int byteTH = 0;
	int bitTH = 31;

	uint32_t p = 2;
	uint32_t j = 0;
	while (p*p <= maxval)
	{
		j = p*p;
		while (j <= maxval)
		{
			//Determine the byteTH and bitTH to be set based on j
			if (j < 8)
			{
				bitTH = 31-(int)j;
				byteTH = 0;
			}
			else
			{
				bitTH = 31 - ((int)j%32);
				byteTH = (int)j / 32;
			}
			//set all non-prime number bits to 1
			se[byteTH] |= ((0x01) << bitTH);
			j += p;
		}
		//Continue to increment p until se[p] = 0 which means that it has not been marked as non-prime numbers in previous rounds
		p += 1;
		bitTH = 31 - ((int)p%32);
		byteTH = (int)p / 32;
		while((se[byteTH] & ((0x01) << bitTH)) != 0)
		{
			p += 1;
			//re-calculate bitTH and byteTH
			bitTH = 31 - ((int)p%32);
			byteTH = (int)p / 32;
		}
	}

	//First to print out the maxval value
	*holder = htonl(maxval);
	fwrite(holder, 1, sizeof(uint32_t), stdout);

	//Then go through the array se and print out the index of those bits that are NOT set
	//Note that we will start at 29th bit of the first uint32_t number in se array since the first 2 bits represent 0 and 1
	byteTH = 0;
	bitTH = 29;
	uint32_t index = 2;
	while (index <= maxval)
	{
		if((se[byteTH] & ((0x01) << bitTH)) == 0)
		{
			*holder = htonl(index);
			fwrite(holder, 1, sizeof(uint32_t), stdout);
		}
		index += 1;
		bitTH -= 1;
		if(bitTH < 0)
		{
			byteTH += 1;
			bitTH = 31;
		}
	}

	free(holder);
	free(se);
}
