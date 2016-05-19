/*
 * inverse.c
 *
 *  Created on: Mar 20, 2016
 *      Author: ntran
 */

#include "inverse.h"


/*
 * Supplemental function to read poly, check if it is valid, and store it internally for further use
 * @param: char pointer to a poly
 * @return: struct inverse_poly with isValid=1 if poly is valid and inv_poly[] stores the poly; else, isValid=0
 */
struct inverse_poly readPoly(char *poly)
{
	struct inverse_poly result;
	result.isValid = 1;

	char *temp_poly = poly;
	int count_poly = 0;
	//Used to store poly temporarily
	unsigned char *store_poly = malloc(1);
	uint8_t poly_firstBits = 0x0;
	uint8_t poly_lastBits = 0x0;
	uint8_t poly_holder = 0x0;
	int poly_index = 0;

	while(*temp_poly != '\0' && *temp_poly != '\n')
	{
		//check input
		if(*temp_poly >= '0' && *temp_poly <= '9')
		{
			*store_poly = (unsigned char)*temp_poly - 48; //representation of char 0-9 to hex of 0-9
			count_poly += 1;
		}
		else if(*temp_poly >= 'a' && *temp_poly <= 'f')
		{
			*store_poly = (unsigned char)*temp_poly - 87; //representation of char a-f to hex of 10-15
			count_poly += 1;
		}
		else
		{
			fprintf(stderr,"Detect invalid char '%c' in poly which should be represented in hexstring\n", *temp_poly);
			result.isValid = 0;
			break;
		}

		if (count_poly > 8) //expect to have 8 chars or 4 bytes for a(x) coefficient
		{
			fprintf(stderr,"Poly has too many values\n");
			result.isValid = 0;
			break;
		}
		//Process poly
		if (count_poly % 2 !=  0)
		{
			poly_firstBits = (uint8_t)*store_poly;
		}
		else
		{
			poly_lastBits = (uint8_t)*store_poly;
			int temp1_index = 3;
			int temp2_index = 3;
			//now set the first 4 bits of holder to be poly1_firstBits, and last 4 bits of holder to be poly2_lastBits
			for(int k = 7; k >= 0; k--)
			{
				if(temp1_index >= 0)
				{
					if((poly_firstBits & ((0x01) << temp1_index)) != 0) //bit is set, then set it in holder
					{
						poly_holder |= ((0x01) << k);
					}
					temp1_index -= 1;
				}
				else
				{
					if((poly_lastBits & ((0x01) << temp2_index)) != 0) //bit is set, then set it in holder
					{
						poly_holder |= ((0x01) << k);
					}
					temp2_index -= 1;
				}
			}
			//Store this byte value into our poly array of struct inverse_poly.
			result.inv_poly[poly_index] = poly_holder;
			poly_index += 1;
			//reset holder for next round
			poly_holder = 0x0;
		}
		temp_poly += 1;

	}
	if (count_poly < 8 && result.isValid == 1)
	{
		fprintf(stderr, "Poly has too few values\n");
		result.isValid = 0;
	}

	if (count_poly == 8 && ((*temp_poly >= 'a' && *temp_poly <= 'z') || (*temp_poly >= '0' && *temp_poly <= '9')))
	{
		fprintf(stderr, "Poly has too many values\n");
		result.isValid = 0;
	}

	free(store_poly);
	return result;
}

/*
 * Supplemental function to print inverse process output to stdout
 * @param: int round, uint8_t rem[], uint8_t quo[], uint8_t aux[]
 * @output: prinnt "i=round, rem[i]={xx}{xx}{xx}{xx}, quo[i]={xx}{xx}{xx}{xx}, aux[i]={xx}{xx}{xx}{xx}"
 */
void printOutput_inverse(int round, uint8_t rem[], uint8_t quo[], uint8_t aux[])
{
	fprintf(stdout, "i=%d, ", round);
	fprintf(stdout, "rem[i]={%02x}{%02x}{%02x}{%02x}, ", rem[0], rem[1], rem[2], rem[3]);
	fprintf(stdout, "quo[i]={%02x}{%02x}{%02x}{%02x}, ", quo[0], quo[1], quo[2], quo[3]);
	fprintf(stdout, "aux[i]={%02x}{%02x}{%02x}{%02x}\n", aux[0], aux[1], aux[2], aux[3]);
}

/*
 * Supplemental function to construct a lookup_table for multiplicative inverse
 * @param: none
 * @return: struct lookup_table contain 16x16 2D array for all multiplicative inverse of 00-ff
 */
struct lookup_table construct_table()
{
	struct lookup_table mytable;

	//Contain all multiplicative inverse of 00-ff. Formate in row major. There are 256 values, but each 2 char represent 1 uint8_t => we have total of 2x256 = 512 chars
	unsigned char lookup_table[512] = "00018df6cb527bd1e84f29c0b0e1e5c774b4aa4b992b605f583ffdccff40eeb23a6e5af1554da8c9c10a98153044a2c22c45926cf3396642f235206f77bb59191dfe37672d31f569a764ab135425e909ed5c05ca4c2487bf183e22f051ec6117165eafd349a63643f44791df3393213b79b7978510b5ba3cb670d006a1fa8182837e7f809673be569b9e95d9f702b9a4de6a326dd88a84722a149f88f9dc899afb7c2ec38fb8654826c8124acee7d2620ce01fef11757871a58e763dbdbc86570b282fa3dad4e40fa92753041bfcace67a07ae63c5dbe2ea948bc4d59df8906bb10dd6ebc60ecfad084ed7e35d501eb35b2338346846038cdd9c7da0cd1a411c";
	int lookup_index = 0;
	uint8_t poly_firstBits = 0x0;
	uint8_t poly_lastBits = 0x0;
	uint8_t poly_holder = 0x0;
	int count = 0;

	for(int i = 0; i < 16; i++)
	{
		for(int k = 0; k < 16; k++)
		{
			//small loop to read 2 characters
			for(int j =0; j < 2; j++)
			{
				count += 1;
				//check input
				if(lookup_table[lookup_index] >= '0' && lookup_table[lookup_index] <= '9')
				{
					lookup_table[lookup_index] = (unsigned char)lookup_table[lookup_index] - 48; //representation of char 0-9 to hex of 0-9
				}
				else if(lookup_table[lookup_index] >= 'a' && lookup_table[lookup_index] <= 'f')
				{
					lookup_table[lookup_index] = (unsigned char)lookup_table[lookup_index] - 87; //representation of char a-f to hex of 10-15
				}

				if(count % 2 != 0)
				{
					poly_firstBits = (uint8_t)lookup_table[lookup_index];
					lookup_index += 1;
				}
				else
				{
					poly_lastBits = (uint8_t)lookup_table[lookup_index];
					lookup_index += 1;

					int temp1_index = 3;
					int temp2_index = 3;
					//now set the first 4 bits of holder to be poly1_firstBits, and last 4 bits of holder to be poly2_lastBits
					for(int k = 7; k >= 0; k--)
					{
						if(temp1_index >= 0)
						{
							if((poly_firstBits & ((0x01) << temp1_index)) != 0) //bit is set, then set it in holder
							{
								poly_holder |= ((0x01) << k);
							}
							temp1_index -= 1;
						}
						else
						{
							if((poly_lastBits & ((0x01) << temp2_index)) != 0) //bit is set, then set it in holder
							{
								poly_holder |= ((0x01) << k);
							}
							temp2_index -= 1;
						}
					}
					//store the value to mytable
					mytable.multiplicative_table[i][k] = poly_holder;

					//reset holder for next round
					poly_holder = 0x00;
				}
			}
		}
	}
	return mytable;
}

/*
 * Supplemental function to look up for multiplicative of a given uint8_t byte
 * @papram: uint8_t poly to be looked up for its multiplicative, and a lookup table
 * @output: uint8_t poly multiplicative of input
 */
uint8_t lookup_multiplicative_inverse(uint8_t poly, struct lookup_table mytable)
{
	uint8_t result = 0x00;

	//Get coor of lookup table from given poly
	struct s_box_coor mycoor = translateUint8toInt(poly);
	result = mytable.multiplicative_table[mycoor.row][mycoor.col];
	return result;
}

/*
 * Supplemental function to perform long-hand division
 * @param: dividend, divisor (dividend / divisor), lookup table, and a flag to handle the the first case which is a special case where we have poly x^4 (4th power degree)
 * @return: struct long_division containing quotient and reminder
 */
struct long_division divide(uint8_t dividend[], uint8_t divisor[], struct lookup_table mytable, int isFirstCase)
{

	struct long_division result;
	//initialize quo and rem
	for(int k = 0; k < 4; k++)
	{
		result.quo[k] = 0x00;
		result.rem[k] = 0x00;
	}

	//copy content of dividend into a temp array in order to use later in division process since dividend array gets changed for every new division round
	uint8_t holder_dividend[4];
	memcpy(holder_dividend, dividend, 4);

	//little lookup table for degree-index pair
	int index_degree_pair[4];
	index_degree_pair[3] = 0;
	index_degree_pair[2] = 1;
	index_degree_pair[1] = 2;
	index_degree_pair[0] = 3; //index 0 of uint8_t arr matches to degree x^3 in poly

	int isConst = 0; //check if the input divisor is a const. If so, handle the special case at the end where we need a {01} reminder

	//Find out the degree of divisor
	int index_degree_divisor = 0; //highest which is x^3
	for(int k = 0; k < 4; k++)
	{
		if(divisor[k] != 0x00)
		{
			index_degree_divisor = k;
			break;
		}
	}
	if(index_degree_divisor == 3)
	{
		isConst = 1;
	}
	int degree_divisor = index_degree_pair[index_degree_divisor];

	//Find out the degree of dividend
	int index_degree_dividend = 0; //highest which is x^3
	if(isFirstCase)
	{
		index_degree_dividend = 0; //set it to zero; otherwise, it will detect the degree to be const as the input is {00}{00}{00}{01} and we ignore the first {01}
	}
	else
	{
		for(int k = 0; k < 4; k++)
		{
			if(dividend[k] != 0x00)
			{
				index_degree_dividend = k;
				break;
			}
		}
	}
	int degree_dividend = index_degree_pair[index_degree_dividend];

	//Find quotient degree
	int quo_degree_index = 0; //highest degree x^3
	//find number of division round
	int division_round = 0;
	if(isFirstCase && !isConst) //handle the the first case which is a special case where we have poly x^, and the divisor is not a constant
	{
		quo_degree_index = index_degree_pair[4 - degree_divisor];
		division_round = (4 - degree_divisor) + 1;
	}
	else if(isFirstCase && isConst) //Take care of special case where divisor is a constant and it is the first case where we have (x^4 + 1) / constant - we should have only 1 round
	{
		division_round = 1;
		quo_degree_index = 3;
	}
	else //for all other cases
	{
		quo_degree_index = index_degree_pair[degree_dividend - degree_divisor];
		division_round = (degree_dividend - degree_divisor) + 1;
	}

	int index_element_dividend = 0; //used to keep track of how many element we have left in dividend. Ensure that we go through all elements before exit

	while(division_round > 0)
	{
		uint8_t temp[4];
		uint8_t temp_dividend[4];
		//initialize temp and temp_dividend array
		for(int k=0; k < 4; k++)
		{
			temp[k] = 0x00;
			temp_dividend[k] = 0x00;
		}

		if(!isFirstCase && isConst && (division_round - 1) == 0)//This will handle all cases, except the first case, for divisor is a const and it's  the last division round. Ensure we have {01} as reminder
		{
			uint8_t xor01 = holder_dividend[index_degree_dividend] ^ 0x01;
			result.quo[quo_degree_index] = xtimeb(lookup_multiplicative_inverse(divisor[index_degree_divisor], mytable), xor01);
			result.rem[3] = 0x01;
			division_round -= 1;
		}
		else if(isFirstCase && isConst) //This is to handle the first case and divisor is a constant. We also need to ensure that the remainder is {01}
		{
			//simply look for multiplicative inverse of divisor[index_degree_divisor] to cancel 0x01
			result.quo[quo_degree_index] = lookup_multiplicative_inverse(divisor[index_degree_divisor], mytable);
			result.rem[3] = 0x01;
			division_round -= 1;
		}
		else //for all other cases
		{
			if(dividend[index_degree_dividend] == 0x01 || (isFirstCase && (division_round == ((4 - degree_divisor) + 1)) ) ) //Either we have {01} or it is the first case x^4 + 1 and first round
			{
				//simply look for multiplicative inverse of divisor[index_degree_divisor] to cancel 0x01
				result.quo[quo_degree_index] = lookup_multiplicative_inverse(divisor[index_degree_divisor], mytable);
			}
			else
			{
				//Get the multiplicative inverse, then bigdot multiply (xtimeb operation) with dividend[index_degree_dividend]
				result.quo[quo_degree_index] = xtimeb(lookup_multiplicative_inverse(divisor[index_degree_divisor], mytable), dividend[index_degree_dividend]);
			}

			/*
			 * Now multiply the quo with each of divisor and XOR it with corresponding dividend to get to next round of division
			 */
			for(int k=index_degree_divisor; k < 4; k++)
			{
				temp[k] = xtimeb(result.quo[quo_degree_index], divisor[k]);
			}
			//XOR with corresponding dividend to get a new dividend for next division
			int temp_holder_index = index_degree_divisor; //index of the first element in divisor
			int temp_index = index_degree_dividend; //this index is used to keep track of element in the original dividend
			int temp_dividend_index = index_degree_dividend; //this index is used to keep track of element in the NEW dividend after XOR
			int temp_flag_fisrtCase = 0;
			while(temp_holder_index < 4)
			{
				if(isFirstCase && (division_round == ((4 - degree_divisor) + 1)) && temp_flag_fisrtCase==0 ) //it is the first case x^4 + 1 and first round -- special case to XOR with {01}
				{
					temp_dividend[temp_dividend_index] = 0x01 ^ temp[temp_holder_index];
					//set flag to ensure that it won't go to this statement for the next XOR
					temp_flag_fisrtCase = 1;
					temp_dividend_index += 1;
				}
				else
				{
					temp_dividend[temp_dividend_index] = dividend[temp_index] ^ temp[temp_holder_index];
					temp_index += 1;
					temp_dividend_index += 1;
				}
				temp_holder_index += 1;
			}

			/*
			 * If it is the first case and first round, we need to get rid of the first XOR result of {00} since we don't include the first {01} in dividend in our dividend array
			 * To get rid of it, we will do a left shift, so that we have free element(s) at the end of NEW dividend array
			 */
			if(isFirstCase && (division_round == ((4 - degree_divisor) + 1)))
			{
				index_element_dividend = index_element_dividend + degree_divisor; //update the number of elements we have left
				//reset
				temp_dividend_index -= 1; //this is to account for the first {00} being result of {01} of x^4 XOR {01}
				//Perform array shift to the left by 1 index
				uint8_t mytemp_holder = temp_dividend[0];
				temp_dividend[0] = temp_dividend[1];
				temp_dividend[1] = temp_dividend[2];
				temp_dividend[2] = temp_dividend[3];
				temp_dividend[3] = mytemp_holder;
			}
			else
			{
				index_element_dividend += 1; //update the number of elements we have left
			}

			if(temp_index < 4) //we still have leftover byte
			{
				//Get new element in original dividend for next round
				temp_dividend[temp_dividend_index] = holder_dividend[temp_index];
				//Copy new dividend into dividend array
				memcpy(dividend, temp_dividend, 4);

				//Prepare for next round
				//Find out the degree of dividend
				index_degree_dividend = 0; //highest which is x^3
				for(int k = 0; k < 4; k++)
				{
					if(dividend[k] != 0x00)
					{
						index_degree_dividend = k;
						break;
					}
				}
				//cover case where index_degree_dividend = 3 which represents a const and divisor is also a constant. If so, stop the division as we are already done
				//Cover cases where we have the degree of dividend is less than degree of divisor (opposite WRT index_degree, index 0 = highest degree)
				if(index_degree_dividend > index_degree_divisor)
				{
					//copy temp to reminder array before exit
					memcpy(result.rem, temp_dividend, 4);
					break; //done
				}
			}
			else //we are done as there is no more leftover byte in dividend. Copy reminder
			{
				//copy temp to reminder array before exit
				memcpy(result.rem, temp_dividend, 4);
			}
			quo_degree_index += 1; //increment quo index to store next quo byte
			division_round -= 1; //decrement division round as we just finished 1
		}
	}

	return result;
}

/*
 * Function to find the multiplicative inverse of a given poly
 * @param: a valid poly
 * @output: multiplicative inverse of poly if it has one and print out the processes to stdout
 */
void inverse(uint8_t inv_poly[])
{
	//Construct lookup_table to find multiplicative inverse of a byte
	struct lookup_table mytable = construct_table();

	uint8_t temp[5]; //initialize temp to be x^4 + 1 or {01}{00}{00}{00}{01}, but ignore the coefficient of x^4
	uint8_t quo[4];
	uint8_t aux[4];
	uint8_t temp_aux[4];
	uint8_t rem[4];
	//initialize reminder, quotient and aux
	for(int k=0; k < 4; k++)
	{
		quo[k] = 0x00;
		aux[k] = 0x00;
		rem[k] = 0x00;
		temp[k] = 0x00;
		temp_aux[k] = 0x00;
	}
	//set the last byte of temp to be {01} since we have x^4 + 1
	temp[3] = 0x01;
	//print first round
	printOutput_inverse(1, temp, quo, temp_aux);

	//Set rem to be poly
	memcpy(rem, inv_poly, 4);
	//set last byte of aux to be {01} since now x=1 for second round
	aux[3] = 0x01;
	//print second round
	printOutput_inverse(2, rem, quo, aux);

	/*
	 * Now go through the last 4 rounds to complete our multiplicative inverse process.
	 * We could end sooner if the poly has no inverse (rem = 00) or we arrive at rem = const sooner than 4 rounds
	 */
	int isInverse = 1;
	for(int k = 3; k < 7; k++)
	{
		struct long_division result;
		if(k == 3) //take special care for first division since the dividend has 4th degree (x^4 + 1)
		{
			result = divide(temp, rem, mytable, 1);
		}
		else
		{
			result = divide(temp, rem, mytable, 0);
		}
		/*
		 * Compute aux
		 */
		struct modprod_result quo_aux = modprod(result.quo, aux, 0);
		//aux holder to later update the previous aux
		uint8_t aux_holder[4];
		memcpy(aux_holder, aux, 4);
		//Perform XOR with previous aux
		for(int i = 0; i < 4; i++)
		{
			aux[i] = quo_aux.modprod_poly[i] ^ temp_aux[i];
		}
		//Update previous aux
		memcpy(temp_aux, aux_holder, 4);

		//print output
		printOutput_inverse(k, result.rem, result.quo, aux);

		/*
		 * Perform check on rem to see if we can stop early for the following cases:
		 * Case 1: rem = {00}{00}{00}{00} - stop the loop and report that the input poly does not have multiplicative inverse
		 * Case 2: rem = {00}{00}{00}{01} - rem is a constant with reminder = 01 signaling that we are done
		 */
		if(result.rem[0] == 0x00 && result.rem[1] == 0x00 && result.rem[2] == 0x00)
		{
			if(result.rem[3] == 0x00)
			{
				isInverse = 0;
				break;
			}
			else if(result.rem[3] == 0x01)
			{
				break;
			}
		}

		/*
		 * Update temp and rem for next round
		 * now temp = rem and rem = result.rem
		 */
		memcpy(temp, rem, 4);
		memcpy(rem, result.rem, 4);
	}

	//print last statement for inverse process
	if(!isInverse)
	{
		fprintf(stdout, "{%02x}{%02x}{%02x}{%02x} does not have a multiplicative inverse\n", inv_poly[0], inv_poly[1], inv_poly[2], inv_poly[3]);
	}
	else
	{
		fprintf(stdout, "Multiplicative inverse of {%02x}{%02x}{%02x}{%02x} is {%02x}{%02x}{%02x}{%02x}\n", inv_poly[0], inv_poly[1], inv_poly[2], inv_poly[3], aux[0], aux[1], aux[2], aux[3]);
	}

}
