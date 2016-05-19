/*
 * keyexpand.c
 *
 *  Created on: Mar 15, 2016
 *      Author: ntran
 */

#include "keyexpand.h"

/*
 * Supplemental function to check if a given key string is valid
 * @param: char pointer to key string
 * @output: isValid = 1 if key is valid and key string is stored in key[16], else isValid = 0
 */
struct mykey isValidKey(char *key)
{
	struct mykey keyReturn;
	keyReturn.isValid = 1; //Initially assume key is valid
	char *keytemp = key;

	char *temp = malloc(1); //temp holder to hold the read-in char
	int key_index = 0;
	int count = 0; //used to keep track of how many characters we read in
	uint8_t temp1 = 0x0; //temp holder to hold the first 4 bits of my byte
	uint8_t temp2 = 0x0; //temp holder to hold the last 4 bits of my byte
	uint8_t holder = 0x0; //temp holder to hold my byte

	while(*keytemp != '\0' && *keytemp != '\n')
	{
		if(*keytemp >= '0' && *keytemp <= '9')
		{
			*temp = (unsigned char)*keytemp - 48; //representation of char 0-9 to hex of 0-9
		}
		else if(*keytemp >= 'a' && *keytemp <= 'f')
		{
			*temp = (unsigned char)*keytemp - 87; //representation of char a-f to hex of 10-15
		}
		else
		{
			fprintf(stderr,"Detect invalid key '%c' in input key which should be represented in hexstring\n", *keytemp);
			keyReturn.isValid = 0;
			break;
		}
		count += 1;
		if (count > 32) //expect to have 32 chars or 16 bytes for AES-128 key
		{
			fprintf(stderr,"Input key has too many values\n");
			keyReturn.isValid = 0;
			break;
		}
		if (count % 2 !=  0)
		{
			temp1 = (uint8_t)*temp;
		}
		else
		{
			temp2 = (uint8_t)*temp;
			int temp1_index = 3;
			int temp2_index = 3;
			//now set the first 4 bits of holder to be temp1, and last 4 bits of holder to be temp2
			for(int k = 7; k >= 0; k--)
			{
				if(temp1_index >= 0)
				{
					if((temp1 & ((0x01) << temp1_index)) != 0) //bit is set, then set it in holder
					{
						holder |= ((0x01) << k);
					}
					temp1_index -= 1;
				}
				else
				{
					if((temp2 & ((0x01) << temp2_index)) != 0) //bit is set, then set it in holder
					{
						holder |= ((0x01) << k);
					}
					temp2_index -= 1;
				}
			}
			//Store this byte value into our key array inside struct mykey.
			keyReturn.key[key_index] = holder;
			key_index += 1;
			//reset holder for next round
			holder = 0x0;
		}
		keytemp += 1;
	}
	if (count < 32 && keyReturn.isValid == 1)
	{
		fprintf(stderr, "Input key has too few values\n");
		keyReturn.isValid = 0;
	}
	free(temp);

	return keyReturn;
}


/*
 * Supplemental function to rotate a word. Example: given 0x09cf4f3c => output = 0xcf4f3c09
 * @param: uint32_t word
 * @output: uint32_t word rotated
 */
uint32_t rotate_word(uint32_t inputWord)
{
	uint32_t returnWord = 0x0;
	uint8_t firstByte = 0x0;
	uint8_t secondByte = 0x0;
	uint8_t thirdByte = 0x0;
	uint8_t fourthByte = 0x0;

	//Extract the 32-bit word into 4 uint8_t bytes
	firstByte = (uint8_t)(inputWord >> 24);
	secondByte = (uint8_t)(inputWord >> 16);
	thirdByte = (uint8_t)(inputWord >> 8);
	fourthByte = (uint8_t)inputWord;
	int firstIndex = 7;
	int secondIndex = 7;
	int thirdIndex = 7;
	int fourthIndex = 7;

	for(int k = 31; k >= 0; k--)
	{
		if(secondIndex >= 0)
		{
			if((secondByte & ((0x01) << secondIndex)) != 0)
			{
				returnWord |= ((0x01) << k);
			}
			secondIndex -= 1;
		}
		else if(thirdIndex >= 0)
		{
			if((thirdByte & ((0x01) << thirdIndex)) != 0)
			{
				returnWord |= ((0x01) << k);
			}
			thirdIndex -= 1;
		}
		else if(fourthIndex >= 0)
		{
			if((fourthByte & ((0x01) << fourthIndex)) != 0)
			{
				returnWord |= ((0x01) << k);
			}
			fourthIndex -= 1;
		}
		else if(firstIndex >= 0)
		{
			if((firstByte & ((0x01) << firstIndex)) != 0)
			{
				returnWord |= ((0x01) << k);
			}
			firstIndex -= 1;
		}
	}
	return returnWord;
}

/*
 * Supplemental function to substitute word using S-box from tablefile
 * @param: uint32_t word and tablefile containing S-box
 * @output: uint32_t word substituted using S-box
 */
uint32_t sub_word(uint32_t inputWord, struct tablefile mytable)
{
	uint32_t subWord = 0x0;

	//Extract the 32-bit word into 4 uint8_t bytes
	uint8_t firstByte = (uint8_t)(inputWord >> 24);
	uint8_t secondByte = (uint8_t)(inputWord >> 16);
	uint8_t thirdByte = (uint8_t)(inputWord >> 8);
	uint8_t fourthByte = (uint8_t)inputWord;

	struct s_box_coor firstByte_coor = translateUint8toInt(firstByte);
	struct s_box_coor secondByte_coor = translateUint8toInt(secondByte);
	struct s_box_coor thirdByte_coor = translateUint8toInt(thirdByte);
	struct s_box_coor fourthByte_coor = translateUint8toInt(fourthByte);

	//Do a look up in the table to get the substitute value
	firstByte = mytable.S[firstByte_coor.row][firstByte_coor.col];
	secondByte = mytable.S[secondByte_coor.row][secondByte_coor.col];
	thirdByte = mytable.S[thirdByte_coor.row][thirdByte_coor.col];
	fourthByte = mytable.S[fourthByte_coor.row][fourthByte_coor.col];

	subWord = (uint32_t)firstByte << 24 | (uint32_t)secondByte << 16 | (uint32_t)thirdByte << 8 | (uint32_t)fourthByte;
	return subWord;
}

/*
 * Suuplemental function to get Rcon[i/Nk] given i/Nk
 * @param: value of i/Nk
 * @output: uint32_t Rcon using the formula {02}^i-1, {00},{00},{00}
 */
uint32_t get_Rcon(int i_Nk)
{
	uint32_t Rcon = 0x0;
	uint8_t firstByte = 0x0;
	//Handle special case where i_Nk = 1. {02}^0 = 01
	if(i_Nk == 1)
	{
		firstByte = 0x01;
	}
	else if(i_Nk == 2) //Handle special case where i_Nk = 2. {02}^1 = 02
	{
		firstByte = 0x02;
	}
	else
	{
		uint8_t temp = 0x02;
		firstByte = 0x02; //initialize firstByte value
		for(int k = 1; k < (i_Nk - 1); k++)
		{
			firstByte = xtimeb(firstByte, temp);
		}
	}
	//Should have firstByte, now add it to first byte of the 32-bit number to return
	int index = 7;
	for(int k = 31; k >=24; k --)
	{
		if((firstByte & ((0x01) << index)) != 0)
		{
			Rcon |= ((0x01) << k);
		}
		index -= 1;
	}
	return Rcon;
}

/*
 * Keyexpand function to generate 44 4-byte subkeys from given 16-byte key
 * @param:struct mykey containing the key, struct tablefile in order to supply s-box for sub_word operation, and a flag to indicate if we need to print output to stdout
 * @output: 44 subkeys printed out in stdout and return struct mysubkeys containing all 44 subkeys
 */
struct mysubkeys keyexpansion(struct mykey key, struct tablefile mytable, int toPrint)
{
	struct mysubkeys returnSubkeys;
	//The first 4 subkeys are from the 16-byte input key
	for(int k = 0; k < Nk; k++)
	{
		returnSubkeys.subkeys[k] = (uint32_t)key.key[4*k] << 24 | (uint32_t)key.key[(4*k)+1] << 16 | (uint32_t)key.key[(4*k)+2] << 8 | (uint32_t)key.key[(4*k)+3];
		if(toPrint == 1)
		{
			fprintf(stdout, "w[ %d]: %08x\n", k, returnSubkeys.subkeys[k]);
		}
	}
	//Compute other subkeys
	for(int k = Nk; k < (Nb*(Nr+1)); k++)
	{
		uint32_t temp = returnSubkeys.subkeys[k-1];
		if (k % Nk == 0)
		{
			temp = sub_word(rotate_word(temp), mytable) ^ get_Rcon(k/Nk);
		}
		else if (Nk > 6 && k % Nk == 4) //This is used for AES-256. We don't need this for the assignment which uses AES-128
		{
			temp = sub_word(temp, mytable);
		}
		returnSubkeys.subkeys[k] = returnSubkeys.subkeys[k-Nk] ^ temp;
		if(toPrint == 1)
		{
			if(k < 10)
			{
				fprintf(stdout, "w[ %d]: %08x\n", k, returnSubkeys.subkeys[k]);
			}
			else
			{
				fprintf(stdout, "w[%d]: %08x\n", k, returnSubkeys.subkeys[k]);
			}
		}
	}
	return returnSubkeys;
}
