/*
 * crypt.c
 *
 *  Created on: Feb 16, 2016
 *      Author: ntran
 */

#include "crypt.h"

/*
 * Function to check if a keyfile is in the right format
 * @param file pointer to a keyfile
 * @return: 0 - for invalid keyfile and 1 - for valid keyfile
 */
int checkKeyfile (FILE *fp)
{
	char *temp = malloc(1);
	int byte = 0;
	int count = 0;
	int period = 0;
	//read in char by char and check each char
	while ((byte = fread(temp,1,1,fp)) > 0)
	{
		count += byte;
		//every 27th character is a '\n', if not, return error
		if ((count % 27 == 0) && *temp != '\n')
		{
			free(temp);
			return 0;
		}

		if ((count % 27 != 0) && (*temp < 'a' || *temp > 'z')) //not in range a-z lower case => error
		{
			free(temp);
			return 0;
		}
	}
	free(temp);
	period = count / 27;
	return period;
}


/*
 * Function to encrypt or decrypt a file given a keyfile
 * @param: fp - keyfile, fp2 - file to be encrypted/decrypted
 * @output: encrypted/decrypted ASCII text
 */
void crypt(FILE *fp, char* keyfile, FILE *fp2)
{
	FILE *temp = fopen(keyfile, "r");
	int isValid = checkKeyfile(temp);
	fclose(temp);
	if (isValid == 0)
	{
		fprintf(stderr, "ERROR: detect incorrect keyfile\n");
		return;
	}
	int period = isValid;
	/*
	 * read and store key file
	 */
	int key_size = period*27;
	char *mykey = malloc (key_size);
	int byte = fread(mykey,1,key_size,fp);
	if (byte != key_size)
	{
		fprintf(stderr, "ERROR: reading in keyfile\n");
		free(mykey);
		return;
	}

	/*
	 * Start to read input char by char and encrypt/decrypt
	 */
	unsigned char *mychar = malloc(1);

	int index = 0;
	int perm_char_index = 0;
	int perm = 0;
	char *buffer = malloc(1);
	while ((byte = fread(mychar,1,1,fp2)) > 0)
	{
		//Check input char to see if it's >0x7e or <0x20
		if (*mychar != '\n' && (*mychar < 0x20 || *mychar > 0x7e))
		{
			fprintf(stderr, "ERROR: detect undefined character in input file\n");
			free(mykey);
			free(mychar);
			free(buffer);
			return;
		}
		if (*mychar >= 'a' && *mychar <= 'z')
		{
			//Start to encrypt/decrypt
			perm = (index % period) * 27;
			perm_char_index = (*mychar - 71) % 26; //get the character index in the original array "abcdefghijklmnopqrstuvwxyz"
			*buffer = *(mykey + perm + perm_char_index); //
			fwrite(buffer,1,1,stdout);
		}
		else //for any other char, we don't encrypt/decrypt
		{
			fwrite(mychar,1,1,stdout);
		}
		index += 1;
	}
	free(mychar);
	free(mykey);
	free(buffer);
}
