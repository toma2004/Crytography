/*
 * main.c
 *
 *  Created on: Jan 26, 2016
 *      Author: ntran
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stream.h"
#include "encrypt.h"
#include "pbm.h"
#include "merge.h"
#include "decrypt.h"

/*
Re-write trim function to remove leading and trailing zeros
*/
char *trim_whitespace(char *str)
{
	char *first = str;
	//Find end of string and start to remove trailing whitespace
	char *end = str;
	while (*end != '\0')
	{
		end++;
	}
	//Subtract 1 from end to avoid null character
	end--;
	while (end != str)
	{
		if (*end == ' ')
		{
			*end = '\0';
			end--;
		}
		else
		{
			break; //found first non-whitespace character
		}
	}

	char *return_ptr = str;

	if (end == str)
	{
		*return_ptr = '\0';
	}
	else
	{
		//Start to remove leading whitespace
		end = str;
		while (*end == ' ')
		{
			end++;
		}


		if (*end != '\0' && end != str) //string has some leading whitespace
		{
			while (*end != '\0')
			{
				*first = *end;
				first++;
				end++;
			}
			//Add null character at the end
			first++;
			*first = '\0';
		}
		else if (*end == '\0') //all whitespace
		{
			*return_ptr = '\0';
		}
	}
	return return_ptr;
}

/*
 * Main function
 */
int main (int argc, char **argv)
{
	FILE *fp = NULL;

	if(argc < 2)
	{
		fprintf(stderr, "Please specify the programs stream/encrypt/merge/decrypt and try again\n");
	}
	else
	{
		if(strcmp(argv[1], "stream") == 0) //Processing stream command
		{
			if (argc != 4)
			{
				fprintf(stderr,"Please double check your arguments. The stream function expects to have exactly 2 arguments -p=pphrase and -l=len\n");
			}
			else
			{
				char *pphrase = "";
				int len = 0;
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL)
					{
						//malformed command
						fprintf(stderr,"Please double check your arguments. The stream function expects to have exactly 2 arguments in following format: -p=pphrase and -l=len\n");
						exit(1);
					}
					else
					{
						char *key = argv[index];
						*val = '\0'; //replace '=' by NULL character to separate key and val pointers
						val++;
						//compare to see which key is it
						if (strncmp(trim_whitespace(key), "-p", 2) == 0)
						{
							//passphrase
							pphrase = val;
						}
						else if(strncmp(trim_whitespace(key), "-l", 2) == 0)
						{
							//key length
							len = atoi(val);
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The stream function expects to have exactly 2 arguments in following format: -p=pphrase and -l=len\n");
							exit(1);
						}
					}
				}
				//Check if we have value for pphrase and len before calling stream function
				if (strncmp(pphrase, "",1) != 0 && *pphrase != '\0' && len > 0)
				{
					//call stream function
					key_stream_generator(pphrase, len);
				}
				else
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The stream function expects to have exactly 2 arguments in following format: -p=pphrase and -l=len\n");
					exit(1);
				}
			}
		}
		else if(strcmp(argv[1], "encrypt") == 0) //processing encrypt command
		{
			if (argc < 4)
			{
				fprintf(stderr,"Please double check your arguments. The encrypt function expects to have at least 2 mandatory arguments in this exact format -p=pphrase and -out=name\n");
			}
			else if (argc == 4 || argc == 5)
			{
				char *pphrase = "";
				char *out = "";
				char *filename = "";
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL && argc == 4)
					{
						//malformed command
						fprintf(stderr,"Please double check your arguments. The encrypt function expects to have at least 2 mandatory arguments in this exact format -p=pphrase and -out=name\n");
						exit(1);
					}
					else if (val == NULL && argc == 5)
					{
						//Found pbmfile name
						filename = trim_whitespace(argv[index]);
					}
					else
					{
						char *key = argv[index];
						*val = '\0'; //replace '=' by NULL character to separate key and val pointers
						val++;
						//compare to see which key is it
						if (strncmp(trim_whitespace(key), "-p", 2) == 0)
						{
							//passphrase
							pphrase = val;
						}
						else if(strncmp(trim_whitespace(key), "-out", 4) == 0)
						{
							//output file name
							out = val;
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The encrypt function expects to have at least 2 mandatory arguments in this exact format -p=pphrase and -out=name\n");
							exit(1);
						}
					}
				}
				//Check if we have value for pphrase, out file name, and optionally pbmfile before calling encrypt function
				if (strncmp(pphrase, "",1) != 0 && *pphrase != '\0' && strncmp(out, "",1) != 0 && *out != '\0')
				{
					if (argc == 5) //expect to have pbmfile
					{
						if (strncmp(filename, "",1) == 0 || *filename == '\0')
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. It seems you might be missing pbmfile\n");
							exit(1);
						}
						else
						{
							fp =fopen(filename, "r");
							if (fp == NULL)
							{
								//malformed command
								fprintf(stderr,"Could not open pbmfile %s. File does not exist\n", filename);
								exit(1);
							}

							encrypt (pphrase, out, fp);
							fclose(fp);
						}
					}
					else if (argc == 4)
					{
						//read pbmfile from stdin
						encrypt (pphrase, out, stdin);
					}
				}
				else
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The encrypt function expects to have at least 2 mandatory arguments in this exact format -p=pphrase and -out=name\n");
					exit(1);
				}
			}
			else
			{
				//malformed command
				fprintf(stderr,"Please double check your arguments. The encrypt function expects to have at least 2 mandatory arguments -p=pphrase, -out=name, and an optional pbmfile name\n");
				exit(1);
			}

		}
		else if(strcmp(argv[1], "merge") == 0)
		{
			//process merge
			if (argc == 4)
			{
				fp = fopen(argv[2], "r");
				if (fp == NULL)
				{
					fprintf(stderr, "File %s does not exist\n", argv[2]);
					exit(1);
				}
				//open another file reader to read second image
				FILE *fp2;
				fp2 = fopen(argv[3], "r");
				if (fp2 == NULL)
				{
					fprintf(stderr, "File %s does not exist\n", argv[3]);
					fclose(fp); //close fp before exit
					exit(1);
				}
				struct PBM_input pbmInput1 = checkPBM(fp);
				struct PBM_input pbmInput2 = checkPBM(fp2);

				//check if 2 images have the same size
				if (strcmp(pbmInput1.width, pbmInput2.width) != 0 || strcmp(pbmInput1.height,pbmInput2.height) != 0)
				{
					fprintf(stderr, "Image sizes do not match\n");
					fclose(fp); //close fp before exit
					fclose(fp2); //close fp2 before exit
					exit(1);
				}

				//start to merge 2 files given pointers to data and width/height of each image
				mergePBMFiles(fp,fp2,atoi(pbmInput1.width),atoi(pbmInput1.height));

				free(pbmInput1.buffer);
				free(pbmInput2.buffer);
				fclose(fp2);
				fclose(fp);
			}
			else
			{
				fprintf(stderr, "Please ensure to have 2 .pbm files as input in order to merge\n");
			}

		}
		else if(strcmp(argv[1], "decrypt") == 0)
		{
			//process decrypt
			if (argc > 3)
			{
				fprintf(stderr, "Please ensure that you only submit 1 pbm file\n");
				exit(1);
			}
			else
			{
				if (argc == 3) //read pbm file from input file
				{
					fp = fopen(argv[2], "r");
					if (fp == NULL)
					{
						fprintf(stderr, "File %s does not exist\n", argv[2]);
						exit(1);
					}
					decrypt(fp);

					fclose(fp);
				}
				else //read from stdin
				{
					decrypt(stdin);
				}
			}
		}
		else
		{
			fprintf(stderr, "Please enter the correct program name and try again\n");
		}
	}
}
