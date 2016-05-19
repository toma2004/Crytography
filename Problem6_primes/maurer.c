/*
 * maurer.c
 *
 *  Created on: Apr 11, 2016
 *      Author: ntran
 */

#include "maurer.h"

/*
 * Supplemental function to act as RndOddNum wrapper
 * @param: k, and FILE pointer to fp_rndfile
 * @return: BIGNUM *bn_n
 */
BIGNUM *RndOddNum_wrapper(int k, FILE *fp_rndfile)
{
	int x_byte = ceil_func((double)k/8, (int)k/8);

	char *temp = malloc(x_byte); //read in x_byte from rndfile
	int byte = 0;
	if((byte = fread(temp, 1, x_byte, fp_rndfile)) == x_byte) //check to see if we have read enough bytes
	{
		BIGNUM *result = RndOddNum(k, temp, x_byte);
		if(result == NULL)
		{
			free(temp);
			return NULL;
		}
		free(temp);
		return result; //It is the calling function responsibility to free
	}
	else
	{
		fprintf(stderr, "rndfile is out byte. Exiting program...\n");
		free(temp);
		return NULL;
	}
}

/*
 * Supplemental function to read a byte from rndfile
 * @param: FILE pointer to fp_rndfile
 * @return: unsigned char of byte read
 */
unsigned char *RndByte(FILE *fp_rndfile)
{
	unsigned char *temp = malloc(1);
	int byte = 0;
	if((byte = fread(temp, 1, 1, fp_rndfile)) == 1) //check to see if we have read enough bytes
	{
		return temp; //It is the calling function responsibility to free
	}
	else
	{
		fprintf(stderr, "rndfile is out byte. Exiting program...\n");
		free(temp);
		return NULL;
	}
}

/*
 * Supplemental function to calculate gcd(x,y)
 * @param: BIGNUM *bn_x, BIGNUM *bn_y
 * @return: gcd(x,y)
 */
BIGNUM *gcd(BIGNUM *bn_x, BIGNUM *bn_y)
{
	BIGNUM *q = NULL;
	q = BN_new();
	if(q == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold quotient in gcd() function\n");
		return NULL;
	}
	BN_zero(q);

	BIGNUM *r = NULL;
	r = BN_new();
	if(r == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold remainder in gcd() function\n");
		BN_free(q);
		return NULL;
	}
	BN_zero(r);

	BIGNUM *holder = NULL;
	holder = BN_new();
	if(holder == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold holder in gcd() function\n");
		BN_free(q);
		BN_free(r);
		return NULL;
	}
	BN_zero(holder);

	BN_CTX * bn_ctx = NULL;
	bn_ctx = BN_CTX_new();
	if(bn_ctx == NULL)
	{
		//error
		fprintf(stderr, "Can't allocate space for bn_ctx in gcd() function\n");
		BN_free(q);
		BN_free(r);
		BN_free(holder);
		return NULL;
	}
	BN_CTX_init(bn_ctx);

	if(BN_cmp(bn_x, bn_y) < 0) //bn_y > bn_x. Always make bn_x > bn_y so that bn_x is dividend and bn_y is divisor
	{
		//initialize bn_y to be q
		BN_copy(q, bn_y);

		//initialize bn_x to be r
		BN_copy(r, bn_x);

	}
	else
	{
		//initialize bn_x to be q
		BN_copy(q, bn_x);

		//initialize bn_y to be r
		BN_copy(r, bn_y);
	}

	//Perform the first division
	BN_nnmod(holder, q, r, bn_ctx);

	while(!BN_is_zero(holder))
	{
		BN_copy(q, r);
		BN_copy(r, holder);
		//BN_div(q, r, bn_x, bn_y, bn_ctx);
		BN_nnmod(holder, q, r, bn_ctx);
	}

	//free allocated space
	BN_free(q);
	BN_free(holder);
	BN_CTX_free(bn_ctx);
	return r; //It is the calling function responsibility to free
}

/*
 * Supplemental function to calculate BIGNUM * I = floor(2^(k-2) /q)
 * @param: int (k-2), BIGNUM *q
 * @return: BIGNUM *I
 */
BIGNUM *calculate_I(int k2, BIGNUM *q)
{
	BIGNUM *result = NULL;
	result = BN_new();
	if(result == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold result in calculate_I() function\n");
		return NULL;
	}
	BN_zero(result);

	BIGNUM *k2_holder = NULL;
	k2_holder = BN_new();
	if(k2_holder == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold k2_holder in calculate_I() function\n");
		BN_free(result);
		return NULL;
	}
	BN_zero(k2_holder);

	char *temp = itoa(k2);
	BN_dec2bn(&k2_holder, temp);
	free(temp);

	BIGNUM *holder_2 = NULL;
	holder_2 = BN_new();
	if(holder_2 == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold holder_2 in calculate_I() function\n");
		BN_free(result);
		BN_free(k2_holder);
		return NULL;
	}
	BN_zero(holder_2);

	char *temp_2 = "2";
	BN_dec2bn(&holder_2, temp_2);

	BIGNUM *exp_holder = NULL;
	exp_holder = BN_new();
	if(exp_holder == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold exp_holder in calculate_I() function\n");
		BN_free(result);
		BN_free(k2_holder);
		BN_free(holder_2);
		return NULL;
	}
	BN_zero(exp_holder);

	BN_CTX * bn_ctx = NULL;
	bn_ctx = BN_CTX_new();
	if(bn_ctx == NULL)
	{
		//error
		fprintf(stderr, "Can't allocate space for bn_ctx in calculate_I() function\n");
		BN_free(result);
		BN_free(k2_holder);
		BN_free(holder_2);
		BN_free(exp_holder);
		return NULL;
	}
	BN_CTX_init(bn_ctx);

	//Calculate 2^(k2)
	BN_exp(exp_holder, holder_2, k2_holder, bn_ctx);

	//Perform div and take only the quotient - same as perform floor
	BN_div(result, NULL, exp_holder, q, bn_ctx);

	//free allocated space
	BN_free(k2_holder);
	BN_free(holder_2);
	BN_free(exp_holder);
	BN_CTX_free(bn_ctx);

	return result; //It is the calling function responsibility to free

}

/*
 * Supplemental function to calculate BIGNUM *R = (R mod I) + I + 1
 * @param: BIGNUM *R and *I
 * @return: BIGNUM *R
 */
BIGNUM *calculate_R(BIGNUM *R, BIGNUM *I)
{
	BIGNUM *result = NULL;
	result = BN_new();
	if(result == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold result in calculate_R() function\n");
		return NULL;
	}
	BN_zero(result);

	BIGNUM *holder_1 = NULL;
	holder_1 = BN_new();
	if(holder_1 == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold holder_1 in calculate_R() function\n");
		BN_free(result);
		return NULL;
	}
	BN_zero(holder_1);

	char *temp_1 = "1";
	BN_dec2bn(&holder_1, temp_1);

	BIGNUM *mod_holder = NULL;
	mod_holder = BN_new();
	if(mod_holder == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold mod_holder in calculate_R() function\n");
		BN_free(result);
		BN_free(holder_1);
		return NULL;
	}
	BN_zero(mod_holder);

	BIGNUM *sum_holder = NULL;
	sum_holder = BN_new();
	if(sum_holder == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold sum_holder in calculate_R() function\n");
		BN_free(result);
		BN_free(mod_holder);
		BN_free(holder_1);
		return NULL;
	}
	BN_zero(sum_holder);

	BN_CTX * bn_ctx = NULL;
	bn_ctx = BN_CTX_new();
	if(bn_ctx == NULL)
	{
		//error
		fprintf(stderr, "Can't allocate space for bn_ctx in calculate_R() function\n");
		BN_free(result);
		BN_free(sum_holder);
		BN_free(holder_1);
		BN_free(mod_holder);
		return NULL;
	}
	BN_CTX_init(bn_ctx);

	//Perform R mod I
	BN_nnmod(mod_holder, R, I, bn_ctx);
	//Perform mod_holder + I
	BN_add(sum_holder, mod_holder, I);
	//Perform sum_holder + 1 to get final result
	BN_add(result, sum_holder, holder_1);

	//Free allocated space
	BN_free(sum_holder);
	BN_free(holder_1);
	BN_free(mod_holder);
	BN_CTX_free(bn_ctx);

	return result; //It is the calling function responsibility to free
}

/*
 * Supplemental function to calculate n = 2*R*q + 1
 * @param: BIGNUM *R, *q
 * @return: BIGNUM *n
 */
BIGNUM *calculate_n(BIGNUM *R, BIGNUM *q)
{
	BIGNUM *result = NULL;
	result = BN_new();
	if(result == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold result in calculate_n() function\n");
		return NULL;
	}
	BN_zero(result);

	BIGNUM *holder_1 = NULL;
	holder_1 = BN_new();
	if(holder_1 == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold holder_1 in calculate_n() function\n");
		BN_free(result);
		return NULL;
	}
	BN_zero(holder_1);

	char *temp_1 = "1";
	BN_dec2bn(&holder_1, temp_1);

	BIGNUM *mul_holder = NULL;
	mul_holder = BN_new();
	if(mul_holder == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold mul_holder in calculate_n() function\n");
		BN_free(result);
		BN_free(holder_1);
		return NULL;
	}
	BN_zero(mul_holder);

	BIGNUM *holder_2 = NULL;
	holder_2 = BN_new();
	if(holder_2 == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold holder_2 in calculate_n() function\n");
		BN_free(result);
		BN_free(holder_1);
		BN_free(mul_holder);
		return NULL;
	}
	BN_zero(holder_2);

	char *temp_2 = "2";
	BN_dec2bn(&holder_2, temp_2);

	BIGNUM *mul_holder_1 = NULL;
	mul_holder_1 = BN_new();
	if(mul_holder_1 == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold mul_holder_1 in calculate_n() function\n");
		BN_free(result);
		BN_free(holder_1);
		BN_free(mul_holder);
		BN_free(holder_2);
		return NULL;
	}
	BN_zero(mul_holder_1);

	BIGNUM *sum_holder = NULL;
	sum_holder = BN_new();
	if(sum_holder == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold sum_holder in calculate_n() function\n");
		BN_free(result);
		BN_free(holder_1);
		BN_free(mul_holder);
		BN_free(holder_2);
		BN_free(mul_holder_1);
		return NULL;
	}
	BN_zero(sum_holder);

	BN_CTX * bn_ctx = NULL;
	bn_ctx = BN_CTX_new();
	if(bn_ctx == NULL)
	{
		//error
		fprintf(stderr, "Can't allocate space for bn_ctx in calculate_n() function\n");
		BN_free(result);
		BN_free(holder_1);
		BN_free(mul_holder);
		BN_free(holder_2);
		BN_free(mul_holder_1);
		BN_free(sum_holder);
		return NULL;
	}
	BN_CTX_init(bn_ctx);

	//perform 2*R
	BN_mul(mul_holder, holder_2, R, bn_ctx);
	//perform mul_holder * q
	BN_mul(mul_holder_1, mul_holder, q, bn_ctx);
	//perform mul_holder_1 + 1 to get final result
	BN_add(result, mul_holder_1, holder_1);

	//free allocated space
	BN_free(holder_1);
	BN_free(mul_holder);
	BN_free(holder_2);
	BN_free(mul_holder_1);
	BN_free(sum_holder);
	BN_CTX_free(bn_ctx);

	return result; //It is the calling function responsibility to free
}

/*
 * Supplemental function to help compare if BIGNUM *a <= 1 or *a >= n-1
 * @param: BIGNUM *a, and *n
 * @return: 0 if false, 1 if true
 */
int cmp_a(BIGNUM *a, BIGNUM *bn_n)
{
	int isTrue = 1;

	BIGNUM *holder_1 = NULL;
	holder_1 = BN_new();
	if(holder_1 == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold holder_1 in cmp_a() function\n");
		return -1;
	}
	BN_zero(holder_1);

	char *temp_1 = "1";
	BN_dec2bn(&holder_1, temp_1);

	BIGNUM *sub_holder = NULL;
	sub_holder = BN_new();
	if(sub_holder == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold sub_holder in cmp_a() function\n");
		BN_free(holder_1);
		return -1;
	}
	BN_zero(sub_holder);

	//perform n-1
	BN_sub(sub_holder, bn_n, holder_1);

	//Start comparison
	if(BN_cmp(a, holder_1) == 1 && BN_cmp(a, sub_holder) == -1)
	{
		isTrue = 0; //send a stop signal
	}

	//Free allocate space
	BN_free(holder_1);
	BN_free(sub_holder);

	return isTrue;
}

/*
 * Supplemental function to calculate b where b = a^{n-1} mod n
 * @param: BIGNUM *a and *bn_n
 * @return: BIGNUM *b
 */
BIGNUM *calculate_b(BIGNUM *a, BIGNUM *bn_n)
{
	BIGNUM *result = NULL;
	result = BN_new();
	if(result == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold result in calculate_b() function\n");
		return NULL;
	}
	BN_zero(result);

	BIGNUM *holder_1 = NULL;
	holder_1 = BN_new();
	if(holder_1 == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold holder_1 in calculate_b() function\n");
		BN_free(result);
		return NULL;
	}
	BN_zero(holder_1);

	char *temp_1 = "1";
	BN_dec2bn(&holder_1, temp_1);

	BIGNUM *sub_holder = NULL;
	sub_holder = BN_new();
	if(sub_holder == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold sub_holder in calculate_b() function\n");
		BN_free(holder_1);
		BN_free(result);
		return NULL;
	}
	BN_zero(sub_holder);

	//perform n-1
	BN_sub(sub_holder, bn_n, holder_1);

	BN_CTX * bn_ctx = NULL;
	bn_ctx = BN_CTX_new();
	if(bn_ctx == NULL)
	{
		//error
		fprintf(stderr, "Can't allocate space for bn_ctx in calculate_n() function\n");
		BN_free(result);
		BN_free(holder_1);
		BN_free(sub_holder);
		return NULL;
	}
	BN_CTX_init(bn_ctx);

	//Perform a^{n-1} mod n
	BN_mod_exp(result, a, sub_holder, bn_n, bn_ctx);

	//Free allocated space
	BN_free(holder_1);
	BN_free(sub_holder);
	BN_CTX_free(bn_ctx);

	return result; //It is the calling function responsibility to free
}

/*
 * Supplemental function to calculate b where b = a^{2R} mod n
 * @param: BIGNUM *a and *bn_n, *R
 * @return: BIGNUM *b
 */
BIGNUM *calculate_b_1(BIGNUM *a, BIGNUM *bn_n, BIGNUM *R)
{
	BIGNUM *result = NULL;
	result = BN_new();
	if(result == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold result in calculate_b_1() function\n");
		return NULL;
	}
	BN_zero(result);

	BIGNUM *mul_holder = NULL;
	mul_holder = BN_new();
	if(mul_holder == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold mul_holder in calculate_b_1() function\n");
		BN_free(result);
		return NULL;
	}
	BN_zero(mul_holder);

	BIGNUM *holder_2 = NULL;
	holder_2 = BN_new();
	if(holder_2 == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold holder_2 in calculate_b_1() function\n");
		BN_free(result);
		BN_free(mul_holder);
		return NULL;
	}
	BN_zero(holder_2);

	char *temp_2 = "2";
	BN_dec2bn(&holder_2, temp_2);


	BN_CTX * bn_ctx = NULL;
	bn_ctx = BN_CTX_new();
	if(bn_ctx == NULL)
	{
		//error
		fprintf(stderr, "Can't allocate space for bn_ctx in calculate_n() function\n");
		BN_free(result);
		BN_free(holder_2);
		BN_free(mul_holder);
		return NULL;
	}
	BN_CTX_init(bn_ctx);

	//Perform 2*R
	BN_mul(mul_holder, holder_2, R, bn_ctx);
	//Perform a^{2R} mod n
	BN_mod_exp(result, a, mul_holder, bn_n, bn_ctx);


	//Free allocated space
	BN_free(holder_2);
	BN_free(mul_holder);
	BN_CTX_free(bn_ctx);

	return result; //It is the calling function responsibility to free
}

/*
 * Supplemental function to calculate b-1
 * @param: BIGNUM *b
 * @reutrn: BIGNUM *b-1
 */
BIGNUM *sub_1(BIGNUM *b)
{
	BIGNUM *result = NULL;
	result = BN_new();
	if(result == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold result in sub_1() function\n");
		return NULL;
	}
	BN_zero(result);

	BIGNUM *holder_1 = NULL;
	holder_1 = BN_new();
	if(holder_1 == NULL)
	{
		fprintf(stderr, "Can't allocate space to hold holder_1 in sub_1() function\n");
		BN_free(result);
		return NULL;
	}
	BN_zero(holder_1);

	char *temp_1 = "1";
	BN_dec2bn(&holder_1, temp_1);

	//perform n-1
	BN_sub(result, b, holder_1);

	//Free allocated space
	BN_free(holder_1);

	return result; //It is the calling function responsibility to free
}

/*
 * Supplemental function to convert binary into decimal to print
 * @param: unsigned char* point to the binary
 * @return: decimal representation of the given binary
 */
uint32_t bin_2_dec(unsigned char* bin)
{
	uint32_t sum = 0;
	//Go through all bits in bin to check which bit is set
	for(int bitTH = 0; bitTH < 8; bitTH ++)
	{
		if(((*bin) & ((0x01) << bitTH)) != 0)
		{
			sum += pow(2,bitTH);
		}
	}
	return sum;
}

/*
 * Maurer algorithm
 * @param: k numbits, level to print to stdout, FILE pointer to primesfile, FILE pointer to rndfile
 * @output: generate and print numbits-bit provable prime using Maurer algorithm
 */
BIGNUM* maurer(int k, int level, FILE *fp, FILE *fp_rndfile)
{
	//Print level and k
	fprintf(stdout, "Maurer: level %d, k=%d\n", level, k);

	BIGNUM *n = NULL;
	if(k <= 20)
	{
		while(1)
		{
			n = RndOddNum_wrapper(k, fp_rndfile);
			if(n == NULL)
			{
				//error
				return NULL;
			}
			else
			{
				//Print value of n
				fprintf(stdout, "  step 1.1, n = %s\n", BN_bn2dec(n));

				//Use trial division function to check if n passes trial division test
				rewind(fp); //ensure that we are at the beginning of primesfile
				int trialdiv_returnCode = trialdiv(n, fp, 4);
				if(trialdiv_returnCode == -1)
				{
					//error has happened in trialdiv function. Terminate this function
					if( n != NULL)
					{
						BN_free(n);
					}
					return NULL;
				}
				else if(trialdiv_returnCode == 1)
				{
					if(level == 0)
					{
						fprintf(stdout, "\nMaurer's Algorithm found an %d-bit prime:\n", BN_num_bits(n));
						fprintf(stdout, "  n = %s\n", BN_bn2dec(n));
					}
					return n;
				}
			}
		}
	}

	//double c = 0.1;
	int m = 20;
	double r = 0.0;
	int whatToPrint = 0;
	if(k <= (2*m))
	{
		r = 0.5;
	}
	else
	{
		whatToPrint = 1;
		while(1)
		{
			unsigned char* temp = RndByte(fp_rndfile);
			if(temp == NULL)
			{
				//error has happened in RndByte() function.
				return NULL;
			}
			r = (double)*temp / 255.0;
			r = 0.5 + r / 2.0;
			if(k*(1-r) > (double)m)
			{
				fprintf(stdout, "  step 4: random byte = %d, ", bin_2_dec(temp));
				free(temp);
				break;
			}
			free(temp);
		}
	}
	if(whatToPrint)
	{
		fprintf(stdout, "r = %d%%\n", round(r*100));
	}
	else
	{
		fprintf(stdout, "  step 4, r = %d%%\n", round(r*100));
	}

	//Recursive function
	BIGNUM *q = maurer(floor(r*k)+1, level+1, fp, fp_rndfile);
	if(q == NULL)
	{
		//Error has happened in recursive function call
		if(n != NULL)
		{
			BN_free(n);
		}
		return NULL;

	}

	//print current level, k and q
	fprintf(stdout, "Maurer: back to level %d, k=%d, q=%s\n", level, k, BN_bn2dec(q));

	int num_bits_in_q = BN_num_bits(q);
	BIGNUM *I = calculate_I(k-2, q);
	if(I == NULL)
	{
		//error
		if(n != NULL)
		{
			BN_free(n);
		}
		if(q != NULL)
		{
			BN_free(q);
		}
		return NULL;
	}
	else
	{
		int itr = 1;
		while(1)
		{
			BIGNUM *R = RndOddNum_wrapper(k+1-num_bits_in_q, fp_rndfile);
			if(R == NULL)
			{
				//error
				if(n != NULL)
				{
					BN_free(n);
				}
				if(q != NULL)
				{
					BN_free(q);
				}
				if(I != NULL)
				{
					BN_free(I);
				}
				return NULL;
			}
			else
			{
				BIGNUM *R_1 = calculate_R(R, I);
				if(R_1 == NULL)
				{
					//error
					if(n != NULL)
					{
						BN_free(n);
					}
					if(q != NULL)
					{
						BN_free(q);
					}
					if(I != NULL)
					{
						BN_free(I);
					}
					if(R != NULL)
					{
						BN_free(R);
					}
					return NULL;
				}
				else
				{
					//perform n calculation where n = 2Rq + 1
					BIGNUM *bn_n = calculate_n(R_1, q);
					if(bn_n == NULL)
					{
						//error
						if(n != NULL)
						{
							BN_free(n);
						}
						if(q != NULL)
						{
							BN_free(q);
						}
						if(I != NULL)
						{
							BN_free(I);
						}
						if(R != NULL)
						{
							BN_free(R);
						}
						if(R_1 != NULL)
						{
							BN_free(R_1);
						}
						return NULL;
					}
					else
					{
						//Done step 7.1, print output
						fprintf(stdout, "  step 7, itr %d: R = %s, n = %s\n", itr, BN_bn2dec(R_1), BN_bn2dec(bn_n));

						//test if bn_n is prime under TrialDiv
						rewind(fp); //ensure that we are at the beginning of primesfile
						int trialdiv_returnCode_1 = trialdiv(bn_n, fp, 4);
						if(trialdiv_returnCode_1 == -1)
						{
							//error has happened in trialdiv function. Terminate this function
							if(n != NULL)
							{
								BN_free(n);
							}
							if(q != NULL)
							{
								BN_free(q);
							}
							if(I != NULL)
							{
								BN_free(I);
							}
							if(R != NULL)
							{
								BN_free(R);
							}
							if(R_1 != NULL)
							{
								BN_free(R_1);
							}
							if(bn_n != NULL)
							{
								BN_free(bn_n);
							}
							return NULL;

						}
						else if(trialdiv_returnCode_1 == 1)
						{
							int num_bits_in_n = BN_num_bits(bn_n);
							BIGNUM *a = RndOddNum_wrapper(num_bits_in_n, fp_rndfile);


							if(a == NULL)
							{
								//error
								if(n != NULL)
								{
									BN_free(n);
								}
								if(q != NULL)
								{
									BN_free(q);
								}
								if(I != NULL)
								{
									BN_free(I);
								}
								if(R != NULL)
								{
									BN_free(R);
								}
								if(R_1 != NULL)
								{
									BN_free(R_1);
								}
								if(bn_n != NULL)
								{
									BN_free(bn_n);
								}
								return NULL;
							}

							int cmp_result = cmp_a(a, bn_n);
							if(cmp_result == -1)
							{
								//error
								if(n != NULL)
								{
									BN_free(n);
								}
								if(q != NULL)
								{
									BN_free(q);
								}
								if(I != NULL)
								{
									BN_free(I);
								}
								if(R != NULL)
								{
									BN_free(R);
								}
								if(R_1 != NULL)
								{
									BN_free(R_1);
								}
								if(a != NULL)
								{
									BN_free(a);
								}
								if(bn_n != NULL)
								{
									BN_free(bn_n);
								}
								return NULL;
							}

							while(cmp_result)
							{
								a = RndOddNum_wrapper(num_bits_in_n, fp_rndfile);
								if(a == NULL)
								{
									//error
									if(n != NULL)
									{
										BN_free(n);
									}
									if(q != NULL)
									{
										BN_free(q);
									}
									if(I != NULL)
									{
										BN_free(I);
									}
									if(R != NULL)
									{
										BN_free(R);
									}
									if(R_1 != NULL)
									{
										BN_free(R_1);
									}
									if(bn_n != NULL)
									{
										BN_free(bn_n);
									}
									return NULL;
								}

								cmp_result = cmp_a(a, bn_n);
								if(cmp_result == -1)
								{
									//error
									if(n != NULL)
									{
										BN_free(n);
									}
									if(q != NULL)
									{
										BN_free(q);
									}
									if(I != NULL)
									{
										BN_free(I);
									}
									if(R != NULL)
									{
										BN_free(R);
									}
									if(R_1 != NULL)
									{
										BN_free(R_1);
									}
									if(a != NULL)
									{
										BN_free(a);
									}
									if(bn_n != NULL)
									{
										BN_free(bn_n);
									}
									return NULL;
								}
							}

							//Done step 7.2.1, print value a
							fprintf(stdout, "  step 7.2.1, itr %d: a = %s\n", itr, BN_bn2dec(a));

							BIGNUM *b = calculate_b(a, bn_n);
							if(b == NULL)
							{
								//error
								if(n != NULL)
								{
									BN_free(n);
								}
								if(q != NULL)
								{
									BN_free(q);
								}
								if(I != NULL)
								{
									BN_free(I);
								}
								if(R != NULL)
								{
									BN_free(R);
								}
								if(R_1 != NULL)
								{
									BN_free(R_1);
								}
								if(a != NULL)
								{
									BN_free(a);
								}
								if(bn_n != NULL)
								{
									BN_free(bn_n);
								}
								return NULL;
							}
							else
							{
								if(BN_is_one(b))
								{
									BIGNUM *b_1 = calculate_b_1(a, bn_n, R_1);
									if(b_1 == NULL)
									{
										//error
										if(n != NULL)
										{
											BN_free(n);
										}
										if(q != NULL)
										{
											BN_free(q);
										}
										if(I != NULL)
										{
											BN_free(I);
										}
										if(R != NULL)
										{
											BN_free(R);
										}
										if(R_1 != NULL)
										{
											BN_free(R_1);
										}
										if(a != NULL)
										{
											BN_free(a);
										}
										if(b != NULL)
										{
											BN_free(b);
										}
										if(bn_n != NULL)
										{
											BN_free(bn_n);
										}
										return NULL;
									}
									else
									{
										//compute b-1
										BIGNUM *sub_b_1 = sub_1(b_1);
										if(sub_b_1 == NULL)
										{
											//error
											if(n != NULL)
											{
												BN_free(n);
											}
											if(q != NULL)
											{
												BN_free(q);
											}
											if(I != NULL)
											{
												BN_free(I);
											}
											if(R != NULL)
											{
												BN_free(R);
											}
											if(R_1 != NULL)
											{
												BN_free(R_1);
											}
											if(a != NULL)
											{
												BN_free(a);
											}
											if(b != NULL)
											{
												BN_free(b);
											}
											if(b_1 != NULL)
											{
												BN_free(b_1);
											}
											if(bn_n != NULL)
											{
												BN_free(bn_n);
											}
											return NULL;
										}
										else
										{
											//find gcd of b-1 and n
											BIGNUM *d = gcd(sub_b_1, bn_n);
											if(d == NULL)
											{
												//error
												if(n != NULL)
												{
													BN_free(n);
												}
												if(q != NULL)
												{
													BN_free(q);
												}
												if(I != NULL)
												{
													BN_free(I);
												}
												if(R != NULL)
												{
													BN_free(R);
												}
												if(R_1 != NULL)
												{
													BN_free(R_1);
												}
												if(a != NULL)
												{
													BN_free(a);
												}
												if(b != NULL)
												{
													BN_free(b);
												}
												if(b_1 != NULL)
												{
													BN_free(b_1);
												}
												if(sub_b_1 != NULL)
												{
													BN_free(sub_b_1);
												}
												if(bn_n != NULL)
												{
													BN_free(bn_n);
												}
												return NULL;
											}

											if(BN_is_one(d))
											{
												if(level == 0)
												{
													fprintf(stdout, "\nMaurer's Algorithm found an %d-bit prime:\n", num_bits_in_n);
													fprintf(stdout, "  n = %s\n", BN_bn2dec(bn_n));
												}

												//Free allocated space
												if(n != NULL)
												{
													BN_free(n);
												}
												if(q != NULL)
												{
													BN_free(q);
												}
												if(I != NULL)
												{
													BN_free(I);
												}
												if(R != NULL)
												{
													BN_free(R);
												}
												if(R_1 != NULL)
												{
													BN_free(R_1);
												}
												if(a != NULL)
												{
													BN_free(a);
												}
												if(b != NULL)
												{
													BN_free(b);
												}
												if(b_1 != NULL)
												{
													BN_free(b_1);
												}
												if(sub_b_1 != NULL)
												{
													BN_free(sub_b_1);
												}
												if(d != NULL)
												{
													BN_free(d);
												}

												return bn_n; //Finally done, it is the calling function responsibility to free
											}
											else
											{
												//free allocated space
												if(n != NULL)
												{
													BN_free(n);
												}
												if(q != NULL)
												{
													BN_free(q);
												}
												if(I != NULL)
												{
													BN_free(I);
												}
												if(R != NULL)
												{
													BN_free(R);
												}
												if(R_1 != NULL)
												{
													BN_free(R_1);
												}
												if(a != NULL)
												{
													BN_free(a);
												}
												if(b != NULL)
												{
													BN_free(b);
												}
												if(b_1 != NULL)
												{
													BN_free(b_1);
												}
												if(sub_b_1 != NULL)
												{
													BN_free(sub_b_1);
												}
												if(d != NULL)
												{
													BN_free(d);
												}
												if(bn_n != NULL)
												{
													BN_free(bn_n);
												}
												return NULL;
											}
										}
									}
								}
							}
						}
					}
					itr += 1;
				}
			}
		}
	}
}

/*
 * Supplemental function to wrap around Maurer function
 * @param: k,  FILE pointer to primesfile, FILE pointer to rndfile
 * @output: print Maurer processes to stdout
 */
void maurer_wrapper(int k, FILE *fp, FILE *fp_rndfile)
{
	int level = 0;

	//Call maurer function with level initialized to zero
	BIGNUM *result = maurer(k, level, fp, fp_rndfile);
	if(result == NULL)
	{
		fprintf(stderr, "Error(s) has happened, exiting program...\n");
	}
	else
	{
		BN_free(result); //free from maurer() function call
	}
}
