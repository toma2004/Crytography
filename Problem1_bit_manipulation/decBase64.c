/*
 * decBase64.c
 *
 *  Created on: Jan 28, 2014
 *      Author: ntran
 */


#include "decBase64.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>


/*
 * The source code below was NOT written by me. I copy it from here: gemsres.com/story/mar06/192527/source.html
 */
uint8_t convert (unsigned char c)
{
	if(c >= 'A' && c <= 'Z')
	{
		return ((uint8_t)c - 65);
	}
	else if(c >= 'a' && c <= 'z')
	{
		return ((uint8_t)c - 71);
	}
	else if(c >= '0' && c <= '9')
	{
		return ((uint8_t)c + 4);
	}
	else if(c == '+')
	{
		return 62;
	}
	else if(c == '/')
	{
		return 63;
	}
	else
	{
		return 0;
	}
}
/*
 * End source code that is not mine
 */

void decBase64(void *location, int size, int remainder, int done)
{
	uint8_t *data = (uint8_t*)location;
	uint32_t n1 = 0;
	uint8_t c1, c2, c3, c4;	//used to store 4 8-bit number of each encoding char
	unsigned char buf[3]; //used to store the 3 decoding characters
	for(int i = 0; i < size; i+=4)
	{
		c1 = convert((unsigned char)data[i]);
		if((i+1) < size)
		{
			c2 = convert((unsigned char)data[i+1]);

		}
		if((i+2) < size)
		{
			c3 = convert((unsigned char)data[i+2]);

		}
		if((i+3) < size)
		{
			c4 = convert((unsigned char)data[i+3]);

		}

		/*
		 * process of creating the original 3 bytes
		 * n1 is a 32-bit number with the first 8 bit are zeros, each 8-bit after represents a byte of the original character
		 */
		n1 = c1 << 24;
		n1 = n1 | (c2 << 18);
		n1 = n1 | (c3 << 12);
		n1 = n1 | (c4 << 6);
		n1 = n1 >> 6;

		buf[0] = (unsigned char)(n1 >> 16);
		buf[1] = (unsigned char)(n1 >> 8);
		buf[2] = (unsigned char)n1;
	}
	if(done == 1)
	{
		unsigned char end[(sizeof (buf)) - remainder];
		for (int k = 0; k < (int)(sizeof(end)); k++)
		{
			end[k] = buf[k];
		}
		fwrite(end,1,sizeof(end),stdout);
	}
	else
	{
		fwrite(buf,1,sizeof(buf),stdout);
	}
}
