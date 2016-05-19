/*
 * x3.c
 *
 *  Created on: Apr 20, 2016
 *      Author: ntran
 */

#include "x3.h"

/*
 * Supplemental function to store bit stream internally for post processing
 * @param: FILE pointer points to bit stream
 * @return: unsigned char pointer points to beginning of bit stream stored internally
 */
char *store_bit_stream(FILE *fp)
{
	char *x3filename = malloc(30);
	strcpy(x3filename, "/tmp/temp_xfile");

/*	int fh = mkstemp(x3filename);
	if(fh < 1)
	{
		fprintf(stderr, "Could not create temp_x3 file in /tmp using mkstemp()\n");
		return NULL;
	}
*/
	FILE *fp_write = NULL;
	fp_write = fopen(x3filename,"wb");
	if(fp_write == NULL)
	{
		//error
		fprintf(stderr, "Could not open %s for write in x3 test\n", x3filename);
		free(x3filename);
		return NULL;
	}

	unsigned char* temp = malloc(1); //read 1 byte at a time if possible
	int byte = 0;
	while((byte = fread(temp, 1, 1, fp)) > 0)
	{
		fwrite(temp, 1, 1, fp_write); //It is the calling funciton responsibility to remove this file after done using
	}
	fclose(fp_write);
	free(temp);

	return x3filename; //It is the calling function responsibility to free
}

/*
 * Supplemental function to calculate value of m and k
 * @param: char pointer point to bit stream stored in a file
 * @output: struct mymk containing value of m and k
 */
struct mymk calculate_mk(char* bitStream)
{
	struct mymk result;
	result.isValid = 1;

	FILE *fp = NULL;
	fp = fopen(bitStream, "rb");
	if(fp == NULL)
	{
		//error
		fprintf(stderr, "Could not open %s file to read bit stream to calculate_mk in x3\n", bitStream);
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

	int m = 1;
	double n_div_m = floor((double)numbits/m);
	double five_times_2powM = 5 * pow((double)2, (double)m);

	while((n_div_m - five_times_2powM) >= 0)
	{
		m += 1;
		n_div_m = floor((double)numbits/m);
		five_times_2powM = 5 * pow((double)2, (double)m);
	}
	result.m = m-1; //return the previous m which is the largest value of m that satisfies the above equation
	result.k = floor((double)numbits/result.m);
	result.n = numbits;

	fclose(fp);
	free(temp);

	return result;
}

/*
 * Supplemental function to print hex dump
 * @param: char pointer points to bit stream stored in a file
 * @output: hex dump the first 256 bytes of bit stream if possible
 */
void print_hexdump(char* bitStream)
{
	FILE *fp = NULL;
	fp = fopen(bitStream, "rb");
	if(fp == NULL)
	{
		//error
		fprintf(stderr, "Could not open %s file to read bit stream in x3\n", bitStream);
		return;
	}

	unsigned char* temp = malloc(1); //read 1 byte at a time if possible
	int byte = 0;
	int count = 0;
	while((byte = fread(temp, 1, 1, fp)) > 0)
	{
		count += 1;
		//print out hex dump
		fprintf(stdout, "%02x", *temp);
		if(count % 16 == 0)
		{
			fprintf(stdout, "\n");
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

	fclose(fp);
	free(temp);
}

/*
 * Supplemental function to convert binary into decimal to print
 * @param: unsigned char* point to the binary, and size of bin (how many bytes)
 * @return: decimal representation of the given binary
 */
int bin_2_dec(unsigned char* bin, int size)
{
	int sum = 0;
	//Go through all bytes
	int exp_factor = 0; //2^exponential_factor at each set bit
	for(int byte = 0; byte < size; byte++)
	{
		//Go through all bits in bin to check which bit is set
		for(int bitTH = 0; bitTH < 8; bitTH++)
		{
			if(((bin[byte]) & ((0x01) << bitTH)) != 0)
			{
				//fprintf(stdout, "exp factor = %d\n", exp_factor);
				sum += (int)pow(2,exp_factor);
			}
			exp_factor += 1;
		}
	}
	return sum;
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
 * Supplemental function to calculate ni given m and bit stream
 * @param: value of m and bit stream
 * @return: int* points to n_array containing value of ni where 1 <= i <= 2^m
 */
int* calculate_ni(int m, char* bitStream)
{
	int size = (int)pow((double)2, (double)m);
	int* result = malloc(sizeof(int) * size); //allocate 2^m-byte space
	//initialize all vaules to zero
	for(int i = 0; i < size; i++)
	{
		result[i] = 0;
	}

	int size_store_m_bit = ceil_func((double)m/8, (int)m/8); //calculate how many byte needed to store m bit
	unsigned char* store_m_bit = malloc(size_store_m_bit); //allocate enough byte to store m bit

	int m_byteTH = (m-1)/8; //determine what byte to start with
	int m_bitTH = (m-1) - (8*m_byteTH); //determine what bit to start with

	//initialize store_m_bit
	for(int i = 0; i < size_store_m_bit; i++)
	{
		store_m_bit[i] = '\0';
	}

	FILE *fp = NULL;
	fp = fopen(bitStream, "rb");
	if(fp == NULL)
	{
		//error
		fprintf(stderr, "Could not open %s file to read bit stream to calculate ni array in x3\n", bitStream);
		free(result);
		free(store_m_bit);
		return NULL;
	}

	unsigned char* temp = malloc(1); //read 1 byte at a time if possible
	int byte = 0;
	int count_bits = 0;
	while((byte = fread(temp, 1, 1, fp)) > 0)
	{
		//read m bit at a time
		int bitTH = 7;
		while(bitTH >= 0)
		{
			if(((*temp) & ((0x01) << bitTH)) != 0)
			{
				//update store_m_bit
				store_m_bit[m_byteTH] |= ((0x01) << m_bitTH);
			}

			count_bits += 1;

			m_bitTH -= 1;
			if(m_bitTH < 0)
			{
				m_bitTH = 7;
				m_byteTH -= 1;
			}

			if(count_bits == m)
			{
				//have read enough m bits
				//convert bin to dec to update result array with occurance of ith type of sequence
				int result_index = bin_2_dec(store_m_bit, size_store_m_bit);

				if(result_index >= size)
				{
					//error
					fprintf(stderr, "Result index in calculate_ni() function of x3 is out of bound\n");
					free(result);
					free(store_m_bit);
					return NULL;
				}
				result[result_index] += 1;

				//re-initialize store_m_bit array for next round
				for(int i = 0; i < size_store_m_bit; i++)
				{
					store_m_bit[i] = '\0';
				}

				//re-initialize other variable as wel
				count_bits = 0;
				m_byteTH = (m-1)/8;
				m_bitTH = (m-1) - (8*m_byteTH);
			}
			bitTH -= 1;
		}
	}

	free(store_m_bit);
	fclose(fp);
	free(temp);

	return result; //It is the calling function responsibility to free
}

/*
 * Supplemental function to convert decimal into binary to print
 * @param: int dec and value of m
 * @return: string containing binary representation of given int
 */
char* dec_2_bin(int dec, int m)
{
	if(dec > 1023) //max m = 10 => max index in our array is 2^m - 1 = 1023
	{
		//error
		fprintf(stderr, "ERROR: out-of-bound index is given\n");
		return NULL;
	}
	char* result = malloc(11); //max m is 10 so we would need at most 10 bytes to print out each bit. Allocate 1 extra byte to store null char
	int index = 0;
	int rem = 0;
	while(dec > 0)
	{
		rem = dec % 2;
		dec /= 2;
		result[index] = rem + '0';
		index += 1;
	}
	//put null at the end of string
	result[index] = '\0';

	/*
	 * Need to reverse the string to store the correct order of binary representation
	 */
	//strrev(result);
	char* end = result;
	while(*end != '\0')
	{
		end += 1;
	}
	end -= 1;

	char* front = result;
	while(end > front)
	{
		char temp = *front;
		*front = *end;
		*end = temp;
		end -= 1;
		front += 1;
	}

	//Check if we have m bits. If not, prepend the string with zero
	if(index < m) //need to prepend zero
	{
		int num_zero_prepend = m - index;
		char *result_1 = malloc(25); //allocate extra just in case
		int index_1 = 0;
		while(num_zero_prepend > 0)
		{
			result_1[index_1] = 0 + '0';
			index_1 += 1;
			num_zero_prepend -= 1;
		}
		//add null to result_1 str
		result_1[index_1] = '\0';

		//strcat the result to result_1 and return result_1
		strcat(result_1, result);

		//Free result before returning
		free(result);
		return result_1; //It is the calling function responsibility to free
	}
	else if(index == m) //no need to prepend any zero
	{
		return result; //It is the calling function responsibility to free
	}
	else
	{
		//error
		fprintf(stderr, "ERROR: index is greater than m bits\n");
		free(result);
		return NULL;
	}
}

/*
 * Supplemental function to print out all nxxx = <integer> to stdout
 * @param: m and int* points to ni array
 * @output: print nxxx= <integer> to stdout
 */
void print_nxx(int m, int* ni)
{
	//re-calculate ni array size
	int size = (int)pow((double)2, (double)m);
	for(int i = 0; i < size; i++)
	{
		fprintf(stdout, "n");

		char* to_print = dec_2_bin(i, m);
		if(to_print == NULL)
		{
			//error has happened in dec_2_bin() function
			return;
		}
		fprintf(stdout, "%s", to_print);
		free(to_print); //free from dec_2_bin() function call
		fprintf(stdout, " = %d\n", ni[i]);
	}
}

/*
 * Supplemental function to calculate x3 based on formula
 * x3 = (2^m/k)* (sum of (ni^2 where i=1 to 2^m) - k
 * @param: value of m and k, int* point to ni array
 * @return: double x3
 */
double calculate_x3(int m, int k, int* ni)
{
	//re-calculate ni array size
	int size = (int)pow((double)2, (double)m);
	int sum_ni = 0;
	for(int i = 0; i < size; i++)
	{
		sum_ni += pow(ni[i], 2);
	}

	double x3_result = (pow(2,m) / k) * sum_ni - k;
	return x3_result;
}

/*
 * x3 - Poker test
 * @param: FILE pointer points to bit stream
 * @output: x3 test outcome
 */
void x3(FILE *fp)
{
	/*
	 * hard-coded value of m and its corresponding threshold
	 */
	double x3_threshold[10];
	x3_threshold[0] = 3.8415; //m = 1
	x3_threshold[1] = 7.8147; //m = 2
	x3_threshold[2] = 14.0671; //m = 3
	x3_threshold[3] = 24.9958; //m = 4
	x3_threshold[4] = 44.9853; //m = 5
	x3_threshold[5] = 82.5287; //m = 6
	x3_threshold[6] = 154.3015; //m = 7
	x3_threshold[7] = 293.2478; //m = 8
	x3_threshold[8] = 564.6961; //m = 9
	x3_threshold[9] = 1098.5208; //m = 10

	char* filename_bitStream = store_bit_stream(fp);
	if(filename_bitStream == NULL)
	{
		//error has happened in store_bit_stream function
		return;
	}

	//print hex dump
	print_hexdump(filename_bitStream);


	//check number of input bits and calculate value of m and k
	struct mymk x3_info = calculate_mk(filename_bitStream);
	if(x3_info.isValid == 0)
	{
		//error has happened in calculate_mk() function
		//Need to remove file from /tmp/x3_temp
		unlink(filename_bitStream);
		free(filename_bitStream);
		return;
	}

	if(x3_info.n < 10)
	{
		fprintf(stderr, "n = %d\n", x3_info.n);
		fprintf(stderr, "x3: insufficient data\n");
		//Need to remove file from /tmp/x3_temp
		unlink(filename_bitStream);
		free(filename_bitStream); //free from store_bit_stream() function call
		return;
	}

	if(x3_info.m >= 11)
	{
		fprintf(stderr, "n = %d\n", x3_info.n);
		fprintf(stderr, "m = %d\n", x3_info.m);
		fprintf(stderr, "x3: m is too large\n");
		//Need to remove file from /tmp/x3_temp
		unlink(filename_bitStream);
		free(filename_bitStream); //free from store_bit_stream() function call
		return;
	}

	//print n, m, k info
	fprintf(stdout, "n = %d\n", x3_info.n);
	fprintf(stdout, "m = %d\n", x3_info.m);
	fprintf(stdout, "k = %d\n", x3_info.k);

	//After passing those 2 checks, continue to calculate ni
	int *ni = calculate_ni(x3_info.m, filename_bitStream);
	if(ni == NULL)
	{
		//error has happened in calculate_ni function
		//Need to remove file from /tmp/x3_temp
		unlink(filename_bitStream);
		free(filename_bitStream);
		return;
	}

	//print all possible n
	print_nxx(x3_info.m, ni);

	//calculate x3
	double x3_result = calculate_x3(x3_info.m, x3_info.k, ni);
	fprintf(stdout, "x3 = %.6f\n", x3_result);

	//compare and output the final outcome
	if((x3_result - x3_threshold[x3_info.m-1]) < 0.0000001)
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
	free(filename_bitStream);
	free(ni);

}
