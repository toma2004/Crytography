/*
 * func.c
 *
 *  Created on: Jan 17, 2014
 *      Author: ntran
 */

#include <stdio.h>
#include "func.h"
#include "test.h"
#include "encBase64.h"
#include "decBase64.h"

void processInputHexdump (FILE *fp)
{
	char buffer[16];
	int byte = 0;
	int index_addr = 0;
	while((byte=fread(buffer,1,sizeof(buffer),fp)) > 0)
	{
		if(byte < 16)	//check if we are reading the last elements before done
		{
			char temp[byte];
			for (int i=0; i<byte;i++)
			{
				temp[i] = buffer[i];
			}
			hexdump (index_addr, temp, sizeof(temp));
		}
		else
		{
			hexdump (index_addr, buffer, sizeof(buffer));
		}
		index_addr=index_addr+16;
	}
}

void processInputEncBase64 (FILE *fp)
{
	char buffer[3];
	int count = 0;
	int byte = 0;

	while((byte=fread(buffer,1,sizeof(buffer),fp)) > 0)
	{
		if(byte < 3)	//check if we are reading the last elements before done
		{
			if(byte == 2)
			{
				buffer[2] = 0;
			}
			else if (byte ==1)
			{
				buffer[1] = 0;
				buffer[2] = 0;
			}
			if(count == 16) //ensure that each line in the output has exactly 64 characters except the last line
			{
				fwrite("\n",sizeof(char),1,stdout);
				count = 0;

			}
			encBase64(buffer,sizeof(buffer), byte);
		}
		else
		{
			if(count == 16) //ensure that each line in the output has exactly 64 characters except the last line
			{
				fwrite("\n",sizeof(char),1,stdout);
				count = 0;

			}
			encBase64(buffer,sizeof(buffer), byte);
			count++;
		}
	}
	fwrite("\n",sizeof(char),1,stdout);
}

void processInputDecBase64 (FILE *fp)
{
	char buffer[4];
	char temp [1];
	int byte = 0;
	int i = 0;
	int done = 0;
	int count = 0;
	int char_count = 0; //used to count number of char per line in input
	while((byte=fread(temp,1,sizeof(temp),fp)) > 0)
	{
		if (char_count == 64) //if the 65th character is not a '\n', input is not in the right format of *.b64 file for decoding
		{
			if (temp[0] == '\n')
			{
				char_count = 0; //reset for new line in *.b64 file
			}
			else
			{
				fprintf(stderr, "Detected input file contains bad input data. Exiting program...\n");
				exit(1);
			}
		}
		while(temp[0] == '\n')
		{
			if((byte=fread(temp,1,sizeof(temp),fp)) <= 0)
			{
				break;
				done = 1;
			}
		}
		char_count += 1; //only count characters, not newline
		if(done == 0)
		{
			if ((temp[0] >= 'a' && temp[0] <= 'z') || (temp[0] >= 'A' && temp[0] <= 'Z') || (temp[0] >= '0' && temp[0] <= '9') || temp[0] == '+' || temp[0] == '/' || temp[0] == '\n' || temp[0] == '=')
			{
				buffer[i] = temp[0];
				if(i == 3)
				{
					if(buffer[2] == '=')
					{
						count = count + 1;
						decBase64(buffer,sizeof(buffer),2,1);	//know for sure there are 2 equal signs
					}
					else if(buffer[3] == '=')
					{
						count = count + 1;
						decBase64(buffer,sizeof(buffer),1,1);
					}
					else
					{
						decBase64(buffer,sizeof(buffer),0,0);
					}
					i = -1;
				}
				i++;
			}
			else
			{
				fprintf(stderr, "Detected input file contains bad input data. Exiting program...\n");
				exit(1);
			}
		}
	}
}
