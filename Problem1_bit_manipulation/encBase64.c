/*
 * encBase64.c
 *
 *  Created on: Jan 26, 2014
 *      Author: ntran
 */

#include "encBase64.h"
#include <inttypes.h>
#include <stdio.h>

void encBase64(void *location, int size, int remainder)
{
	const char base64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	uint8_t *data = (uint8_t*)location;
	uint32_t n = 0; //used to stored 24-bit number
	uint8_t num1, num2, num3, num4; //used to store 4 "6-bit" numbers

	char charac[4];
	/*
	 * The idea of below code is borrowed from: http://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C
	 * I added in new codes of my own
	 */
	for(int i = 0; i < size; i+=3)
	{
		n = data[i] << 16;
		if((i+1) < size)
		{
			n += data[i+1] << 8;
		}
		if((i+2) < size)
		{
			n += data[i+2];
		}

		/*
		 * num1, num2, num3, and num4 are not really a 6-bit number. Instead, they are 8-bit number with 2 leading zeros
		 * I do bitwise-AND with 63 (0011 1111) because this will ensure that 2 leading bits are always zeros and the last 6 most significant bits are the same
		 */
		num1 = (uint8_t) (n >> 18) & 63;
		num2 = (uint8_t) (n >> 12) & 63;
		num3 = (uint8_t) (n >> 6) & 63;
		num4 = (uint8_t)n  & 63;

		charac[0] = base64Table[num1];
		charac[1] = base64Table[num2];
		/*
		 * 1 byte is enough for 2 characters. Above it, we have to check if there is enough byte
		 */
		if((i+1) < size)
		{
			charac[2] = base64Table[num3];
		}
		if((i+2) < size)
		{
			charac[3] = base64Table[num4];
		}
	}
	/*
	 * End of idea borrowed
	 */

	/*
	 * take care of Endgame where we don't have enough 3 bytes
	 */
	if(remainder ==  1)
	{
		charac[2] = '=';
		charac[3] = '=';
	}
	else if(remainder == 2)
	{
		charac[3] = '=';
	}
	fwrite(charac,1,sizeof(charac),stdout);
}
