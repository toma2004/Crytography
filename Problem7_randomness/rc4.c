/*
 * rc4.c
 *
 *  Created on: Apr 17, 2016
 *      Author: ntran
 */

#include "rc4.h"

/*
 * Supplemental function to check if a given pphrase is valid
 * @param: char pointer to a pass phrase
 * @return: 1 if isValid; else return 0
 */
int check_pphrase(char *pphrase)
{
	int isValid = 1;

	char *temp = pphrase;
	while(*temp != '\0' && *temp != '\n')
	{
		if(*temp == 34 || *temp == 39) //decimal representation of " and ' respectively
		{
			fprintf(stderr, "Detect pass phrase contains an invalid character %c\n", *temp);
			isValid = 0;
			break;
		}
		temp += 1;
	}
	return isValid;
}

/*
 * Supplemental function to check if a given length is valid
 * @param: char pointer to len
 * @return:
 * 0 if invalid
 * actual len in int format
 */
int check_len(char *len)
{
	int mylen = 1;
	char *temp = len;
	while(*temp != '\0' && *temp != '\n')
	{
		if(*temp < '0' || *temp > '9') //Detect a non-digit character
		{
			fprintf(stderr, "Detect a non-digit character %c while expecting an integer\n", *temp);
			mylen = 0;
			break;
		}
		temp += 1;
	}
	if(mylen)
	{
		mylen = atoi(len);
	}

	return mylen;
}

/*
 * Supplemental function to generate key given passphrase using MD5 algorithm
 * @param: char *pointer to passphrase
 * @return: 256-byte key (run MD15 16 times since each MD5 function call produces 16-byte key)
 */
unsigned char *generate_key(char *pphrase)
{
	char *s = malloc(strlen(pphrase) + 3);

	unsigned char *result = malloc(256); //expect to have 256 bytes returned
	int index = 0;

	for(int i = 0; i < 16; i++)
	{
		sprintf(s, "%02d%s", i, pphrase);
		MD5((unsigned char*)s, strlen(s), (result+index));
		index += 16; //expect to have 16 byte for every MD5() function call
	}
	free(s);
	return result; //It is the calling function responsibility to free
}

/*
 * Key scheduling algorithm
 * @param: generated key using MD5
 * @return: unsigned char* state containing the final state of key scheduling
 */
unsigned char* key_schedule(unsigned char* key)
{
	int size = 256;
	unsigned char* state = malloc(size); //expect to have state array with size 256 bytes
	//initialize state array
	for(int i = 0; i < size; i++)
	{
		state[i] = i;
	}

	//Key scheduling algorithm
	int j = 0;
	for(int i = 0; i < size; i++)
	{
		j = (j + state[i] + key[i % size]) % 256; //key size is 256 computed in generate_key() function above
		unsigned char temp = state[i];
		state[i] = state[j];
		state[j] = temp;
	}

	return state; //It is the calling function responsibility to free
}

/*
 * RC4 algorithm
 * @param: pass phrase, and an optional length
 * @output: print to stdout RC4 output with specified length if given
 */
void rc4(char* pphrase_input, int len)
{
	int size = 256; //expect state array size is 256
	int i = 0;
	int j = 0;

	unsigned char* key = generate_key(pphrase_input);
	unsigned char* state = key_schedule(key);
	if(len == 0) //no length is given; therefore print out the last state in key scheduling algorithm
	{
		//output last state in key scheduling algorithm to stdout
		for(int k = 0; k < 256; k++)
		{
			fwrite((state+k), 1, 1, stdout);
		}
	}

	//len is given, use output algorithm
	while(len > 0)
	{
		i = (i + 1) % size;
		j = (j + state[i]) % size;
		unsigned char temp = state[i];
		state[i] = state[j];
		state[j] = temp;
		fwrite((state + ((state[i] + state[j]) % size)), 1, 1, stdout);
		len -= 1;
	}

	//free allocated space
	free(key);
	free(state);
}
