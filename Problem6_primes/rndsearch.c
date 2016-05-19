/*
 * rndsearch.c
 *
 *  Created on: Apr 10, 2016
 *      Author: ntran
 */

#include "rndsearch.h"

/*
 * Supplemental function to return BIGNUM containing value n = RndOddNum(k) algorithm
 * @param: k numbits and char pointer to binary read in using x = ceil(k/8) bytes
 * @return: BIGNUM n
 */
BIGNUM *RndOddNum(int k, char *temp, int x_byte)
{
	BIGNUM *result = NULL;

	result = BN_new();
	if(result == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold returned result for RndOddNum function\n");
		return NULL;
	}
	BN_zero(result);

	if(BN_bin2bn((unsigned char*)temp, x_byte, result) != NULL)
	{
		//Set bit 0 and k-1
		if(BN_set_bit(result, 0))
		{
			if(BN_set_bit(result, k-1))
			{
				//Get total number of bits in result
				int total_num_bits = BN_num_bits(result);
				//Clear all higher order bit
				while(k < total_num_bits)
				{
					if(BN_clear_bit(result, k) == 0) //error: can't clear bit
					{
						//error: can't clear bit
						fprintf(stderr, "ERROR: can't clear bit %dth\n", k);
						BN_free(result);
						return NULL;
					}
					k += 1;
				}

			}
			else
			{
				//error: can't set bit k-1
				fprintf(stderr, "ERROR: can't set bit k-1\n");
				BN_free(result);
				return NULL;
			}
		}
		else
		{
			//error: can't set bit 0
			fprintf(stderr, "ERROR: can't set bit 0\n");
			BN_free(result);
			return NULL;
		}
	}
	else
	{
		//error: can't convert from bin to bn
		fprintf(stderr, "ERROR: can't convert binary read from rndfile to bn\n");
		BN_free(result);
		return NULL;
	}

	return result; //It is the calling function responsibility to free BIGNUM result
}

/*
 * rndsearch function to generate numbits-bit probable prime
 * @param: k numbits, maxitr, FILE pointer to primesfile, FILE pointer to rndfile
 * @output: Print out rndsearch process and generate a probable prime
 */
void rndsearch(int k, int maxitr, FILE *fp, FILE *fp_rndfile)
{
	int byte = 0;
	int x_byte = ceil_func((double)k/8, (int)k/8);

	char *temp = malloc(x_byte); //read in x_byte from rndfile
	int itr_count = 1;
	while((byte = fread(temp, 1, x_byte, fp_rndfile)) > 0)
	{
		BIGNUM *bn_n = RndOddNum(k, temp, x_byte);
		if(bn_n == NULL)
		{
			free(temp);
			return; //ERROR has happened in RndOddNum() function call
		}
		else
		{
			//Print itr
			fprintf(stdout, "RANDOM-SEARCH: iteration %d\n", itr_count);
			itr_count += 1;
			fprintf(stdout, "  n = %s\n", BN_bn2dec(bn_n));

			//Use trial division function to check if n passes trial division test
			int trialdiv_returnCode = trialdiv(bn_n, fp, 2);
			//reset fp pointer to beginning of primesfile for next time use
			rewind(fp);
			if(trialdiv_returnCode == -1)
			{
				//error has happened in trialdiv function. Terminate this function
				free(temp);
				BN_free(bn_n); //free from RndOddNum() function call
				return;
			}
			else if(trialdiv_returnCode == 0)
			{
				BN_free(bn_n);
				continue; //bn_n does not pass trial division test. Continue to generate next number
			}

			int millerrabin_returnCode = millerrabin(bn_n, maxitr, fp, 2);
			//reset fp pointer to beginning of primesfile for next time use
			rewind(fp);
			if(millerrabin_returnCode == -1)
			{
				//error has happened in millerrabin function. Terminate this function
				free(temp);
				BN_free(bn_n); //free from RndOddNum() function call
				return;
			}
			else if(millerrabin_returnCode == 1)
			{
				free(temp);
				BN_free(bn_n); //free from RndOddNum() function call
				return; //Prove n to be a prime number. End function
			}
			else
			{
				//continue generate the next probable prime
				BN_free(bn_n);
			}
		}
	}
	//Free in case we have never generated a bn_n that passes both tests
	free(temp);
	fprintf(stderr, "Run out of bytes in rndfile and we have not successfully generated a prime number\n");
}
