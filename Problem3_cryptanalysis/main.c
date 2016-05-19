/*
 * main.c
 *
 *  Created on: Feb 14, 2016
 *      Author: ntran
 */

#include "keygen.h"
#include "invkey.h"
#include "crypt.h"
#include "histo.h"
#include "solve.h"
/*
 * Main function
 */
int main (int argc, char **argv)
{
	FILE *fp = NULL;

	if(argc < 2)
	{
		fprintf(stderr, "Please specify the programs keygen/crypt/invkey/histo/solve and try again\n");
	}
	else
	{
		if(strcmp(argv[1], "keygen") == 0) //Processing keygen command
		{
			if (argc != 4)
			{
				fprintf(stderr,"Please double check your arguments. The keygen function expects to have exactly 2 arguments -p=pphrase and -t=period\n");
			}
			else
			{
				char *pphrase = "";
				int period = 0;
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL)
					{
						//malformed command
						fprintf(stderr,"Please double check your arguments. The keygen function expects to have exactly 2 arguments in following format: -p=pphrase and -t=period\n");
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
							//passphrase
							pphrase = val;
						}
						else if(strncmp(key, "-t", 2) == 0)
						{
							//key length
							period = atoi(val);
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The keygen function expects to have exactly 2 arguments in following format: -p=pphrase and -t=period\n");
							exit(1);
						}
					}
				}
				//Check if we have value for pphrase and len before calling keygen function
				if (strncmp(pphrase, "",1) != 0 && *pphrase != '\0' && period > 0)
				{
					//call keygen function
					generate_key(pphrase, period);
				}
				else
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The keygen function expects to have exactly 2 arguments in following format: -p=pphrase and -t=period\n");
					exit(1);
				}
			}
		}
		else if(strcmp(argv[1], "crypt") == 0) //processing crypt command
		{
			if (argc < 3 || argc > 4)
			{
				fprintf (stderr, "Please double check your arguments. The crypt function expects to have at least 1 argument in following format: -k=keyfile and an optional file to encrypt/decrypt\n");
				exit(1);
			}
			else if (argc == 3)
			{
				//Check if there is a -k argument. If so, read the file from stdin
				char *keyfile = "";
				char *checkArgs = "";
				//find '=' at argument
				char *val = strchr(argv[2], '=');
				if (val == NULL)
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The crypt function expects to have at least 1 argument in following format: -k=keyfile and an optional file to encrypt/decrypt\n");
					exit(1);
				}
				else
				{
					checkArgs = argv[2];
					if (strncmp(checkArgs, "-k", 2) != 0)
					{
						//malformed command
						fprintf(stderr,"Please double check your arguments. The crypt function expects to have at least 1 argument in following format: -k=keyfile and an optional file to encrypt/decrypt\n");
						exit(1);
					}
					else
					{
						*val = '\0';
						val++;
						keyfile = val;
						//Now try to open the key file, if open successfully, continue. Else, exit with error
						fp = fopen(keyfile,"r");
						if (fp == NULL)
						{
							fprintf (stderr, "Keyfile %s does not exist\n", keyfile);
							exit(1);
						}
						//Now call crypt function with fp to read keyfile and stdin to read file to be encrypted/decrypted
						crypt(fp, keyfile, stdin);
						fclose(fp);
					}
				}
			}
			else if (argc == 4)
			{
				char *keyfile = "";
				char *file = "";
				char *checkArgs = "";
				FILE *fp2;
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL)
					{
						//This must be the file name. Let try to open this argument to see if file exists
						file = argv[index];
					}
					else
					{
						checkArgs = argv[index];
						*val = '\0'; //replace '=' by NULL character to separate key and val pointers
						val++;
						//compare to see which key is it
						if (strncmp(checkArgs, "-k", 2) == 0)
						{
							//keyfile
							keyfile = val;
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The crypt function expects to have at least 1 argument in following format: -k=keyfile and an optional file to encrypt/decrypt\n");
							exit(1);
						}
					}
				}
				if (strcmp(file,"") == 0 || strcmp(keyfile,"") == 0)
				{
					fprintf(stderr, "Either keyfile or file is missing. Please double check your arguments\n");
					exit(1);
				}
				//Now try to open both file and keyfile to see both exist
				fp = fopen(keyfile, "r");
				if (fp == NULL)
				{
					fprintf (stderr, "Keyfile %s does not exist\n", keyfile);
					exit(1);
				}

				fp2 = fopen(file, "r");
				if (fp2 == NULL)
				{
					fprintf (stderr, "File %s does not exist\n", file);
					//close fp
					fclose(fp);
					exit(1);
				}
				//Call crypt function with fp for keyfile and fp2 for file to be encrypted/decrypted
				crypt(fp, keyfile, fp2);
				fclose(fp);
				fclose(fp2);
			}
		}
		else if(strcmp(argv[1], "invkey") == 0) //process invkey
		{
			if (argc == 3)
			{
				//printf("my letter a number = %d\n",'z');
				fp = fopen(argv[2], "r");
				if(fp == NULL)
				{
					fprintf(stderr, "File %s does not exist.\n", argv[2]);
					exit(1);
				}
				invkey(fp);
				fclose(fp);
			}
			else
			{
				fprintf(stderr, "Please double check your arguments. The invkey function expects to have only 1 argument which is the keyfile\n");
				exit(1);
			}
		}
		else if(strcmp(argv[1], "histo") == 0) //processing histo command
		{
			if (argc == 4 || argc == 5) //expect to have 2 or 3 argument on top of the executable name and function name
			{
				int which = 0;
				int period = 0;
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
							fprintf(stderr,"Please double check your arguments. The histo function expects to have at least 2 arguments in following format: -t=period and -i=which, and an optional input file\n");
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
							//period
							period = atoi(val);
						}
						else if(strncmp(checkArgs, "-i", 2) == 0)
						{
							//which
							which = atoi(val);
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The histo function expects to have at least 2 arguments in following format: -t=period and -i=which, and an optional input file\n");
							exit(1);
						}
					}
				}

				if (which <= 0 || period <= 0)
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The histo function expects to have at least 2 arguments in following format: -t=period and -i=which, and an optional input file\n");
					exit(1);
				}
				else if (argc == 5 && strcmp(filename,"") == 0)
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The histo function expects to have at least 2 arguments in following format: -t=period and -i=which, and an optional input file\n");
					exit(1);
				}
				//Check if we have which is greater than the period. If so, return error
				if (which > period)
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The histo function does not allow --which value is greater than --period value\n");
					exit(1);
				}
				//Now call histo function with period, which, and input file from stdin or from input file
				if (argc == 4)
				{
					histo(period,which,stdin);
				}
				else
				{
					fp = fopen(filename,"r");
					if (fp == NULL)
					{
						fprintf(stderr,"File %s does not exist.\n", filename);
						exit(1);
					}
					histo(period,which,fp);
					fclose(fp);
				}

			}
			else
			{
				//malformed command
				fprintf(stderr,"Please double check your arguments. The histo function expects to have at least 2 arguments in following format: -t=period and -i=which, and an optional input file\n");
				exit(1);
			}
		}
		else if(strcmp(argv[1], "solve") == 0) //processing solve command
		{
			if (argc == 4)
			{
				char *filename = "";
				int max_t = 0;
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL)
					{
						//Must be file name
						filename = argv[index];
					}
					else
					{
						char *checkArgs = argv[index];
						*val = '\0'; //replace '=' by NULL character to separate key and val pointers
						val++;
						//compare to see which key is it
						if (strncmp(checkArgs, "-l", 2) == 0)
						{
							//max_t period
							max_t = atoi(val);
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The solve function expects to have exactly 2 arguments in following format: -l=max_t and input file\n");
							exit(1);
						}
					}
				}
				if (strcmp(filename,"") == 0 || max_t == 0)
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The solve function expects to have exactly 2 arguments in following format: -l=max_t and input file\n");
					exit(1);
				}
				else
				{
					fp = fopen(filename, "r");
					if(fp == NULL)
					{
						fprintf(stderr,"File %s does not exist\n", filename);
						exit(1);
					}
					//Call solve function
					solve(fp,max_t);
					fclose(fp);
				}
			}
			else
			{
				//malformed command
				fprintf(stderr,"Please double check your arguments. The solve function expects to have exactly 2 arguments in following format: -l=max_t and input file\n");
				exit(1);
			}
		}
		else
		{
			fprintf(stderr, "Please enter the correct program name and try again\n");
		}
	}
}



