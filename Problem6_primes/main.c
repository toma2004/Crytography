/*
 * main.c
 *
 *  Created on: Mar 29, 2016
 *      Author: ntran
 */

#include "primes.h"
#include "trialdiv.h"
#include "millerrabin.h"
#include "rndsearch.h"
#include "maurer.h"

/*
 * Main function
 */
int main (int argc, char **argv)
{
	FILE *fp = NULL;

	if(argc < 2)
	{
		fprintf(stderr, "Please specify the programs primes/trialdiv/millerrabin/rndsearch/maurer and try again\n");
	}
	else
	{
		if(strcmp(argv[1], "primes") == 0) //Processing primes command
		{
			if (argc != 3)
			{
				fprintf(stderr,"Please double check your arguments. The primes function expects to have exactly 1 argument -n=maxval\n");
				exit(1);
			}
			else
			{
				char *maxval_input = "";
				//find '=' at each argument
				char *val = strchr(argv[2], '=');
				if (val == NULL)
				{
					fprintf(stderr, "Please double check your arguments. The primes function expects to have exactly 1 argument -n=maxval\n");
					exit(1);
				}
				else
				{
					char *checkArgs = argv[2];
					*val = '\0'; //replace '=' by NULL character to separate key and val pointers
					val++;
					//compare to see if the right argument is passed in
					if (strncmp(checkArgs, "-n", 2) == 0)
					{
						//found maxval
						maxval_input = val;
					}
					else
					{
						fprintf(stderr, "Please double check your arguments. The primes function expects to have exactly 1 argument -n=maxval\n");
						exit(1);
					}
					//Verify valid maxval
					struct mymaxval maxval = check_and_return_maxval(maxval_input);
					if(maxval.isValid)
					{
						//call primes function
						primes(maxval.maxval);
					}
				}
			}
		}
		else if (strcmp(argv[1], "trialdiv") == 0) //process trialdiv command
		{
			if (argc != 4)
			{
				fprintf(stderr,"Please double check your arguments. The trialdiv function expects to have exactly 2 arguments -n=number and -p=primesfile\n");
			}
			else
			{
				char *inputNum = "";
				char *primesfile = "";
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL)
					{
						//malformed command
						fprintf(stderr,"Please double check your arguments. The trialdiv function expects to have exactly 2 arguments -n=number and -p=primesfile\n");
						exit(1);
					}
					else
					{
						char *key = argv[index];
						*val = '\0'; //replace '=' by NULL character to separate key and val pointers
						val++;
						//compare to see which key is it
						if (strncmp(key, "-n", 2) == 0)
						{
							//number
							inputNum = val;
						}
						else if(strncmp(key, "-p", 2) == 0)
						{
							//primes file
							primesfile = val;
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The trialdiv function expects to have exactly 2 arguments -n=number and -p=primesfile\n");
							exit(1);
						}
					}
				}
				//Check if we have value for number and primesfile before calling trialdiv function
				if (strncmp(inputNum, "",1) != 0 && *inputNum != '\0' && strncmp(primesfile, "",1) != 0 && *primesfile != '\0')
				{
					//Check to see if num is valid
					struct myNum validNum = read_verify_store_number(inputNum);
					if(validNum.isValid)
					{
						//Try to open primesfile to ensure that it exists
						fp = fopen(primesfile, "r");
						if(fp == NULL)
						{
							fprintf(stderr, "Primesfile %s does not exist.\n", primesfile);
							BN_free(validNum.bn_n); //free from function call read_verify_store_number()
							exit(1);
						}
						//call trialdiv function
						trialdiv(validNum.bn_n, fp, 0);
						fclose(fp);
						BN_free(validNum.bn_n); //free from function call read_verify_store_number()
					}
				}
				else
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The trialdiv function expects to have exactly 2 arguments -n=number and -p=primesfile\n");
					exit(1);
				}
			}
		}
		else if(strcmp(argv[1], "millerrabin") == 0) //process miller-rabin command
		{
			if (argc != 5)
			{
				fprintf(stderr,"Please double check your arguments. The millerrabin function expects to have exactly 3 arguments -n=number, -t=maxitr and -p=primesfile\n");
			}
			else
			{
				char *inputNum = "";
				char *inputMaxItr = "";
				char *primesfile = "";
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL)
					{
						//malformed command
						fprintf(stderr,"Please double check your arguments. The millerrabin function expects to have exactly 3 arguments -n=number, -t=maxitr and -p=primesfile\n");
						exit(1);
					}
					else
					{
						char *key = argv[index];
						*val = '\0'; //replace '=' by NULL character to separate key and val pointers
						val++;
						//compare to see which key is it
						if (strncmp(key, "-n", 2) == 0)
						{
							//number
							inputNum = val;
						}
						else if(strncmp(key, "-p", 2) == 0)
						{
							//primes file
							primesfile = val;
						}
						else if(strncmp(key, "-t", 2) == 0)
						{
							//maxitr
							inputMaxItr = val;
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The millerrabin function expects to have exactly 3 arguments -n=number, -t=maxitr and -p=primesfile\n");
							exit(1);
						}
					}
				}
				//Check if we have value for number, maxitr and primesfile before calling millerrabin function
				if (strncmp(inputNum, "",1) != 0 && *inputNum != '\0' && strncmp(primesfile, "",1) != 0 && *primesfile != '\0' && strncmp(inputMaxItr, "", 1) != 0 && *inputMaxItr != '\0')
				{
					//Check to see if num is valid
					struct myNum validNum = read_verify_store_number(inputNum);
					//check to see if maxItr is valid
					struct myIntInput validMaxItr = check_and_store_maxitr(inputMaxItr);
					if(validNum.isValid && validMaxItr.isValid)
					{
						//Try to open primesfile to ensure that it exists
						fp = fopen(primesfile, "r");
						if(fp == NULL)
						{
							fprintf(stderr, "Primesfile %s does not exist.\n", primesfile);
							BN_free(validNum.bn_n); //free from function call read_verify_store_number()
							exit(1);
						}
						//call millerrabin function
						millerrabin(validNum.bn_n, validMaxItr.intInput, fp, 0);

						fclose(fp);
						BN_free(validNum.bn_n); //free from function call read_verify_store_number()

					}
				}
				else
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The millerrabin function expects to have exactly 3 arguments -n=number, -t=maxitr and -p=primesfile\n");
					exit(1);
				}
			}
		}
		else if(strcmp(argv[1], "rndsearch") == 0) //processing rndsearch command
		{
			if (argc != 6)
			{
				fprintf(stderr,"Please double check your arguments. The rndsearch function expects to have exactly 4 arguments -k=numbits, -t=maxitr, -p=primesfile, and -r=rndfile\n");
			}
			else
			{
				char *inputNum = "";
				char *inputMaxItr = "";
				char *primesfile = "";
				char *rndfile = "";
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL)
					{
						//malformed command
						fprintf(stderr,"Please double check your arguments. The rndsearch function expects to have exactly 4 arguments -k=numbits, -t=maxitr, -p=primesfile, and -r=rndfile\n");
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
							//numbits
							inputNum = val;
						}
						else if(strncmp(key, "-p", 2) == 0)
						{
							//primes file
							primesfile = val;
						}
						else if(strncmp(key, "-t", 2) == 0)
						{
							//maxitr
							inputMaxItr = val;
						}
						else if(strncmp(key, "-r", 2) == 0)
						{
							//rndfile
							rndfile = val;
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The rndsearch function expects to have exactly 4 arguments -k=numbits, -t=maxitr, -p=primesfile, and -r=rndfile\n");
							exit(1);
						}
					}
				}
				//Check if we have value for numbits, maxitr, rndfile and primesfile before calling rndsearch function
				if (strncmp(inputNum, "",1) != 0 && *inputNum != '\0' && strncmp(primesfile, "",1) != 0 && *primesfile != '\0' && strncmp(inputMaxItr, "", 1) != 0 && *inputMaxItr != '\0' && strncmp(rndfile, "", 1) != 0 && *rndfile != '\0')
				{
					//Check to see if numbit is valid
					struct myIntInput validNum = check_and_store_maxitr(inputNum); //Utilize the maxitr function to check for inputNumBits since they have the same set of checks

					//check to see if maxItr is valid
					struct myIntInput validMaxItr = check_and_store_maxitr(inputMaxItr);
					if(validNum.isValid && validMaxItr.isValid)
					{
						//Try to open primesfile to ensure that it exists
						fp = fopen(primesfile, "r");
						if(fp == NULL)
						{
							fprintf(stderr, "Primesfile %s does not exist.\n", primesfile);
							exit(1);
						}

						//Try to open rndfile to ensure that it exists
						FILE *fp_rndfile = NULL;
						fp_rndfile = fopen(rndfile, "r");
						if(fp_rndfile == NULL)
						{
							fprintf(stderr, "Rndfile %s does not exist.\n", rndfile);
							fclose(fp); //close fp before exit
							exit(1);
						}

						//Calling rndsearch function
						rndsearch(validNum.intInput, validMaxItr.intInput, fp, fp_rndfile);

						fclose(fp);
						fclose(fp_rndfile);
					}
				}
				else
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The rndsearch function expects to have exactly 4 arguments -k=numbits, -t=maxitr, -p=primesfile, and -r=rndfile\n");
					exit(1);
				}
			}
		}
		else if(strcmp(argv[1], "maurer") == 0) //process maurer command
		{
			if (argc != 5)
			{
				fprintf(stderr,"Please double check your arguments. The maurer function expects to have exactly 3 arguments -k=numbits, -p=primesfile, and -r=rndfile\n");
			}
			else
			{
				char *inputNum = "";
				char *primesfile = "";
				char *rndfile = "";
				//Use for loop to go through all arguments
				for (int index = argc-1; index > 1; index--)
				{
					//find '=' at each argument
					char *val = strchr(argv[index], '=');
					if (val == NULL)
					{
						//malformed command
						fprintf(stderr,"Please double check your arguments. The maurer function expects to have exactly 3 arguments -k=numbits, -p=primesfile, and -r=rndfile\n");
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
							//numbits
							inputNum = val;
						}
						else if(strncmp(key, "-p", 2) == 0)
						{
							//primes file
							primesfile = val;
						}
						else if(strncmp(key, "-r", 2) == 0)
						{
							//rndfile
							rndfile = val;
						}
						else
						{
							//malformed command
							fprintf(stderr,"Please double check your arguments. The maurer function expects to have exactly 3 arguments -k=numbits, -p=primesfile, and -r=rndfile\n");
							exit(1);
						}
					}
				}
				//Check if we have value for numbits, maxitr, rndfile and primesfile before calling rndsearch function
				if (strncmp(inputNum, "",1) != 0 && *inputNum != '\0' && strncmp(primesfile, "",1) != 0 && *primesfile != '\0' && strncmp(rndfile, "", 1) != 0 && *rndfile != '\0')
				{
					//Check to see if numbit is valid
					struct myIntInput validNum = check_and_store_maxitr(inputNum); //Utilize the maxitr function to check for inputNumBits since they have the same set of checks

					if(validNum.isValid)
					{
						//Try to open primesfile to ensure that it exists
						fp = fopen(primesfile, "r");
						if(fp == NULL)
						{
							fprintf(stderr, "Primesfile %s does not exist.\n", primesfile);
							exit(1);
						}

						//Try to open rndfile to ensure that it exists
						FILE *fp_rndfile = NULL;
						fp_rndfile = fopen(rndfile, "r");
						if(fp_rndfile == NULL)
						{
							fprintf(stderr, "Rndfile %s does not exist.\n", rndfile);
							fclose(fp); //close fp before exit
							exit(1);
						}

						//Calling maurer function
						maurer_wrapper(validNum.intInput, fp, fp_rndfile);

						fclose(fp);
						fclose(fp_rndfile);
					}
				}
				else
				{
					//malformed command
					fprintf(stderr,"Please double check your arguments. The maurer function expects to have exactly 3 arguments -k=numbits, -p=primesfile, and -r=rndfile\n");
					exit(1);
				}
			}
		}
		else
		{
			fprintf(stderr, "Please enter the correct program name and try again. Programs supported are: primes/trialdiv/millerrabin/rndsearch/maurer\n");
		}
	}
}
