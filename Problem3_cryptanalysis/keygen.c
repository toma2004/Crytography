/*
 * keygen.c
 *
 *  Created on: Feb 14, 2016
 *      Author: ntran
 */

#include "keygen.h"
#include <openssl/md5.h>

#define MD5_DIGEST_LENGTH  16

/*
 * User simple stream cipher from HW2 to generate randon number X
 */
unsigned char *random_integer(char *pphrase, char *s, int hash_len, int i, unsigned char md5_buf[])
{
	unsigned char *return_key = malloc(8);
	unsigned char *cur_ptr = return_key;

	/*
	 * Below code is from Professor's HW2 webpage at: http://merlot.usc.edu/cs531-s16/homeworks/hw2/
	 * I did not write the code, but modified to suite my need
	 */
	sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, pphrase);
	memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
	MD5((unsigned char*)s, hash_len, md5_buf);
	/*
	 * end code I did not write
	 */

	memcpy(cur_ptr, md5_buf, 8); //take only the first 8 byte of md5_buf whose size is 16
	return return_key; //will be freed in the function that calls it
}

/*
 * Generate key and print it out to stdout
 */
void generate_key(char *pphrase, int period)
{
	char state[] = "abcdefghijklmnopqrstuvwxyz";
	int L = 26;
	/*
	 * Perform all the initialization before calling random integer function
	 */
	/*
	 * Below code is from Professor's HW2 webpage at: http://merlot.usc.edu/cs531-s16/homeworks/hw2/
	 * I did not write the code, but modified to suite my need
	 */
	unsigned char md5_buf[MD5_DIGEST_LENGTH];
	int hash_len = MD5_DIGEST_LENGTH + 2 + strlen(pphrase);
	char *s = malloc(hash_len+1);
	int i = 0;
	MD5((unsigned char*)pphrase, strlen(pphrase), md5_buf); //initialize MD5
	/*
	 * end code I did not write
	 */

	unsigned char *key = NULL;
	unsigned x1 = 0;
	unsigned x2 = 0;
	int r = 0;
	for(int j = 0; j < period; j++)
	{
		for(int k = 0; k < 2; k++)
		{
			while (L > 1)
			{
				if (x1 == 0 && x2 == 0)
				{
					if (key != NULL)
					{
						free(key);
					}
					unsigned char *key = random_integer(pphrase, s, hash_len, i, md5_buf);
					x1 = (unsigned char) *key << 24 | (unsigned char) *(key+1) << 16 | (unsigned char) *(key+2) << 8 | (unsigned char) *(key+3); //Get my first unsigned int
					x2 = (unsigned char) *(key+4) << 24 | (unsigned char) *(key+5) << 16 | (unsigned char) *(key+6) << 8 | (unsigned char) *(key+7); //Get my second unsigned int
					i++;
					if (i == 100)
					{
						i = 0;
					}
				}

				if (x1 != 0)
				{
					r = x1 % L;
					x1 = 0;
				}
				else
				{
					r = x2 % L;
					x2 = 0;
				}

				if (r != L-1)
				{
					char temp = state[r];
					state[r] = state[L-1];
					state[L-1] = temp;
				}
				L -= 1;
			}
			//reset L for next iteration
			L = 26;
		}
		fwrite(state,1,sizeof(state)-1,stdout); //exclude writing the null chacracter at the end of each string
		fwrite("\n", 1, 1, stdout);
		strcpy(state,"abcdefghijklmnopqrstuvwxyz");
	}

	if (key != NULL)
	{
		free(key);
	}
	free(s);
}
