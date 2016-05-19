/*
 * main.c
 *
 *  Created on: Jan 15, 2014
 *      Author: ntran
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "test.h"
#include "func.h"
#include "encBase64.h"
#include "decBase64.h"

int main (int argc, char **argv)
{
	FILE *fp = NULL;

	if(argc < 2)
	{
		fprintf(stderr, "Please specify the program hexdump/enc-base64/dec-base64 and try again\n");
	}
	else
	{
		if(strcmp(argv[1], "hexdump") == 0)
		{
			if(argc < 3)
			{
				processInputHexdump(stdin);
			}
			else
			{
				fp = fopen(argv[2], "r"); //open file for read
				if(fp == NULL)
				{
					fprintf(stderr, "Cannot open %s for reading. File does not exist\n",argv[2]);
					return 0;
				}
				processInputHexdump(fp);
				fclose(fp);
			}
		}
		else if(strcmp(argv[1], "enc-base64") == 0)
		{
			if(argc < 3)
			{
				processInputEncBase64(stdin);
			}
			else
			{
				fp = fopen(argv[2], "r"); //open file for read
				if(fp == NULL)
				{
					fprintf(stderr, "Cannot open %s for reading. File does not exist\n",argv[2]);
					return 0;
				}
				processInputEncBase64(fp);
				fclose(fp);
			}
		}
		else if(strcmp(argv[1], "dec-base64") == 0)
		{

			if(argc < 3)
			{
				processInputDecBase64(stdin);
			}
			else
			{
				fp = fopen(argv[2], "r"); //open file for read
				if(fp == NULL)
				{
					fprintf(stderr, "Cannot open %s for reading. File does not exist\n",argv[2]);
					return 0;
				}
				processInputDecBase64(fp);
				fclose(fp);
			}
		}
		else
		{
			fprintf(stderr, "Please enter the correct program name and try again\n");
		}
	}
	return 0;
}
