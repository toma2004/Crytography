/*
 * main.c
 *
 *  Created on: Feb 25, 2016
 *      Author: ntran
 */

#include "tablecheck.h"
#include "encrypt.h"
#include "decrypt.h"
#include "encrypt3.h"
#include "decrypt3.h"

/*
 * Main function
 */
int main (int argc, char **argv)
{
	FILE *fp = NULL;

	if(argc < 2)
	{
		fprintf(stderr, "Please specify the programs tablecheck/encrypt/decrypt/encrypt3/decrypt3 and try again\n");
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
					isValidTablefile(fp);

					fclose(fp);
				}
			}
		}
		else if(strcmp(argv[1], "encrypt") == 0 || strcmp(argv[1], "decrypt") == 0 || strcmp(argv[1], "encrypt3") == 0 || strcmp(argv[1], "decrypt3") == 0 ) //processing encrypt/decrypt, encrypt3/decrypt3 command
		{
			if (argc == 4 || argc == 5) //expect to have 2 or 3 argument on top of the executable name and function name
			{
				char *key = "";
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
							fprintf(stderr,"Please double check your arguments. The encrypt/decrypt or encrypt3/decrypt3 function expects to have at least 2 arguments in following format: -k=key and -t=tablefile, and an optional input file\n");
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
							key = val;
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The encrypt/decrypt or encrypt3/decrypt3 function expects to have at least 2 arguments in following format: -k=key and -t=tablefile, and an optional input file\n");
							exit(1);
						}
					}
				}

				if (strcmp(key,"") == 0 || strcmp(tablefile,"") == 0)
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The encrypt/decrypt or encrypt3/decrypt3 function expects to have at least 2 arguments in following format: -k=key and -t=tablefile, and an optional input file\n");
					exit(1);
				}
				else if (argc == 5 && strcmp(filename,"") == 0)
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The encrypt/decrypt or encrypt3/decrypt3 function expects to have at least 2 arguments in following format: -k=key and -t=tablefile, and an optional input file\n");
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
				struct key mykey;
				struct key3 mykey3;
				if(strcmp(argv[1], "encrypt") == 0 || strcmp(argv[1], "decrypt") == 0)
				{
					mykey = isValidKey(key);
				}
				else
				{
					mykey3 = isValid_Key3(key);
				}

				//Check if tablefile is valid
				struct tablefile mytable = isValidTablefile(fp_tablefile);
				struct subkeys mysubkeys;
				if(strcmp(argv[1], "encrypt") == 0 || strcmp(argv[1], "decrypt") == 0)
				{
					if(mykey.isValid && mytable.isValid)
					{
						mysubkeys = keySchedule(mytable,mykey);
					}
					else
					{
						//fprintf(stderr, "Detect an invalid key file or tablefile!\n");
						fclose(fp_tablefile);
						fclose(fp);
						exit(1);
					}
				}
				else
				{
					if(mykey3.isValid == 0 || mytable.isValid == 0)
					{
						//fprintf(stderr, "Detect an invalid key file or tablefile!\n");
						fclose(fp_tablefile);
						fclose(fp);
						exit(1);
					}
				}

				//Now call encrypt function with key file, tablefile and input file from stdin or from input file
				if (argc == 4)
				{
					if(strcmp(argv[1], "encrypt") == 0)
					{
						//Call encrypt
						encrypt(mytable, mysubkeys, stdin);
					}
					else if (strcmp(argv[1], "decrypt") == 0)
					{
						//call decrypt
						decrypt(mytable, mysubkeys, stdin);
					}
					else if(strcmp(argv[1], "encrypt3") == 0)
					{
						//Call encrypt3
						encrypt3(mytable, mykey3, stdin);
					}
					else if (strcmp(argv[1], "decrypt3") == 0)
					{
						//Call decrypt3
						decrypt3(mytable, mykey3, stdin);
					}
				}
				else
				{
					if(strcmp(argv[1], "encrypt") == 0)
					{
						//call encrypt
						encrypt(mytable, mysubkeys, fp);
					}
					else if(strcmp(argv[1], "decrypt") == 0)
					{
						//call decrypt
						decrypt(mytable, mysubkeys, fp);
					}
					else if (strcmp(argv[1], "encrypt3") == 0)
					{
						//call encrypt3
						encrypt3(mytable, mykey3, fp);
					}
					else if (strcmp(argv[1], "decrypt3") == 0)
					{
						//call decrypt3
						decrypt3(mytable, mykey3, fp);
					}
					fclose(fp);
				}
				fclose(fp_tablefile);

			}
			else
			{
				//malformed command
				fprintf(stderr,"Please double check your arguments. The encrypt/decrypt or encrypt3/decrypt3 function expects to have at least 2 arguments in following format: -k=key and -t=tablefile, and an optional input file\n");
				exit(1);
			}

		}
		else
		{
			fprintf(stderr, "Please enter the correct program name and try again\n");
		}
	}
}
