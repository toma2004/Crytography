/*
 * test.c
 *
 *  Created on: Jan 15, 2014
 *      Author: ntran
 */

#include <stdio.h>
#include "test.h"


void hexdump (int addr, void *location, int size)
{
	unsigned char *data = (unsigned char*)location;
	unsigned char buffer[16];
	int lastIndex = 0;
	for(int i = 0; i < size; i++)
	{
		/*
		 * The idea of below code is borrowed from: http://stackoverflow.com/questions/7775991/how-to-get-hexdump-of-a-structure-data
		 * I added in new codes of my own
		 */
		if((i%16) == 0){ //only print the memory address at the beginning and after reading 16 bytes
			fprintf(stdout,"%06x: ",addr);
		}
		if((i%8) == 0 && (i%16) != 0){ //print a space after reading 8 bytes
			if(i != 0)
			{
				fwrite(" ",sizeof(char),1,stdout);
			}
		}

		fprintf(stdout,"%02x ",data[i]);

		if(data[i]<= 0x1f || data[i] == 0x7f)
		{
			buffer[i%16] = '.';
		}
		else if(data[i] >= 0x80)
		{
			buffer[i%16] = '~';
		}
		else
		{
			buffer[i%16] = data[i];
		}
		/*
		 * End of idea borrowed
		 */

		lastIndex++;
	}
	//Use lastIndex to keep track and fill in "--" for non-existing bytes
	while((lastIndex%16) !=0)
	{
		if((lastIndex%8) == 0)
		{
			fwrite(" ",sizeof(char),1,stdout);
		}
		fwrite("-",sizeof(char),1,stdout);
		fwrite("-",sizeof(char),1,stdout);
		fwrite(" ",sizeof(char),1,stdout);
		lastIndex++;
	}

	fwrite(" ",sizeof(char),1,stdout);
	if(size < 16)
	{
		for (int i = size;i < 16;i++)
		{
			buffer[i] = ' '; //add space character for non-existing bytes

		}
	}
	fwrite(buffer,1,sizeof(buffer),stdout);
	fwrite("\n",sizeof(char),1,stdout);

}
