/*
 * x4.c
 *
 *  Created on: Apr 23, 2016
 *      Author: ntran
 */

#include "x4.h"

/*
 * Supplemental function to find ei
 * @param: n and i
 * @return: double ei = (n-i+3)/2^i+2
 */
double calculate_e(int n, int i)
{
	double dividend_x4 = (double)n - i + 3;
	double divisor_x4 = pow((double)2, (double)i+2);
	double quo = dividend_x4 / divisor_x4;
	return quo;
}

/*
 * Supplemental function to find k so that k (n-k+3)/2^k+1 >= 5
 * @param:char * points to file name of file containing bit stream
 * @return: n and k
 */
struct mynk_x4 calculate_nk(char *bitStream)
{
	struct mynk_x4 result;
	result.isValid = 1;

	FILE *fp = NULL;
	fp = fopen(bitStream, "rb");
	if(fp == NULL)
	{
		//error
		fprintf(stderr, "Could not open %s file to read bit stream to calculate_nk in x4\n", bitStream);
		result.isValid = 0;
		return result;
	}

	unsigned char* temp = malloc(1); //read 1 byte at a time if possible
	int byte = 0;
	int numbits = 0;
	while((byte = fread(temp, 1, 1, fp)) > 0)
	{
		numbits += 8; //1 byte = 8 bits
	}

	//Try to find k
	int k = 2; //for this HW, smallest k value is 2

	double e_result = calculate_e(numbits, k);

	while((e_result - 5) > 0)
	{
		k += 1;
		e_result = calculate_e(numbits, k);
	}

	result.k = k-1; //largest k so that quo >= 5
	result.n = numbits;

	fclose(fp);
	free(temp);

	return result;
}

/*
 * Supplemental function to read in bit stream and stored it internally for processing
 * @param: FILE pointer points file name
 * @return: struct mydata containing data and total byte count if valid
 */
struct mydata store_data(char *bitStream)
{
	struct mydata result;
	result.isValid = 1;

	FILE *fp = NULL;
	fp = fopen(bitStream, "rb");
	if(fp == NULL)
	{
		//error
		fprintf(stderr, "Could not open %s file to read bit stream to calculate_nk in x4\n", bitStream);
		result.isValid = 0;
		return result;
	}

	/*
	 * Store all bit stream internall for easy processing
	 */
	int size = 1024; //intialize size to be 1024 bytes. Will allocate more space if needed
	unsigned char* data = malloc(size);
	//initialize result array
	for(int i = 0; i < size; i++)
	{
		data[i] = '\0';
	}
	int index = 0;
	int realloc_size = size;

	unsigned char* temp = malloc(1); //read 1 byte at a time if possible
	int byte = 0;
	int count = 0;
	int total_byte_read = 0;
	//go through each byte in bit stream
	while((byte = fread(temp, 1, 1, fp)) > 0)
	{
		total_byte_read += 1;
		//Reallocate more space if we run out of space
		//Check the size to see if we reach capacity
		if (count == size - 1)
		{
			realloc_size *= 2;
			unsigned char *temp_realloc = realloc(data, realloc_size);
			if (temp_realloc == NULL)
			{
				free(data);
				free(temp);
				fprintf(stderr, "ERROR: Can't reallocate more space to buffer in order to store more input in store_data function of x4. Please check memory usage and capacity\n");
				result.isValid = 0;
				return result;
			}
			data = temp_realloc;
			count = 0;
		}
		count += byte;
		//copy data into result
		data[index] = *temp;
		index += 1;
	}
	result.total_byte = total_byte_read;
	result.data = data; // It is the calling function responsibility to free

	fclose(fp);
	free(temp);

	return result;
}

/*
 * Supplemental funciton to calculate B and G for each i where 1 <= i <= k
 * @param: struct mydata stream and int i
 * @return: value of B and G
 */
struct myBG calculate_bg(struct mydata x4_data, int i)
{
	struct myBG result;

	/*
	 * initialize number of block/gap
	 */
	int block = 0;
	int gap = 0;

	//Go to first byte and first bit
	int bitTH = 7;
	int byteTH = 0;

	int isCheckingBlock = 0;

	while(byteTH < x4_data.total_byte) //keep checking until we finish with all bytes
	{
		if(((x4_data.data[byteTH]) & ((0x01) << bitTH)) != 0) //start block checking process
		{
			isCheckingBlock = 1;

			//start to check for next bit
			int temp_byteTH = byteTH;
			int temp_bitTH = bitTH;
			if(bitTH > 0)
			{
				temp_bitTH -= 1;
			}
			else
			{
				temp_bitTH = 7;
				temp_byteTH += 1;
			}

			int temp_i = i-1; //store value of i-1 for decrement. It is (i-1) because we count 1 bit found earlier.
			while(temp_i >= 0 && temp_byteTH < x4_data.total_byte) //end loop condition. We either end the loop when we check for length i bits, or when we reach end of bit stream
			{
				if(temp_i == 0)
				{
					//Block should end
					if(((x4_data.data[temp_byteTH]) & ((0x01) << temp_bitTH)) != 0) //but find another 1 after; therefore, this is not a block of length i
					{
						break;
					}
					else //this is a block of length i
					{
						block += 1;
					}
					temp_i -= 1;
				}
				else //continue to check for next bit to see if we have a block or not
				{
					if(((x4_data.data[temp_byteTH]) & ((0x01) << temp_bitTH)) != 0) //Yes, we have a block since next bit is 1
					{
						temp_i -= 1;
					}
					else //we don't have a block of length i since next bit is 0
					{
						//break out of loop early since we already know that we don't have a block
						break;
					}
				}
				//Go to next bit
				temp_bitTH -= 1;
				if(temp_bitTH < 0)
				{
					temp_bitTH = 7;
					temp_byteTH += 1;
				}
			}

			/*
			 * Take care of corner case: last element
			 * If we run out of bit, and temp_i = 0, this means that it is a block
			 */
			if (temp_i == 0 && temp_byteTH == x4_data.total_byte)
			{
				block += 1;
			}

		}
		else //start gap checking process
		{
			//start to check for next bit
			int temp_byteTH = byteTH;
			int temp_bitTH = bitTH;
			if(bitTH > 0)
			{
				temp_bitTH -= 1;
			}
			else
			{
				temp_bitTH = 7;
				temp_byteTH += 1;
			}

			int temp_i = i-1; //store value of i-1 for decrement. It is (i-1) because we count 1 bit found earlier.
			while(temp_i >= 0 && temp_byteTH < x4_data.total_byte) //end condition. We either end the loop when we check for length i bits, or when we reach end of bit stream
			{
				if(temp_i == 0)
				{
					//Gap should end
					if(((x4_data.data[temp_byteTH]) & ((0x01) << temp_bitTH)) == 0) //but find another 0 after; therefore, this is not a gap of length i
					{
						break;
					}
					else //this is a gap of length i
					{
						gap += 1;
					}
					temp_i -= 1;
				}
				else //continue to check for next bit to see if we have a gap or not
				{
					if(((x4_data.data[temp_byteTH]) & ((0x01) << temp_bitTH)) == 0) //Yes, we have a gap since next bit is 0
					{
						temp_i -= 1;
					}
					else //we don't have a gapl of length i since next bit is 1
					{
						//break out of loop early since we already know that we don't have a block
						break;
					}
				}
				//Go to next bit
				temp_bitTH -= 1;
				if(temp_bitTH < 0)
				{
					temp_bitTH = 7;
					temp_byteTH += 1;
				}
			}

			/*
			 * Take care of corner case: last element
			 * If we run out of bit, and temp_i = 0, this means that it is a gap
			 */
			if (temp_i == 0 && temp_byteTH == x4_data.total_byte)
			{
				gap += 1;
			}
		}

		/*
		 * we will need to go forward until we see a bit that is different from the bit read in
		 * this is because those same bits are in the same block/gap which had been checked
		 */
		if(isCheckingBlock) //we checked for a block previously, now need to find bit 0 to start
		{
			while(byteTH < x4_data.total_byte)
			{
				if(((x4_data.data[byteTH]) & ((0x01) << bitTH)) != 0)
				{
					bitTH -= 1;
					if(bitTH < 0)
					{
						bitTH = 7;
						byteTH += 1;
					}
				}
				else //found a zero bit
				{
					break;
				}
			}
		}
		else //we checked for gap previously, now need to find bit 1 to start
		{
			while(byteTH < x4_data.total_byte)
			{
				if(((x4_data.data[byteTH]) & ((0x01) << bitTH)) == 0)
				{
					bitTH -= 1;
					if(bitTH < 0)
					{
						bitTH = 7;
						byteTH += 1;
					}
				}
				else //found bit 1
				{
					break;
				}
			}
		}

		//reset variables for next round
		isCheckingBlock = 0;

	}

	result.B = block;
	result.G = gap;

	return result;
}

/*
 * x4 - runs test
 * @param: File pointer points to bit stream
 * @output: print out x4 test process and result
 */
void x4(FILE *fp)
{
	/*
	 * hard-coded value of m and its corresponding threshold
	 */
	double x4_threshold[12];
	x4_threshold[2] = 5.9915; //k = 2
	x4_threshold[3] = 9.4877; //k = 3
	x4_threshold[4] = 12.5916; //k = 4
	x4_threshold[5] = 15.5073; //k = 5
	x4_threshold[6] = 18.3070; //k = 6
	x4_threshold[7] = 21.0261; //k = 7
	x4_threshold[8] = 23.6848; //k = 8
	x4_threshold[9] = 26.2962; //k = 9
	x4_threshold[10] = 28.8693; //k = 10
	x4_threshold[11] = 31.4104; //k = 11

	char* filename_bitStream = store_bit_stream(fp);
	if(filename_bitStream == NULL)
	{
		//error has happened in store_bit_stream function
		return;
	}

	//print hex dump
	print_hexdump(filename_bitStream);

	struct mynk_x4 x4_info = calculate_nk(filename_bitStream);
	if(x4_info.isValid == 0)
	{
		//error has happened in calculate_nk
		//Need to remove file from /tmp/x3_temp
		unlink(filename_bitStream);
		free(filename_bitStream);
		return;
	}

	if(x4_info.n < 79)
	{
		fprintf(stderr, "n = %d\n", x4_info.n);
		fprintf(stderr, "x4: insufficient data\n");
		//Need to remove file from /tmp/x3_temp
		unlink(filename_bitStream);
		free(filename_bitStream);
		return;
	}

	if(x4_info.k >= 12)
	{
		fprintf(stderr, "n = %d\n", x4_info.n);
		fprintf(stderr, "k = %d\n", x4_info.k);
		fprintf(stderr, "x4: k is too large\n");
		//Need to remove file from /tmp/x3_temp
		unlink(filename_bitStream);
		free(filename_bitStream);
		return;
	}

	//print out value of n and k
	fprintf(stdout, "n = %d\n", x4_info.n);
	fprintf(stdout, "k = %d\n", x4_info.k);

	//Pass above 2 checks, now need to calculate e, B, and G for each i from 1 <= i <= k
	//prep step before calculate B and G
	struct mydata data = store_data(filename_bitStream);
	if(data.isValid == 0)
	{
		//error has happened in store_data() function
		//Need to remove file from /tmp/x3_temp
		unlink(filename_bitStream);
		free(filename_bitStream);
		return;
	}

	double x4_sum = 0;
	for(int i = 1; i <= x4_info.k; i++)
	{
		struct myBG bg_data = calculate_bg(data, i);
		double e = calculate_e(x4_info.n, i);
		fprintf(stdout, "e%d = %.06f\n", i, e);
		fprintf(stdout, "B%d = %d\n", i, bg_data.B);
		fprintf(stdout, "G%d = %d\n", i, bg_data.G);
		x4_sum += (( pow((double)bg_data.B - e, 2) / e ) + (pow((double)bg_data.G - e, 2) / e));
	}

	fprintf(stdout, "x4 = %.06f\n", x4_sum);
	if(x4_sum - x4_threshold[x4_info.k] < 0.0000001)
	{
		fprintf(stdout, "pass/fail = %s\n", "pass");
	}
	else
	{
		fprintf(stdout, "pass/fail = %s\n", "fail");
	}

	//Need to remove file from /tmp/temp_xfile
	unlink(filename_bitStream);

	//free allocated space
	free(data.data);
	free(filename_bitStream);
}
