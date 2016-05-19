/*
 * main.c
 *
 *  Created on: Mar 12, 2016
 *      Author: ntran
 */

#include "tablecheck_AES.h"
#include "modprod.h"
#include "keyexpand.h"
#include "encrypt.h"
#include "decrypt.h"
#include "inverse.h"

/*
 * Main function
 */
int main (int argc, char **argv)
{
	FILE *fp = NULL;

	if(argc < 2)
	{
		fprintf(stderr, "Please specify the programs tablecheck/modprod/keyexpand/encrypt/decrypt/inverse and try again\n");
	}
	else
	{
		if(strcmp(argv[1], "tablecheck") == 0) //Processing tablecheck command
		{
			if (argc != 3)
			{
				fprintf(stderr,"Please double check your arguments. The tablecheck function expects to have exactly 1 argument -t=tablefile\n");
				exit(1);
			}
			else
			{
				char *tablefile = "";
				//find '=' at each argument
				char *val = strchr(argv[2], '=');
				if (val == NULL)
				{
					fprintf(stderr, "Please double check your arguments. The tablecheck function expects to have exactly 1 argument -t=tablefile\n");
					exit(1);
				}
				else
				{
					char *checkArgs = argv[2];
					*val = '\0'; //replace '=' by NULL character to separate key and val pointers
					val++;
					//compare to see if the right argument is passed in
					if (strncmp(checkArgs, "-t", 2) == 0)
					{
						//tablefile
						tablefile = val;
					}
					else
					{
						fprintf(stderr, "Please double check your arguments. The tablecheck function expects to have exactly 1 argument -t=tablefile\n");
						exit(1);
					}
					//Attempt to open tablefile
					fp = fopen(tablefile,"r");
					if(fp == NULL)
					{
						fprintf(stderr, "Table file %s does not exist.\n",tablefile);
						exit(1);
					}

					//Check if table is valid
					isValidTablefile(fp);

					fclose(fp);
				}
			}
		}
		else if (strcmp(argv[1], "modprod") == 0) //process modprod command
		{
			if (argc != 4)
			{
				fprintf(stderr,"Please double check your arguments. The modprod function expects to have exactly 2 arguments -p1=poly1 and -p2=poly2\n");
			}
			else
			{
				char *poly1 = "";
				char *poly2 = "";
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL)
					{
						//malformed command
						fprintf(stderr,"Please double check your arguments. The modprod function expects to have exactly 2 arguments -p1=poly1 and -p2=poly2\n");
						exit(1);
					}
					else
					{
						char *key = argv[index];
						*val = '\0'; //replace '=' by NULL character to separate key and val pointers
						val++;
						//compare to see which key is it
						if (strncmp(key, "-p1", 3) == 0)
						{
							//poly1
							poly1 = val;
						}
						else if(strncmp(key, "-p2", 3) == 0)
						{
							//poly2
							poly2 = val;
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The modprod function expects to have exactly 2 arguments -p1=poly1 and -p2=poly2\n");
							exit(1);
						}
					}
				}
				//Check if we have value for poly1 and poly2 before calling modprod function
				if (strncmp(poly1, "",1) != 0 && *poly1 != '\0' && strncmp(poly2, "",1) != 0 && *poly2 != '\0')
				{
					//Call function to check if we have valid poly1 and poly2
					struct mypoly poly_check = check_and_store_poly_input(poly1, poly2);
					if(poly_check.isValid)
					{
						//Call modprod function
						modprod(poly_check.poly1, poly_check.poly2, 1);
					}
				}
				else
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The modprod function expects to have exactly 2 arguments -p1=poly1 and -p2=poly2\n");
					exit(1);
				}
			}
		}
		else if(strcmp(argv[1], "keyexpand") == 0)
		{
			if (argc != 4)
			{
				fprintf(stderr,"Please double check your arguments. The keyexpand function expects to have exactly 2 arguments -k=key and -t=tablefile\n");
			}
			else
			{
				char *inputKey = "";
				char *tablefile = "";
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL)
					{
						//malformed command
						fprintf(stderr,"Please double check your arguments. The keyexpand function expects to have exactly 2 arguments -k=key and -t=tablefile\n");
						exit(1);
					}
					else
					{
						char *key = argv[index];
						*val = '\0'; //replace '=' by NULL character to separate key and val pointers
						val++;
						//compare to see which key is it
						if (strncmp(key, "-k", 2) == 0)
						{
							//key
							inputKey = val;
						}
						else if(strncmp(key, "-t", 2) == 0)
						{
							//tablefile
							tablefile = val;
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The keyexpand function expects to have exactly 2 arguments -k=key and -t=tablefile\n");
							exit(1);
						}
					}
				}
				//Check if we have value for key and tablefile before calling keyexapnd function
				if (strncmp(inputKey, "",1) != 0 && *inputKey != '\0' && strncmp(tablefile, "",1) != 0 && *tablefile != '\0')
				{
					//Check to see if key is valid
					struct mykey validKey = isValidKey(inputKey);
					if(validKey.isValid)
					{
						//Try to open tablefile to ensure that it exists
						fp = fopen(tablefile, "r");
						if(fp == NULL)
						{
							fprintf(stderr, "Tablefile %s does not exist.\n", tablefile);
							exit(1);
						}
						//Check to see if tablefile is a valid
						struct tablefile mytable = isValidTablefile(fp);
						if(mytable.isValid)
						{
							//call keyexpand function
							keyexpansion(validKey, mytable, 1);
						}
						fclose(fp);
					}
				}
				else
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The keyexpand function expects to have exactly 2 arguments -k=key and -t=tablefile\n");
					exit(1);
				}
			}
		}
		else if(strcmp(argv[1], "encrypt") == 0 || strcmp(argv[1], "decrypt") == 0) //processing encrypt/decrypt command
		{

			if (argc == 4 || argc == 5) //expect to have 2 or 3 argument on top of the executable name and function name
			{
				char *input_key = "";
				char *tablefile = "";
				char *filename = "";
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL)
					{
						if (argc == 5)
						{
							//Must be the input file
							filename = argv[index];
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The encrypt/decrypt function expects to have at least 2 arguments in following format: -k=key and -t=tablefile, and an optional input file\n");
							exit(1);
						}
					}
					else
					{
						char *checkArgs = argv[index];
						*val = '\0'; //replace '=' by NULL character to separate key and val pointers
						val++;
						//compare to see which key is it
						if (strncmp(checkArgs, "-t", 2) == 0)
						{
							//tablefile
							tablefile = val;
						}
						else if(strncmp(checkArgs, "-k", 2) == 0)
						{
							//key file
							input_key = val;
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The encrypt/decrypt function expects to have at least 2 arguments in following format: -k=key and -t=tablefile, and an optional input file\n");
							exit(1);
						}
					}
				}

				if (strcmp(input_key,"") == 0 || strcmp(tablefile,"") == 0)
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The encrypt/decrypt function expects to have at least 2 arguments in following format: -k=key and -t=tablefile, and an optional input file\n");
					exit(1);
				}
				else if (argc == 5 && strcmp(filename,"") == 0)
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The encrypt/decrypt function expects to have at least 2 arguments in following format: -k=key and -t=tablefile, and an optional input file\n");
					exit(1);
				}

				/*
				 * Try to open file to check if file exists
				 */
				FILE *fp_tablefile = fopen(tablefile, "r");
				if (fp_tablefile == NULL)
				{
					fprintf(stderr, "Tablefile %s does not exist\n",tablefile);
					exit(1);
				}

				/*
				 * Try to open input file if supplied
				 */

				if (argc == 5)
				{
					fp = fopen(filename,"r");
					if (fp == NULL)
					{
						fprintf(stderr,"Input file %s does not exist.\n", filename);
						fclose(fp_tablefile);
						exit(1);
					}
				}

				/*
				 * Check key to see if it's a valid key
				 */
				struct mykey key = isValidKey(input_key);
				if(key.isValid)
				{
					struct tablefile mytable = isValidTablefile(fp_tablefile);
					if(mytable.isValid)
					{
						//Both key and tablefile are valid, now call encrypt/decrypt function with key file, tablefile and input file from stdin or from input file
						if (argc == 4)
						{
							if(strcmp(argv[1], "encrypt") == 0)
							{
								//Call encrypt
								encrypt(mytable, key, stdin);

							}
							else if (strcmp(argv[1], "decrypt") == 0)
							{
								//call decrypt
								decrypt(mytable, key, stdin);
							}
						}
						else
						{
							if(strcmp(argv[1], "encrypt") == 0)
							{
								//call encrypt
								encrypt(mytable, key, fp);

							}
							else if(strcmp(argv[1], "decrypt") == 0)
							{
								//call decrypt
								decrypt(mytable, key, fp);

							}
							fclose(fp);
						}
						fclose(fp_tablefile);
					}
				}
			}
			else
			{
				//malformed command
				fprintf(stderr,"Please double check your arguments. The encrypt/decrypt function expects to have at least 2 arguments in following format: -k=key and -t=tablefile, and an optional input file\n");
				exit(1);
			}

		}
		else if(strcmp(argv[1], "inverse") == 0) //process inverse command
		{
			if (argc != 3)
			{
				fprintf(stderr,"Please double check your arguments. The inverse function expects to have exactly 1 argument -p=poly\n");
				exit(1);
			}
			else
			{
				char *poly = "";
				//find '=' at each argument
				char *val = strchr(argv[2], '=');
				if (val == NULL)
				{
					fprintf(stderr, "Please double check your arguments. The inverse function expects to have exactly 1 argument -p=poly\n");
					exit(1);
				}
				else
				{
					char *checkArgs = argv[2];
					*val = '\0'; //replace '=' by NULL character to separate key and val pointers
					val++;
					//compare to see if the right argument is passed in
					if (strncmp(checkArgs, "-p", 2) == 0)
					{
						//poly
						poly = val;
					}
					else
					{
						fprintf(stderr, "Please double check your arguments. The inverse function expects to have exactly 1 argument -p=poly\n");
						exit(1);
					}
					//Check if we have value for poly
					if (strncmp(poly, "",1) != 0 && *poly != '\0')
					{
						struct inverse_poly mypoly = readPoly(poly);
						if(mypoly.isValid)
						{
							//call inverse function
							inverse(mypoly.inv_poly);
						}
					}
					else
					{
						fprintf(stderr, "Please double check your arguments. The inverse function expects to have exactly 1 argument -p=poly and poly is represented in hex string\n");
						exit(1);
					}
				}
			}
		}
		else
		{
			fprintf(stderr, "Please enter the correct program name and try again. Programs supported are: tablecheck/modprod/keyexpand/encrypt/decrypt/inverse\n");
		}
	}
}

