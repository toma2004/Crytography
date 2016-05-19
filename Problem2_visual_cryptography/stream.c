/*
 * stream.c
 *
 *  Created on: Jan 30, 2016
 *      Author: ntran
 */

#include "stream.h"
#include <openssl/md5.h>

#define MD5_DIGEST_LENGTH  16

/*
 * Generate simple stream cipher with passphrase and specified length
 */
void key_stream_generator(char *pphrase, int len)
{
	/*
	 * Below code is from Professor's HW2 webpage at: http://merlot.usc.edu/cs531-s16/homeworks/hw2/
	 * I did not write the code, but modified to suite my need
	 */
	unsigned char md5_buf[MD5_DIGEST_LENGTH];
	int hash_len = MD5_DIGEST_LENGTH + 2 + strlen(pphrase);
	char *s=malloc(hash_len+1);
	int i=0;

	MD5((unsigned char*)pphrase, strlen(pphrase), md5_buf); //initialize MD5
	int byte_generated = 0; //keep track of number of bytes generated
	for (;;) {
		sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, pphrase);
		memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
		MD5((unsigned char*)s, hash_len, md5_buf);
		//Stop generating when len is reached
		if ((byte_generated+8) > len)
		{
			int reminder = len % 8;
			fwrite(md5_buf, 1, reminder, stdout);
			break;
		}
		else
		{
			fwrite(md5_buf, 1, sizeof(md5_buf)>>1, stdout);
		}
		if (++i == 100) i = 0;
		byte_generated += 8;
	}
	free(s);
	/*
	 * end code I did not write
	 */
}

/*
 * return key with passphrase and specified length */
char *generate_key(char *pphrase, int len)
{
	/*
	 * Below code is from Professor's HW2 webpage at: http://merlot.usc.edu/cs531-s16/homeworks/hw2/
	 * I did not write the code, but modified to suite my need
	 */
	unsigned char md5_buf[MD5_DIGEST_LENGTH];
	int hash_len = MD5_DIGEST_LENGTH + 2 + strlen(pphrase);
	char *s=malloc(hash_len+1);
	int i=0;
	char *return_key = malloc(len);
	char *cur_ptr = return_key;

	MD5((unsigned char*)pphrase, strlen(pphrase), md5_buf); //initialize MD5
	int byte_generated = 0; //keep track of number of bytes generated
	for (;;) {
		sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, pphrase);
		memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
		MD5((unsigned char*)s, hash_len, md5_buf);
		//Stop generating when len is reached
		if ((byte_generated+8) > len)
		{
			int reminder = len % 8;
			memcpy(cur_ptr, md5_buf, reminder);
			break;
		}
		else
		{
			memcpy(cur_ptr, md5_buf, sizeof(md5_buf)>>1);
			cur_ptr += 8;
		}
		if (++i == 100) i = 0;
		byte_generated += 8;
	}
	free(s);

	/*
	 * end code I did not write
	 */

	return return_key; //will be freed in the function that calls it
}
