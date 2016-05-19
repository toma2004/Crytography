/*
 * trialdiv.c
 *
 *  Created on: Apr 4, 2016
 *      Author: ntran
 */

#include "trialdiv.h"

/*
 * Supplemental function to read in number, verify if it is valid, and store it internally as BIGNUM type
 * @param: char pointer to number from args
 * @output: struct myNum where isValid=1 and BIGNUM containing the number when num is valid; else isValid = 0
 */
struct myNum read_verify_store_number(char *num)
{
	struct myNum result;
	result.isValid = 1;

	char *temp_num = num;
	while(*temp_num != '\0' && *temp_num != '\n')
	{
		if(*temp_num >= '0' && *temp_num <= '9')
		{
			temp_num += 1;
		}
		else
		{
			fprintf(stderr, "Detect an invalid char '%c' in input number\n", *temp_num);
			result.isValid = 0;
			return result;
		}
	}

	BIGNUM *bn_n = NULL;
	bn_n = BN_new();
	if(bn_n == NULL)
	{
		fprintf(stderr, "ERROR: can't allocate space to hold BIGNUM\n");
		result.isValid = 0;
		//free bn_n
		BN_free(bn_n);
		return result;
	}
	BN_zero(bn_n);

	if(BN_dec2bn(&bn_n, num))
	{
		char *temp = BN_bn2dec(bn_n);
		free(temp);

		//It is the calling function responsibility to free bn_n properly after use
		result.bn_n = bn_n;
	}
	else
	{
		fprintf(stderr, "ERROR: can't transform from dec to bn\n");
		result.isValid = 0;
		//free bn_n
		BN_free(bn_n);
		return result;
	}

	return result;
}

/*
 * Supplemental function to find floor(sqrt(BIGNUM number)) using brute force (try to square all number from 2 to when we have result >= BIGNUM number
 * @param: BIGNUM *mynum
 * @output: BIGNUM *numsqrt
 */
BIGNUM *floor_sqrt_bn(BIGNUM *myNum)
{
	BIGNUM *bn_r = NULL;
	BIGNUM *bn_p = NULL;
	BIGNUM *bn_a = NULL;
	BIGNUM *bn_temp = NULL;
	BN_CTX *bn_ctx = NULL;
	bn_r = BN_new();
	if(bn_r == NULL)
	{
		//error
		fprintf(stderr, "Can't allocate space for bn_r while trying to perform floo(sqrt(num)) operation\n");
		return NULL;
	}
	bn_p = BN_new();
	if(bn_p == NULL)
	{
		//error
		BN_free(bn_r);
		fprintf(stderr, "Can't allocate space for bn_p while trying to perform floo(sqrt(num)) operation\n");
		return NULL;
	}
	bn_a = BN_new();
	if(bn_a == NULL)
	{
		//error
		BN_free(bn_r);
		BN_free(bn_p);
		fprintf(stderr, "Can't allocate space for bn_a while trying to perform floo(sqrt(num)) operation\n");
		return NULL;
	}
	bn_temp = BN_new();
	if(bn_temp == NULL)
	{
		//error
		BN_free(bn_r);
		BN_free(bn_p);
		BN_free(bn_a);
		fprintf(stderr, "Can't allocate space for bn_temp while trying to perform floo(sqrt(num)) operation\n");
		return NULL;
	}
	bn_ctx = BN_CTX_new();
	if(bn_ctx == NULL)
	{
		//error
		BN_free(bn_r);
		BN_free(bn_p);
		BN_free(bn_a);
		BN_free(bn_temp);
		fprintf(stderr, "Can't allocate space for bn_ctx while trying to perform floo(sqrt(num)) operation\n");
		return NULL;
	}
	//Initialize my bn numbers
	BN_zero(bn_r);
	BN_zero(bn_p);
	BN_zero(bn_a);
	BN_zero(bn_temp);
	BN_CTX_init(bn_ctx);
	char *temp = "2"; //start with 2
	BN_dec2bn(&bn_a, temp);

	temp = "1"; //used to increment a by 1 step at a time
	BN_dec2bn(&bn_p, temp);

	BN_sqr(bn_r, bn_a, bn_ctx);
	int result = BN_cmp(bn_r, myNum);
	if(result == 0)
	{
		//free bn numbers
		BN_free(bn_r);
		BN_free(bn_p);
		BN_free(bn_temp);
		BN_CTX_free(bn_ctx);
		return bn_a; //It is the calling function to free this bn number
	}
	while(result < 0)
	{
		//copy a to temp in order to return temp in case sqr of a is greater than myNum
		BN_copy(bn_temp, bn_a);
		//increment a
		BN_add(bn_r, bn_a, bn_p);
		//copy value to a to update a
		BN_copy(bn_a, bn_r);
		//square a again
		BN_sqr(bn_r, bn_a, bn_ctx);
		//test the result
		result = BN_cmp(bn_r, myNum);
		if(result == 0)
		{
			//free bn numbers
			BN_free(bn_r);
			BN_free(bn_p);
			BN_free(bn_temp);
			BN_CTX_free(bn_ctx);
			return bn_a; //It is the calling function to free this bn number
		}
		else if(result == 1)
		{
			//free bn numbers
			BN_free(bn_r);
			BN_free(bn_p);
			BN_free(bn_a);
			BN_CTX_free(bn_ctx);
			return bn_temp; //It is the calling function to free this bn number
		}
	}
	//free bn numbers
	BN_free(bn_r);
	BN_free(bn_p);
	BN_free(bn_a);
	BN_free(bn_temp);
	BN_CTX_free(bn_ctx);
	return NULL;
}

/*
 * Version 2 of floor_sqrt_bn function using binary search to improve the run time
 * @param: BIGNUM *mynum
 * @output: floor(sqrt(mynum)
 */
BIGNUM *floor_sqrt_bn_ver2(BIGNUM *myNum)
{
	//return x if x = 0 or x = 1
	if(BN_is_zero(myNum) || BN_is_one(myNum))
	{
		return myNum;
	}

	BIGNUM *result = NULL;
	result = BN_new();
	if(result == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold result in floor_sqrt_bn_ver2() function\n");
		return NULL;
	}
	BN_zero(result);

	BIGNUM *hold_1 = NULL;
	hold_1 = BN_new();
	if(hold_1 == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold hold_1 in floor_sqrt_bn_ver2() function\n");
		BN_free(result);
		return NULL;
	}
	BN_zero(hold_1);
	char *temp_1 = "1";
	BN_dec2bn(&hold_1, temp_1);


	BIGNUM *hold_2 = NULL;
	hold_2 = BN_new();
	if(hold_2 == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold hold_2 in floor_sqrt_bn_ver2() function\n");
		BN_free(result);
		BN_free(hold_1);
		return NULL;
	}
	BN_zero(hold_2);
	char *temp_2 = "2";
	BN_dec2bn(&hold_2, temp_2);

	BIGNUM *start = NULL;
	start = BN_new();
	if(start == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold start in floor_sqrt_bn_ver2() function\n");
		BN_free(result);
		BN_free(hold_1);
		BN_free(hold_2);
		return NULL;
	}
	BN_zero(start);

	BN_CTX * bn_ctx = NULL;
	bn_ctx = BN_CTX_new();
	if(bn_ctx == NULL)
	{
		//error
		fprintf(stderr, "Can't allocate space for bn_ctx in floor_sqrt_bn_ver2() function\n");
		BN_free(result);
		BN_free(hold_1);
		BN_free(hold_2);
		BN_free(start);
		return NULL;
	}
	BN_CTX_init(bn_ctx);

	BIGNUM *end = NULL;
	end = BN_new();
	if(end == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold end in floor_sqrt_bn_ver2() function\n");
		BN_free(result);
		BN_free(hold_1);
		BN_free(hold_2);
		BN_free(start);
		BN_CTX_free(bn_ctx);
		return NULL;
	}
	BN_zero(end);
	//Set end = x/2 since anything greater than x/2 is not sqrt of x
	BN_div(end, NULL, myNum, hold_2, bn_ctx);

	BIGNUM *mid = NULL;
	mid = BN_new();
	if(mid == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold mid in floor_sqrt_bn_ver2() function\n");
		BN_free(result);
		BN_free(hold_1);
		BN_free(hold_2);
		BN_free(start);
		BN_CTX_free(bn_ctx);
		BN_free(end);
		return NULL;
	}
	BN_zero(mid);

	BIGNUM *sqr_mid = NULL;
	sqr_mid = BN_new();
	if(sqr_mid == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold sqr_mid in floor_sqrt_bn_ver2() function\n");
		BN_free(result);
		BN_free(hold_1);
		BN_free(hold_2);
		BN_free(start);
		BN_CTX_free(bn_ctx);
		BN_free(end);
		BN_free(mid);
		return NULL;
	}
	BN_zero(sqr_mid);

	BIGNUM *sum_start_end = NULL;
	sum_start_end = BN_new();
	if(sum_start_end == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold sum_start_end in floor_sqrt_bn_ver2() function\n");
		BN_free(result);
		BN_free(hold_1);
		BN_free(hold_2);
		BN_free(start);
		BN_CTX_free(bn_ctx);
		BN_free(end);
		BN_free(mid);
		BN_free(sqr_mid);
		return NULL;
	}
	BN_zero(sum_start_end);

	/*
	 * Piece of code below is borrowed from the idea on this website: http://www.geeksforgeeks.org/square-root-of-an-integer/
	 */
	while(BN_cmp(start, end) < 1) //while start <= end
	{
		BN_add(sum_start_end, start, end); //start + end
		BN_div(mid, NULL, sum_start_end, hold_2, bn_ctx); //sum of start + end / 2 to find mid
		//check if myNum is a perfect square
		BN_sqr(sqr_mid, mid, bn_ctx);
		int cmp_result = BN_cmp(sqr_mid, myNum);
		if(cmp_result == 0)
		{
			return mid; //found perfect square
		}
		else if(cmp_result < 0) //if mid sqr is less than myNum
		{
			BN_add(start, mid, hold_1); //update start = mid + 1
			BN_copy(result, mid);
		}
		else //if mid sqrt is greater than myNum
		{
			BN_sub(end, mid, hold_1); //update end = mid - 1
		}
	}
	/*
	 * End of idea borrowed
	 */

	//free allocated space
	BN_free(hold_1);
	BN_free(hold_2);
	BN_free(start);
	BN_CTX_free(bn_ctx);
	BN_free(end);
	BN_free(mid);
	BN_free(sqr_mid);
	BN_free(sum_start_end);

	return result; //It is the calling function responsibility to free

}

/*
 * Perform trialdiv to determine if a given num is proved to be prime
 * @param: BIGNUM pointer points to bn_n containing given num and FILE pointer points to primesfile, optional print space
 * @output: 3 scenarios:
 * 1/ n passes trial division test
 * 2/ n passes trial divisino test (not enough primes)
 * 3/ n is composite by trial division (mod m = 0)
 * return 0 if failure, else return 1. Return -1 in case if error
 */
int trialdiv(BIGNUM *myNum, FILE *fp, int spaces)
{
	int isError = 0; //used to keep track if error has occurred
	int isFailure = -1;
	int skipFirstNum = 0; //used to skip the first num in primesfiles as it is the maxval
	/*
	 * Compute floor(sqrt(myNum))
	 */
	BIGNUM *floor_sqrt_result = floor_sqrt_bn_ver2(myNum);

	if(floor_sqrt_result != NULL)
	{
		//fprintf(stdout, "my floor sqrt result = %s\n", BN_bn2dec(floor_sqrt_result));

		/*
		 * Start to read from primesfile, one prime at a time for trial
		 */
		BIGNUM *primes = NULL;
		primes = BN_new();
		if(primes == NULL)
		{
			//error
			fprintf(stderr, "Can't allocate space to store a new prime read from primesfile\n");
			BN_free(floor_sqrt_result);
			return -1;
		}
		BN_zero(primes);

		BIGNUM *rem = NULL;
		rem = BN_new();
		if(rem == NULL)
		{
			//error
			fprintf(stderr, "Can't allocate space to store reminder returned by bn_mod function\n");
			BN_free(floor_sqrt_result);
			BN_free(primes);
			return -1;
		}
		BN_zero(rem);

		BN_CTX *bn_ctx = NULL;
		bn_ctx = BN_CTX_new();
		if(bn_ctx == NULL)
		{
			//error
			fprintf(stderr, "Can't allocate space for bn_ctx in function trialdiv\n");
			BN_free(floor_sqrt_result);
			BN_free(primes);
			BN_free(rem);
			return -1;
		}
		BN_CTX_init(bn_ctx);

		int byte = 0;
		unsigned char *temp = malloc(4); //read 4 byte at a time which represents 1 uint32_t
		while((byte = fread(temp, 1, 4, fp)) > 0)
		{
			if(skipFirstNum)
			{
				//Convert from binary into bn
				if(BN_bin2bn(temp, 4, primes))
				{
					//fprintf(stdout, "my primes orig = %s\n", BN_bn2dec(primes));
					//Perform a check to see if primes > floor_sqrt_result
					if(BN_cmp(primes, floor_sqrt_result) == 1)
					{
						break; // we are done since primes > floor_sqrt_result
					}

					//fprintf(stdout, "my primes 1 = %s\n", BN_bn2dec(primes));
					//Trial test to check if myNum % current prime == 0
					if(BN_mod(rem, myNum, primes, bn_ctx))
					{
						//Check if rem is 0
						if(BN_is_zero(rem))
						{
							for(int k = 0; k < spaces; k++)
							{
								fprintf(stdout, " ");
							}
							fprintf(stdout, "n is composite by trial division (mod %s = 0)\n", BN_bn2dec(primes));
							isError = 1; //Set this flag so that we don't check for other 2 cases
							isFailure = 0;
							break; //found a divisor, done with the loop and exit
						}
					}
					else
					{
						fprintf(stderr, "ERROR: BN_div operation fails\n");
						isError = 1;
						break;
					}
				}
				else
				{
					//error
					fprintf(stderr, "BN_bin2bn operation fails\n");
					isError = 1;
					break;
				}
				//break;
			}
			//Skip the first num in primefiles since it is the maxval
			if(!skipFirstNum)
			{
				skipFirstNum = 1;
			}
		}

		if(!isError)
		{
			//Perform a check against the last primes number to see if we have enough primes
			if(BN_cmp(primes, floor_sqrt_result) == 1)
			{
				for(int k = 0; k < spaces; k++)
				{
					fprintf(stdout, " ");
				}
				fprintf(stdout, "n passes trial division test\n");
			}
			else
			{
				for(int k = 0; k < spaces; k++)
				{
					fprintf(stdout, " ");
				}
				fprintf(stdout, "n passes trial division test (not enough primes)\n");
			}
			isFailure = 1;
		}

		BN_free(primes);
		free(temp);
		BN_free(floor_sqrt_result); //free from function floor_sqrt_bn() call
		BN_free(rem);
		BN_CTX_free(bn_ctx);
		return isFailure;
	}
	else //floor_sqrt_bn() fails
	{
		return -1;
	}

}
