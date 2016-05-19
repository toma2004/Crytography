/*
 * encrypt3.c
 *
 *  Created on: Mar 4, 2016
 *      Author: ntran
 */

#include "encrypt3.h"

/*
 * Supplemental function to check if key is valid
 * @param: pointer to key file
 * @reutrn: struct key containing key and 0 if invalid, and 1 if valid
 */
struct key3 isValid_Key3(char *input_key3)
{
	struct key3 mykey3;
	//int index_mykey3 = 0;
	mykey3.isValid = 1;
	for(int i = 0; i < 3; i ++)
	{
		for (int k = 0; k < 8; k++)
		{
			mykey3.key3[i][k] = '\0'; //initialize key3 array
		}
	}
	/*
	 * Read 8 bytes from input_key3 at a time, and call function isValidKey in encrypt.c to check
	 * return valid = 1 only if we have enough 24 bytes and they are all valid
	 */
	int count = 0;
	int key_part = 0;
	char *myinput = malloc(17); //null-terminated string
	int index = 0; //index of myinput char pointer
	int index_key3 = 0; //index of key3 char pointer
	while(input_key3[index_key3] != '\0' && mykey3.isValid == 1)
	{
		//Check to see if we somehow read in more than 24 bytes
		if(index_key3 >= 48)
		{
			mykey3.isValid = 0;
			fprintf(stderr, "key3 has too many values\n");
			break;
		}
		count += 1;
		myinput[index] = input_key3[index_key3];
		index += 1;
		index_key3 += 1;
		//When we have 8 byte,call isValidKey function to check
		if (count == 16)
		{
			myinput[index] = '\0';
			struct key mykeytoken = isValidKey(myinput);
			if(mykeytoken.isValid)
			{
				//copy partial key into my key struct
				for(int k = 0; k < 8; k++)
				{
					mykey3.key3[key_part][k] = mykeytoken.key[k];
				}
				key_part += 1;
			}
			else
			{
				mykey3.isValid = 0;
				break;
			}
			count = 0; //reset count
			index = 0; //reset index
		}
	}

	if (index_key3 < 48)
	{
		mykey3.isValid = 0;
		fprintf(stderr, "key3 has too few values\n");
	}
	free(myinput);
	return mykey3;
}

/*
 * encrypt3 function to encrypt the plaintext using EDE 3DES
 * @param: tablefile, key, and file input pointer
 * @output: ciphertext
 */
void encrypt3(struct tablefile mytable, struct key3 mykey3, FILE *fp)
{
	int byte = 0;
	char *myinput = malloc(8);
	unsigned char *cipher;
	unsigned char *decrypt_cipher;
	unsigned char *encrypt_decrypt_cipher;
	while((byte = fread(myinput,1,8,fp)) >= 8) //Read 8 bytes at a time from plaintext input
	{
		for(int i = 0; i < 3; i++)//Go through all 3 part of my 24-byte key
		{
			struct key mykey;
			mykey.isValid = 1; //When we get to this function, we already know that key3 is a valid key
			for(int k = 0; k < 8; k++)
			{
				mykey.key[k] = mykey3.key3[i][k];
			}
			struct subkeys mysubkeys = keySchedule(mytable, mykey);
			if (i == 0)
			{
				cipher = sup_encrypt(mytable, mysubkeys, myinput, i);
			}
			else if(i == 1)
			{
				decrypt_cipher = sup_decrypt(mytable, mysubkeys, (char*)cipher, i);
			}
			else
			{
				encrypt_decrypt_cipher = sup_encrypt(mytable, mysubkeys, (char*) decrypt_cipher, i);
			}
		}
		fwrite(encrypt_decrypt_cipher,1,8,stdout);
		free(cipher);
		free(decrypt_cipher);
		free(encrypt_decrypt_cipher);
	}
	if (byte > 0) //take care of last case where there is not enough 8 bytes
	{
		//padding input with zeros
		for(int k = byte; k < 8; k++)
		{
			myinput[k] = '\0';
		}

		for(int i = 0; i < 3; i++)//Go through all 3 part of my 24-byte key
		{
			struct key mykey;
			mykey.isValid = 1; //When we get to this function, we already know that key3 is a valid key
			for(int k = 0; k < 8; k++)
			{
				mykey.key[k] = mykey3.key3[i][k];
			}
			struct subkeys mysubkeys = keySchedule(mytable, mykey);
			if (i == 0)
			{
				cipher = sup_encrypt(mytable, mysubkeys, myinput, i);
			}
			else if(i == 1)
			{
				decrypt_cipher = sup_decrypt(mytable, mysubkeys, (char *)cipher, i);
			}
			else
			{
				encrypt_decrypt_cipher = sup_encrypt(mytable, mysubkeys, (char *)decrypt_cipher, i);
			}
		}
		fwrite(encrypt_decrypt_cipher,1,8,stdout);
		free(cipher);
		free(decrypt_cipher);
		free(encrypt_decrypt_cipher);
	}
	free(myinput);
}
