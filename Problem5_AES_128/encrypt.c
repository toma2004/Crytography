/*
 * encrypt.c
 *
 *  Created on: Mar 17, 2016
 *      Author: ntran
 */

#include "encrypt.h"

/*
 * Supplemental function to read in 16 byte input and store internally for AES-128 encryption
 * @param: input file pointer
 * @return: struct my input with isValid = 1 if input is valid and uint8_t arr will contain all 16 byte input, else isValid = 0
 */
struct myinput readInput(FILE *fp)
{
	struct myinput inputReturn;
	inputReturn.isValid = 1;

	int byte = 0;
	unsigned char *temp = malloc(1);
	int input_index = 0;
	int count = 0;

	while((byte = fread(temp, 1, 1, fp)) > 0)
	{
		count += byte;
		if (count > 16) //expect to have 16 bytes for AES-128 input. We will ignore the rest of input if there are more than 16 bytes
		{
			break;
		}

		//store input into my uint8_t input array
		inputReturn.input[input_index] = (uint8_t)*temp;
		input_index += 1;
	}
	if (count < 16)
	{
		fprintf(stderr, "Input file does not have enough 16 bytes. Exiting program...\n");
		inputReturn.isValid = 0;
	}
	free(temp);

	return inputReturn;
}

/*
 * Supplemental function to print encrypt output to stdout
 * @param: round index, round type, content, and flag to signal of this function is used to print output for encrypt or decrypt
 * @output: print "round[round_index}.round_type   round_content" to stdout
 */
void printOutput(int round_index, char* round_type, uint8_t round_content[], int isEncryption)
{
	if(round_index <10)
	{
		fprintf(stdout, "round[ %d].%s",round_index, round_type);
	}
	else
	{
		fprintf(stdout, "round[%d].%s",round_index, round_type);
	}

	if(strcmp(round_type, "output") == 0) //encrypt
	{
		fprintf(stdout, "   "); //3 spaces separating the columns
	}
	else if(strcmp(round_type, "ioutput") == 0) //decrypt
	{
		fprintf(stdout, "  "); //2 spaces separating the columns
	}
	else
	{
		if(isEncryption)
		{
			fprintf(stdout, "    "); //4 spaces separating the columns
		}
		else
		{
			fprintf(stdout, "   "); //3 spaces separating the columns
		}
	}
	for(int k=0; k <16 ; k++)
	{
		fprintf(stdout, "%02x", round_content[k]);
	}
	fprintf(stdout, "\n");
}

/*
 * Supplemental function to translate 4 uint32_t numbers into 16 uint8_t numbers
 * @param: struct mysubkeys containing all 44 subkeys, from & to index to indicate which 4 subkeys to use
 * @return: struct myholder containing 16 uint8_t numbers constructed from 4 uint32_t numbers
 */
struct myholder getRoundKey(struct mysubkeys subkeys, int from, int to)
{
	struct myholder roundkey;
	int roundKey_index = 0;

	for(int k = from; k <= to; k++)
	{
		//for each subkeys, we will translate it into 4 subround key. 32 bit = 4 * 8 (bits)
		roundkey.holder[roundKey_index] = (uint8_t)(subkeys.subkeys[k] >> 24);
		roundKey_index += 1;

		roundkey.holder[roundKey_index] = (uint8_t)(subkeys.subkeys[k] >> 16);
		roundKey_index += 1;

		roundkey.holder[roundKey_index] = (uint8_t)(subkeys.subkeys[k] >> 8);
		roundKey_index += 1;

		roundkey.holder[roundKey_index] = (uint8_t)subkeys.subkeys[k];
		roundKey_index += 1;
	}

	return roundkey;
}

/*
 * Supplental function to perform subByte
 * @param: uint8_t byte array containing 16 bytes to be substituted, struct mytable containing s-box table, and flag to signal of this function is used for encrypt or decrypt
 * @output: struct myholder containing substituted byte using s-box
 */
struct myholder subByte(uint8_t toBeSub[], struct tablefile mytable, int isEncryption)
{
	struct myholder result;
	//Substitute each byte
	for(int k = 0; k < 16; k++)
	{
		struct s_box_coor byte_coor = translateUint8toInt(toBeSub[k]);
		//Do a look up in the table to get the substitute value
		if(isEncryption)
		{
			result.holder[k] = mytable.S[byte_coor.row][byte_coor.col];
		}
		else
		{
			//use Inverse S-box to look up for decryption
			result.holder[k] = mytable.INVS[byte_coor.row][byte_coor.col];
		}
	}

	return result;
}

/*
 * Supplemental function to perform shiftrows
 * @param: uint8_t byte array to be under shift row operation, and flag to signal of this function is used for encrypt or decrypt
 * @output struct myholder containing result
 */
struct myholder shiftrows(uint8_t toBeShiftedRows[], int isEncryption)
{
	struct myholder result;
	int row = 1; //start with second row since we don't shift first row
	//declare index for first row which should be 0,4,8,12 in the array of uint8_t
	int i = 0;
	int k = 4;
	int j = 8;
	int m = 12;
	for (int round = row; round < 4; round++)
	{
		i += row;
		k += row;
		j += row;
		m += row;
		for(int shift = round; shift > 0; shift--)
		{
			if(isEncryption)
			{
				uint8_t temp = toBeShiftedRows[i];
				toBeShiftedRows[i] = toBeShiftedRows[k];
				toBeShiftedRows[k] = toBeShiftedRows[j];
				toBeShiftedRows[j] = toBeShiftedRows[m];
				toBeShiftedRows[m] = temp;
			}
			else
			{
				//implement inv shift row for decryption
				uint8_t temp = toBeShiftedRows[m];
				toBeShiftedRows[m] = toBeShiftedRows[j];
				toBeShiftedRows[j] = toBeShiftedRows[k];
				toBeShiftedRows[k] = toBeShiftedRows[i];
				toBeShiftedRows[i] = temp;
			}
		}
	}

	memcpy(result.holder, toBeShiftedRows, 16);
	return result;
}

/*
 * Supplemental function to perform mix coumns
 * @param: uint8_t byte array to be under mixed col operation, table file in order to get P - a(x) or INVP - a^-1(x), and flag to signal of this function is used for encrypt or decrypt
 * @output struct myholder containing result
 */
struct myholder mixcols(uint8_t toBeMixedCol[], struct tablefile mytable, int isEncryption)
{
	struct myholder result;
	int result_index = 0;
	//create array of struct modprod_result to hold result back from modprod operation
	struct modprod_result *circleX = malloc(4*sizeof(struct modprod_result));
	int modprod_index = 0;

	//Since I have 16 element in uint8_t toBeMixedCol[], process 4 at a time
	uint8_t poly2[4];
	for(int k = 0; k < 16; k=k+4)
	{
		poly2[0] = toBeMixedCol[k+3];
		poly2[1] = toBeMixedCol[k+2];
		poly2[2] = toBeMixedCol[k+1];
		poly2[3] = toBeMixedCol[k];
		if(isEncryption)
		{
			circleX[modprod_index] = modprod(mytable.P, poly2, 0);
		}
		else
		{
			//use INVP - a^-1(x) for multiplication in order to get back the original value for decryption
			circleX[modprod_index] = modprod(mytable.INVP, poly2, 0);
		}
		modprod_index += 1;
	}
	//Need to post process the modprod result back in column major of result and return
	int temp = 3;
	for(int k = 0; k < 4; k ++)
	{
		while(temp >= 0)
		{
			result.holder[result_index] = circleX[k].modprod_poly[temp];
			result_index += 1;
			temp -= 1;
		}
		//reset temp for next round
		temp = 3;
	}
	//free circleX array
	free(circleX);

	return result;

}

/*
 * encrypt input using AES-128
 * @param: pointer to input file, struct tablefile containing tables, and struct mykey containing key
 * @output: encrypted cipher and print to stdout each round processes
 */
void encrypt(struct tablefile mytable, struct mykey key, FILE *fp)
{
	//define constant in keyexpand (Nk, Nb, Nr)

	//construct subkeys using keyexpansion function
	struct mysubkeys subkeys = keyexpansion(key, mytable, 0);

	//Read input
	struct myinput input = readInput(fp);
	if(input.isValid)
	{
		uint8_t state[16];
		//copy input into state
		for(int k=0; k < 16; k++)
		{
			state[k] = input.input[k];
		}
		//print round 0 output
		printOutput(0, "input", state, 1);
		struct myholder roundkey = getRoundKey(subkeys, 0, Nb-1);
		printOutput(0, "k_sch", roundkey.holder, 1);
		//Perform AddRoundKey operation - state XOR roundKey
		for(int k=0; k < 16; k++)
		{
			state[k] = state[k] ^ roundkey.holder[k];
		}
		printOutput(1, "start", state, 1);

		/*
		 * Start for loop to go through all 10 rounds
		 */
		for(int round=1; round < Nr; round++)
		{
			//Perform subByte and copy return data to state
			struct myholder mySubByte = subByte(state, mytable, 1);
			memcpy(state, mySubByte.holder, 16);
			printOutput(round, "s_box", state, 1);
			//Perform shift row and copy return data to state
			struct myholder myShiftRows = shiftrows(state, 1);
			memcpy(state, myShiftRows.holder, 16);
			printOutput(round, "s_row", state, 1);
			//Perform mix cols and copy return data to state
			struct myholder myMixCols = mixcols(state, mytable, 1);
			memcpy(state, myMixCols.holder, 16);
			printOutput(round, "m_col", state, 1);
			//Perform add round key operation on state with new key rounds
			struct myholder roundKey2 = getRoundKey(subkeys, round*Nb, (round+1)*Nb-1);
			printOutput(round, "k_sch", roundKey2.holder, 1);
			//Perform AddRoundKey operation - state XOR roundKey
			for(int k=0; k < 16; k++)
			{
				state[k] = state[k] ^ roundKey2.holder[k];
			}
			printOutput(round+1, "start", state, 1);
		}

		/*
		 * Perform the last round to obtain output
		 */
		//Perform subByte and copy return data to state
		struct myholder mySubByte = subByte(state, mytable, 1);
		memcpy(state, mySubByte.holder, 16);
		printOutput(10, "s_box", state, 1);
		//Perform shift row and copy return data to state
		struct myholder myShiftRows = shiftrows(state, 1);
		memcpy(state, myShiftRows.holder, 16);
		printOutput(10, "s_row", state, 1);

		//Don't do mix cols for last operation

		//Perform add round key operation on state with new key rounds
		struct myholder roundKey2 = getRoundKey(subkeys, Nr*Nb, (Nr+1)*Nb-1);
		printOutput(10, "k_sch", roundKey2.holder, 1);
		//Perform AddRoundKey operation - state XOR roundKey
		for(int k=0; k < 16; k++)
		{
			state[k] = state[k] ^ roundKey2.holder[k];
		}
		printOutput(10, "output", state, 1);

	}
}
