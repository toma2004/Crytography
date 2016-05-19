/*
 * x5.c
 *
 *  Created on: Apr 24, 2016
 *      Author: ntran
 */

#include "x5.h"

/*
 * Supplemental function to calculate A(d)
 * @param: n and dval and bit stream
 * @return: A(d)
 */
int calculate_A(int n, int dval, unsigned char* data)
{
	//Start to do XOR. We will check if 2 bits are the same or different and increment the count
	int count_diff = 0;
	int begin_bitTH = 7;
	int begin_byteTH = 0;

	int offset_bitTH = 7 - (dval % 8);
	int offset_byteTH = dval/8;

	for(int i = 0; i <= (n-dval-1); i++)
	{
		//check if 2 bits are different. If so, their XOR result is equal to 1
		if(( (data[begin_byteTH] & ((0x01) << begin_bitTH)) != 0 && (data[offset_byteTH] & ((0x01) << offset_bitTH)) == 0) || ((data[begin_byteTH] & ((0x01) << begin_bitTH)) == 0 && (data[offset_byteTH] & ((0x01) << offset_bitTH)) != 0))
		{
			count_diff += 1;
		}

		//go to next bit
		begin_bitTH -= 1;
		if(begin_bitTH < 0)
		{
			begin_bitTH = 7;
			begin_byteTH += 1;
		}

		offset_bitTH -= 1;
		if(offset_bitTH < 0)
		{
			offset_bitTH = 7;
			offset_byteTH += 1;
		}
	}

	return count_diff;
}

/*
 * Supplemental function to calculate x5
 * @param: n, dval, A(d)
 * @return: x5 value
 */
double calculate_x5(int n, int dval, int A)
{
	double dividend = 2 * (A - ((double)(n-dval) / 2));
	double divisor = sqrt((double)(n-dval));

	return dividend/divisor;
}

/*
 * x5 - autocorrelation test
 * @param: dval and FILE pointer points to bit stream
 * @output: x5 test process and result
 */
void x5(int dval, FILE *fp)
{
	char* filename_bitStream = store_bit_stream(fp);
	if(filename_bitStream == NULL)
	{
		//error has happened in store_bit_stream function
		return;
	}

	//print hex dump
	print_hexdump(filename_bitStream);

	FILE *fp_bitStream = fopen(filename_bitStream, "rb");
	if(fp_bitStream == NULL)
	{
		//error
		fprintf(stderr, "Could not open %s file to read bit stream in x5\n", filename_bitStream);
		free(filename_bitStream);
		return;
	}

	int byte = 0;
	unsigned char* temp = malloc(1);
	int numbits = 0;
	while((byte = fread(temp, 1, 1, fp_bitStream)) > 0)
	{
		numbits += 8; //1 byte = 8 bits
	}
	free(temp);
	fclose(fp_bitStream);

	//check if we have enough bits
	if(numbits < dval+10)
	{
		fprintf(stderr, "n = %d\n", numbits);
		fprintf(stderr, "x5: insufficient data\n");
		//Need to remove file from /tmp/temp_xfile
		unlink(filename_bitStream);
		free(filename_bitStream);
		return;
	}

	//check if d is too small or too large.  1 <= d <= n/2
	int half = numbits >> 1;
	if(dval < 1 || dval > half)
	{
		fprintf(stderr, "n = %d\n", numbits);
		fprintf(stderr, "x5: d should be in the range [1,%d]\n", half);
		//Need to remove file from /tmp/temp_xfile
		unlink(filename_bitStream);
		free(filename_bitStream);
		return;
	}

	fprintf(stdout, "n = %d\n", numbits);
	fprintf(stdout, "d = %d\n", dval);

	//Store bit stream internally
	struct mydata data = store_data(filename_bitStream);
	if(data.isValid == 0)
	{
		//error has happened in store_data() function
		//Need to remove file from /tmp/temp_xfile
		unlink(filename_bitStream);
		free(filename_bitStream);
		return;
	}

	//Calculate A(d)
	int A = calculate_A(numbits, dval, data.data);

	//Calculate x5
	double x5 = calculate_x5(numbits, dval, A);

	fprintf(stdout, "A(d) = %d\n", A);
	fprintf(stdout, "x5 = %.06f\n", x5);

	//Need find absoluate of x5 since it is a two-sided test
	if(fabs(x5) - 1.9600 < 0.0000001) //x5 threshold = 1.9600
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
	free(data.data);
}
