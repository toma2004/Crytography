/*
 * decrypt3.c
 *
 *  Created on: Mar 5, 2016
 *      Author: ntran
 */

#include "decrypt3.h"

/*
 * decrypt3 function to decrypt the ciphertext using EDE 3DES
 * @param: tablefile, key, and file input pointer
 * @output: original plaintext
 */
void decrypt3(struct tablefile mytable, struct key3 mykey3, FILE *fp)
{
	int byte = 0;
	char *myinput = malloc(8);
	unsigned char *decrypt_cipher;
	unsigned char *encrypt_decrypt_cipher;
	unsigned char *plaintext;
	while((byte = fread(myinput,1,8,fp)) >= 8) //Read 8 bytes at a time from plaintext input
	{
		for(int i = 2; i >= 0; i--)//Go through all 3 part of my 24-byte key in reverse order
		{
			struct key mykey;
			mykey.isValid = 1; //When we get to this function, we already know that key3 is a valid key
			for(int k = 0; k < 8; k++)
			{
				mykey.key[k] = mykey3.key3[i][k];
			}
			struct subkeys mysubkeys = keySchedule(mytable, mykey);
			if (i == 2)
			{
				decrypt_cipher = sup_decrypt(mytable, mysubkeys, myinput, i);
			}
			else if(i == 1)
			{
				encrypt_decrypt_cipher = sup_encrypt(mytable, mysubkeys, (char*)decrypt_cipher, i);
			}
			else
			{
				plaintext = sup_decrypt(mytable, mysubkeys, (char*)encrypt_decrypt_cipher, i);
			}
		}
		fwrite(plaintext,1,8,stdout);
		free(decrypt_cipher);
		free(encrypt_decrypt_cipher);
		free(plaintext);
	}
	if (byte > 0) //take care of last case where there is not enough 8 bytes
	{
		//padding input with zeros
		for(int k = byte; k < 8; k++)
		{
			myinput[k] = '\0';
		}

		for(int i = 2; i >= 0; i--)//Go through all 3 part of my 24-byte key in reverse order
		{
			struct key mykey;
			mykey.isValid = 1; //When we get to this function, we already know that key3 is a valid key
			for(int k = 0; k < 8; k++)
			{
				mykey.key[k] = mykey3.key3[i][k];
			}
			struct subkeys mysubkeys = keySchedule(mytable, mykey);
			if (i == 2)
			{
				decrypt_cipher = sup_decrypt(mytable, mysubkeys, myinput, i);
			}
			else if(i == 1)
			{
				encrypt_decrypt_cipher = sup_encrypt(mytable, mysubkeys, (char*)decrypt_cipher, i);
			}
			else
			{
				plaintext = sup_decrypt(mytable, mysubkeys, (char*)encrypt_decrypt_cipher, i);
			}
		}
		fwrite(plaintext,1,8,stdout);
		free(decrypt_cipher);
		free(encrypt_decrypt_cipher);
		free(plaintext);
	}
	free(myinput);
}
