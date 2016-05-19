/*
 * main.c
 *
 *  Created on: Apr 16, 2016
 *      Author: ntran
 */

#include "rc4.h"
#include "x1.h"
#include "x2.h"
#include "x3.h"
#include "x4.h"
#include "x5.h"

/*
 * Main function
 */
int main (int argc, char **argv)
{
	FILE *fp = NULL;

	if(argc < 2)
	{
		fprintf(stderr, "Please specify the programs rc4/x1/x2/x3/x4/x5 and try again\n");
	}
	else
	{
		if(strcmp(argv[1], "rc4") == 0) //Processing rc4 command
		{
			if (argc < 3 || argc > 4)
			{
				fprintf(stderr,"Please double check your arguments. The rc4 function expects to have 1 required argument -p=pphrase and an optional argument -l=len\n");
				exit(1);
			}
			else
			{
				char *pphrase_input = "";
				char *len = "";
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL)
					{
						//malformed command
						fprintf(stderr,"Please double check your arguments. The rc4 function expects to have 1 required argument -p=pphrase and an optional argument -l=len\n");
						exit(1);
					}
					else
					{
						char *key = argv[index];
						*val = '\0'; //replace '=' by NULL character to separate key and val pointers
						val++;
						//compare to see which key is it
						if (strncmp(key, "-p", 2) == 0)
						{
							//pass phrase
							pphrase_input = val;
						}
						else if(strncmp(key, "-l", 2) == 0)
						{
							//length
							len = val;
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The rc4 function expects to have 1 required argument -p=pphrase and an optional argument -l=len\n");
							exit(1);
						}
					}
				}
				if (strncmp(pphrase_input, "",1) != 0 && *pphrase_input != '\0')
				{
					//Check to see if len is provided
					int valid_len = 0; //By default, assuming there is no given len, we should output all 256 byte of last state in key scheduling algorithm
					if (strncmp(len, "",1) != 0 && *len != '\0')
					{
						//len is provided, check if len is valid
						valid_len = check_len(len);
						if(valid_len == 0)
						{
							//given len is not valid. Exit program
							exit(1);
						}
					}

					//check if pass phrase is valid
					int isValid_pphrase = check_pphrase(pphrase_input);
					if(isValid_pphrase)
					{
						//call rc4 function
						rc4(pphrase_input, valid_len);
					}

				}
				else
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The rc4 function expects to have 1 required argument -p=pphrase and an optional argument -l=len\n");
					exit(1);
				}
			}
		}
		else if (strcmp(argv[1], "x1") == 0 || strcmp(argv[1], "x2") == 0 || strcmp(argv[1], "x3") == 0 || strcmp(argv[1], "x4") == 0) //process x1/x2/x3/x4 command
		{
			if(argc >  3)
			{
				//malformed command
				fprintf(stderr,"Please double check your arguments. The x1/x2/x3/x4 function expects to have an optional argument [file]\n");
				exit(1);
			}
			else if (argc == 3) //file input in given in argument
			{
				fp = fopen(argv[2], "r");
				if(fp == NULL)
				{
					fprintf(stderr, "File %s does not exist\n", argv[2]);
					exit(1);
				}

				if(strcmp(argv[1], "x1") == 0)
				{
					//call x1 function
					x1(fp);
				}
				else if(strcmp(argv[1], "x2") == 0)
				{
					//call x2 function
					x2(fp);
				}
				else if(strcmp(argv[1], "x3") == 0)
				{
					//call x3 function
					x3(fp);
				}
				else if(strcmp(argv[1], "x4") == 0)
				{
					//call x4 function
					x4(fp);
				}

				fclose(fp);
			}
			else //expect to read file input from stdin
			{
				if(strcmp(argv[1], "x1") == 0)
				{
					//call x1 function
					x1(stdin);
				}
				else if(strcmp(argv[1], "x2") == 0)
				{
					//call x2 function
					x2(stdin);
				}
				else if(strcmp(argv[1], "x3") == 0)
				{
					//call x3 function
					x3(stdin);
				}
				else if(strcmp(argv[1], "x4") == 0)
				{
					//call x4 function
					x4(stdin);
				}
			}
		}
		else if(strcmp(argv[1], "x5") == 0) //process x5 command
		{
			if (argc < 3 || argc > 4)
			{
				fprintf(stderr,"Please double check your arguments. The x5 function expects to have 1 required argument -d=dval and an optional argument file\n");
				exit(1);
			}
			else
			{
				char *dval = "";
				char *filename = "";
				int isFoundFile = 0; //prevent error where user enters 2 file names
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL)
					{
						if(!isFoundFile)
						{
							//it is file name
							filename = argv[index];
							isFoundFile = 1;
						}
						else
						{
							fprintf(stderr,"File name has been found before, please double check your arguments. The x5 function expects to have 1 required argument -d=dval and an optional argument file\n");
							exit(1);
						}
					}
					else
					{
						char *key = argv[index];
						*val = '\0'; //replace '=' by NULL character to separate key and val pointers
						val++;
						//compare to see which key is it
						if (strncmp(key, "-d", 2) == 0)
						{
							//d val
							dval = val;
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The x5 function expects to have 1 required argument -d=dval and an optional argument file\n");
							exit(1);
						}
					}
				}
				if (strncmp(dval, "",1) != 0 && *dval != '\0')
				{
					//check to see if dval is valid
					int valid_dval = check_len(dval); //utilize check_len() function in rc4.h to check if given dval is valid
					if(valid_dval == 0)
					{
						//given dval is not valid. Exit program
						exit(1);
					}

					//check if we have filename input. If we don't, read from stdin
					if (strncmp(filename, "",1) != 0 && *filename != '\0')
					{
						//file is given
						fp = fopen(filename, "r");
						if(fp == NULL)
						{
							fprintf(stderr, "File %s does not exist\n", filename);
							exit(1);
						}

						//call x5 function
						x5(valid_dval, fp);

						fclose(fp);
					}
					else
					{
						//read from stdin
						//call x5 function
						x5(valid_dval, stdin);
					}

				}
				else
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The x5 function expects to have a required argument -d=dval and an optional argument file\n");
					exit(1);
				}
			}
		}
		else
		{
			fprintf(stderr, "Please enter the correct program name and try again. Programs supported are: rc4/x1/x2/x3/x4/x5\n");
		}
	}
}
