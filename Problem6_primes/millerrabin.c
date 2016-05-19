/*
 * millerrabin.c
 *
 *  Created on: Apr 6, 2016
 *      Author: ntran
 */

#include "millerrabin.h"

/*
 * Supplemental function to check and store maxitr input
 * @param: char pointer to maxitr
 * @output: struct myIntInput containing maxitr and isValid = 1 if input maxitr is valid, else isValid = 0
 */
struct myIntInput check_and_store_maxitr(char *inputMaxItr)
{
	struct myIntInput result;
	result.isValid = 1;

	char *temp_num = inputMaxItr;
	while(*temp_num != '\0' && *temp_num != '\n')
	{
		if(*temp_num >= '0' && *temp_num <= '9')
		{
			temp_num += 1;
		}
		else
		{
			fprintf(stderr, "Detect an invalid char '%c' in input maxitr\n", *temp_num);
			result.isValid = 0;
			return result;
		}
	}
	//Has not detected any weird char in maxitr except for only digits. Let's convert it into integer and return
	result.intInput = atoi(inputMaxItr);

	return result;
}

/*
 * convert into to char pointer
 * @param: int n
 * @output: char *
 */
char *itoa(int n)
{
	char *myreturn = (char *) malloc(4);
	char *mytemp = myreturn;
	char *temp = (char *) malloc(4);
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

/*
 * Supplemental function to find 2 and r in formula: n-1 = (2^s)*r such that r is odd
 * @param: BIGNUM *n
 * @output: struct mysr with int s and BIGNUM *r
 */
struct mysr find_s_r(BIGNUM *n)
{
	struct mysr result;
	result.isValid = 1;
	//Initialize s and r
	result.s = 0;

	result.r = NULL;
	result.r = BN_new();
	if(result.r == NULL)
	{
		fprintf(stderr, "Can't allocate space to store BUGNUM r in find_s_r function\n");
		result.isValid = 0;
		return result;
	}
	BN_zero(result.r);

	BIGNUM *holder_s = NULL;
	holder_s = BN_new();
	if(holder_s == NULL)
	{
		fprintf(stderr, "Can't allocate space to store BUGNUM holder_s for calculation in find_s_r function\n");
		result.isValid = 0;
		BN_free(result.r);
		return result;
	}
	BN_zero(holder_s);

	BIGNUM *bn_r = NULL;
	bn_r = BN_new();
	if(bn_r == NULL)
	{
		fprintf(stderr, "Can't allocate space to store BUGNUM bn_r for calculation in find_s_r function\n");
		result.isValid = 0;
		BN_free(result.r);
		BN_free(holder_s);
		return result;
	}
	BN_zero(bn_r);

	BIGNUM *bn_result = NULL;
	bn_result = BN_new();
	if(bn_result == NULL)
	{
		fprintf(stderr, "Can't allocate space to store BUGNUM bn_result for calculation in find_s_r function\n");
		result.isValid = 0;
		BN_free(result.r);
		BN_free(holder_s);
		BN_free(bn_r);
		return result;
	}
	BN_zero(bn_result);

	BIGNUM *bn_2 = NULL;
	bn_2 = BN_new();
	if(bn_2 == NULL)
	{
		fprintf(stderr, "Can't allocate space to store BUGNUM bn_2 for calculation in find_s_r function\n");
		result.isValid = 0;
		BN_free(result.r);
		BN_free(holder_s);
		BN_free(bn_r);
		BN_free(bn_result);
		return result;
	}
	BN_zero(bn_2);

	BIGNUM *bn_3 = NULL;
	bn_3 = BN_new();
	if(bn_3 == NULL)
	{
		fprintf(stderr, "Can't allocate space to store BUGNUM bn_3 for calculation in find_s_r function\n");
		result.isValid = 0;
		BN_free(result.r);
		BN_free(holder_s);
		BN_free(bn_r);
		BN_free(bn_result);
		BN_free(bn_2);
		return result;
	}
	BN_zero(bn_3);

	BIGNUM *bn_4 = NULL;
	bn_4 = BN_new();
	if(bn_4 == NULL)
	{
		fprintf(stderr, "Can't allocate space to store BUGNUM bn_4 for calculation in find_s_r function\n");
		result.isValid = 0;
		BN_free(result.r);
		BN_free(holder_s);
		BN_free(bn_r);
		BN_free(bn_result);
		BN_free(bn_2);
		BN_free(bn_3);
		return result;
	}
	BN_zero(bn_4);

	BIGNUM *bn_5 = NULL;
	bn_5 = BN_new();
	if(bn_5 == NULL)
	{
		fprintf(stderr, "Can't allocate space to store BUGNUM bn_5 for calculation in find_s_r function\n");
		result.isValid = 0;
		BN_free(result.r);
		BN_free(holder_s);
		BN_free(bn_r);
		BN_free(bn_result);
		BN_free(bn_2);
		BN_free(bn_3);
		BN_free(bn_4);
		return result;
	}
	BN_zero(bn_5);

	result.nsub1 = NULL;
	result.nsub1 = BN_new();
	if(result.nsub1 == NULL)
	{
		fprintf(stderr, "Can't allocate space to store BUGNUM bn_5 for calculation in find_s_r function\n");
		result.isValid = 0;
		BN_free(result.r);
		BN_free(holder_s);
		BN_free(bn_r);
		BN_free(bn_result);
		BN_free(bn_2);
		BN_free(bn_3);
		BN_free(bn_4);
		BN_free(bn_5);
		return result;
	}
	BN_zero(result.nsub1);

	BN_CTX *bn_ctx = NULL;
	bn_ctx = BN_CTX_new();
	if(bn_ctx == NULL)
	{
		//error
		fprintf(stderr, "Can't allocate space for bn_ctx while trying to perform floo(sqrt(num)) operation\n");
		result.isValid = 0;
		BN_free(result.r);
		BN_free(holder_s);
		BN_free(bn_r);
		BN_free(bn_result);
		BN_free(bn_2);
		BN_free(bn_2);
		BN_free(bn_3);
		BN_free(bn_4);
		BN_free(bn_5);
		BN_free(result.nsub1);
		return result;
	}
	BN_CTX_init(bn_ctx);

	char *temp = "1"; //used to decrement n
	BN_dec2bn(&bn_3, temp);

	char *temp1 = "2"; //used to perform 2^x
	BN_dec2bn(&bn_2, temp1);

	int isDone = 0;
	if(BN_sub(bn_r, n, bn_3))
	{
		BN_copy(result.nsub1, bn_r); //save n-1 to return
		while(!isDone)
		{
			result.s += 1;
			char *char_holder_s = itoa(result.s);
			BN_dec2bn(&holder_s, char_holder_s);
			free(char_holder_s); //free from itoa() function call
			//Perform 2^s
			if(BN_exp(bn_3, bn_2, holder_s, bn_ctx))
			{
				//(n-1)/(2^s)
				BN_div(bn_4, NULL, bn_r, bn_3, bn_ctx);
				//check if r is an odd
				BN_mod(bn_5, bn_4, bn_2, bn_ctx);
				char *temp2 = "0";
				BN_dec2bn(&bn_3, temp2);
				if(BN_cmp(bn_5, bn_3) != 0)
				{
					BN_copy(result.r, bn_4); // It is the calling function responsibility to free this variable
					isDone = 1;
				}
			}
			else
			{
				fprintf(stderr, "ERROR: 2^s operation fails in find_s_r function\n");
				result.isValid = 0;
				BN_free(result.r);
				BN_free(holder_s);
				BN_free(bn_r);
				BN_free(bn_result);
				BN_free(bn_2);
				BN_free(bn_2);
				BN_free(bn_3);
				BN_free(bn_4);
				BN_free(bn_5);
				BN_CTX_free(bn_ctx);
				BN_free(result.nsub1);
				return result;
			}
		}

	}
	else
	{
		fprintf(stderr, "ERROR: n-1 operation fails in find_s_r function\n");
		result.isValid = 0;
		BN_free(result.r);
		BN_free(holder_s);
		BN_free(bn_r);
		BN_free(bn_result);
		BN_free(bn_2);
		BN_free(bn_2);
		BN_free(bn_3);
		BN_free(bn_4);
		BN_free(bn_5);
		BN_CTX_free(bn_ctx);
		BN_free(result.nsub1);
		return result;
	}

	//Free all allocated space before return
	BN_free(holder_s);
	BN_free(bn_r);
	BN_free(bn_result);
	BN_free(bn_2);
	BN_free(bn_3);
	BN_free(bn_4);
	BN_free(bn_5);
	BN_CTX_free(bn_ctx);
	return result;
}

/*
 * Miller-Rabin function to test the primality of number
 * @param: numer and maxitr, and FILE fp pointing to primesfile, and optional print space
 * @output: return 1 if n is prime, else return 0 for other cases. Report error by returning -1
 * Return message if number is composite or prime. Report if failure occurs
 */
int millerrabin(BIGNUM *n, int maxitr, FILE *fp, int spaces)
{
	int isPrime = 0;
	//print n to output
	if(spaces == 0)
	{
		fprintf(stdout, "n = %s\n", BN_bn2dec(n));
	}

	struct mysr get_s_r = find_s_r(n);
	if(get_s_r.isValid)
	{
		for(int k = 0; k < spaces; k++)
		{
			fprintf(stdout, " ");
		}
		//Print out n-1, s and r
		fprintf(stdout, "  n-1 = %s\n", BN_bn2dec(get_s_r.nsub1));

		for(int k = 0; k < spaces; k++)
		{
			fprintf(stdout, " ");
		}
		fprintf(stdout, "  s = %d\n", get_s_r.s);

		for(int k = 0; k < spaces; k++)
		{
			fprintf(stdout, " ");
		}
		fprintf(stdout, "  r = %s\n", BN_bn2dec(get_s_r.r));

		int byte = 0;
		unsigned char *temp = malloc(4); //read 4 byte at a time which represents 1 uint32_t
		byte = fread(temp, 1 ,4 , fp); //read the first number and skip it since it is maxval

		BIGNUM *primes = NULL;
		primes = BN_new();
		if(primes == NULL)
		{
			fprintf(stderr, "Can't allocate space to hold prime number in millerrabin function\n");
			free(temp);
			BN_free(get_s_r.r); //free from function call find_s_r()
			BN_free(get_s_r.nsub1); //free from function call find_s_r()
			return -1;
		}
		BN_zero(primes);

		BIGNUM *holder = NULL;
		holder = BN_new();
		if(holder == NULL)
		{
			fprintf(stderr, "Can't allocate space for holder in millerrabin function\n");
			free(temp);
			BN_free(primes);
			BN_free(get_s_r.r); //free from function call find_s_r()
			BN_free(get_s_r.nsub1); //free from function call find_s_r()
			return -1;
		}
		BN_zero(holder);

		BIGNUM *y = NULL;
		y = BN_new();
		if(y == NULL)
		{
			fprintf(stderr, "Can't allocate space to hold y in millerrabin function\n");
			free(temp);
			BN_free(primes);
			BN_free(holder);
			BN_free(get_s_r.r); //free from function call find_s_r()
			BN_free(get_s_r.nsub1); //free from function call find_s_r()
			return -1;
		}
		BN_zero(y);

		BIGNUM *bn_1 = NULL;
		bn_1 = BN_new();
		if(bn_1 == NULL)
		{
			fprintf(stderr, "Can't allocate space to hold bn_1 in millerrabin function\n");
			free(temp);
			BN_free(primes);
			BN_free(holder);
			BN_free(get_s_r.r); //free from function call find_s_r()
			BN_free(get_s_r.nsub1); //free from function call find_s_r()
			BN_free(y);
			return -1;
		}
		BN_zero(bn_1);
		char *mytemp1 = "1";
		BN_dec2bn(&bn_1, mytemp1);

		BN_CTX *bn_ctx = NULL;
		bn_ctx = BN_CTX_new();
		if(bn_ctx == NULL)
		{
			fprintf(stderr, "Can't allocate space to hold bn_ctx in millerrabin function\n");
			free(temp);
			BN_free(primes);
			BN_free(holder);
			BN_free(y);
			BN_free(bn_1);
			BN_free(get_s_r.r); //free from function call find_s_r()
			BN_free(get_s_r.nsub1); //free from function call find_s_r()
			return -1;
		}
		BN_CTX_init(bn_ctx);

		for(int i = 1; i <= maxitr; i++)
		{
			if((byte = fread(temp, 1 ,4 , fp)) > 0)
			{
				BN_bin2bn(temp, 4, primes);
				//Compare primes to n-1
				if(BN_cmp(primes, get_s_r.nsub1) >= 0)
				{
					//failure
					for(int k = 0; k < spaces; k++)
					{
						fprintf(stdout, " ");
					}
					fprintf(stdout, "Miller-Rabin failure, maxitr is too large\n");

					free(temp);
					BN_free(get_s_r.r); //free from function call find_s_r()
					BN_free(get_s_r.nsub1); //free from function call find_s_r()
					BN_free(primes);
					BN_free(holder);
					BN_free(y);
					BN_free(bn_1);
					BN_CTX_free(bn_ctx);
					return isPrime;
				}

				//Compute y = (a^r) mod n
				BN_mod_exp(y, primes, get_s_r.r, n, bn_ctx);

				//Print output to stdout
				for(int k = 0; k < spaces; k++)
				{
					fprintf(stdout, " ");
				}
				fprintf(stdout, "  Itr %d of %d, a = %s, y = %s", i, maxitr, BN_bn2dec(primes), BN_bn2dec(y));
				if(BN_cmp(y, get_s_r.nsub1) == 0)
				{
					fprintf(stdout, " (which is n-1)\n");
				}
				else
				{
					fprintf(stdout, "\n");
				}

				//Check to see if y != 1 and y != n-1
				if((BN_cmp(y, bn_1) != 0) && (BN_cmp(y, get_s_r.nsub1) != 0))
				{
					for(int j = 1; (j <= get_s_r.s - 1) && (BN_cmp(y, get_s_r.nsub1) != 0); j++)
					{
						//copy y into holder
						BN_copy(holder, y);
						//computre y = (y^2) mod n
						BN_mod_sqr(y, holder, n, bn_ctx);

						//Print output to stdout
						for(int k = 0; k < spaces; k++)
						{
							fprintf(stdout, " ");
						}
						fprintf(stdout, "    j = %d of %d, y = %s", j, (get_s_r.s - 1), BN_bn2dec(y));
						if(BN_cmp(y, get_s_r.nsub1) == 0)
						{
							fprintf(stdout, " (which is n-1)\n");
						}
						else
						{
							fprintf(stdout, "\n");
						}

						if(BN_cmp(y, bn_1) == 0)
						{
							//composite
							for(int k = 0; k < spaces; k++)
							{
								fprintf(stdout, " ");
							}
							fprintf(stdout, "Miller-Rabin found a strong witness %s\n", BN_bn2dec(primes));

							free(temp);
							BN_free(get_s_r.r); //free from function call find_s_r()
							BN_free(get_s_r.nsub1); //free from function call find_s_r()
							BN_free(primes);
							BN_free(holder);
							BN_free(y);
							BN_free(bn_1);
							BN_CTX_free(bn_ctx);
							return isPrime;
						}
					}
					if(BN_cmp(y, get_s_r.nsub1) != 0)
					{
						//composite
						for(int k = 0; k < spaces; k++)
						{
							fprintf(stdout, " ");
						}
						fprintf(stdout, "Miller-Rabin found a strong witness %s\n", BN_bn2dec(primes));

						free(temp);
						BN_free(get_s_r.r); //free from function call find_s_r()
						BN_free(get_s_r.nsub1); //free from function call find_s_r()
						BN_free(primes);
						BN_free(holder);
						BN_free(y);
						BN_free(bn_1);
						BN_CTX_free(bn_ctx);
						return isPrime;
					}
				}
			}
			else
			{
				//error: can't retrieve data from primesfile
				fprintf(stderr, "ERROR: Can't read data from primesfile\n");
				free(temp);
				BN_free(get_s_r.r); //free from function call find_s_r()
				BN_free(get_s_r.nsub1); //free from function call find_s_r()
				BN_free(primes);
				BN_free(holder);
				BN_free(y);
				BN_free(bn_1);
				BN_CTX_free(bn_ctx);
				return -1;
			}
		}

		//return PRIME
		for(int k = 0; k < spaces; k++)
		{
			fprintf(stdout, " ");
		}
		fprintf(stdout, "Miller-Rabin declares n to be a prime number\n");
		isPrime = 1;

		free(temp);
		BN_free(get_s_r.r); //free from function call find_s_r()
		BN_free(get_s_r.nsub1); //free from function call find_s_r()
		BN_free(primes);
		BN_free(holder);
		BN_free(y);
		BN_free(bn_1);
		BN_CTX_free(bn_ctx);
		return isPrime;
	}
	else //find_s_r() return error(s)
	{
		return -1;
	}
}
