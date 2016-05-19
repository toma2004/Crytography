/*
 * solve.c
 *
 *  Created on: Feb 20, 2016
 *      Author: ntran
 */

#include "solve.h"

/*
 * Supplemental function to read ciphertext file input and store it internally for post process
 * @param: file pointer to input file
 * @return: struct that contains char pointer to ciphertext input and total char count
 */
struct ciphertextInfo read_ciphertext (FILE *fp)
{
	struct ciphertextInfo myciphertext;
	int size = 1024; //initial allocation size, will double every time we need more space
	myciphertext.ciphertext = malloc(size);
	int index = 0;

	char *mychar = malloc(1);
	int byte = 0;
	int count = 0;
	int count_char = 0;
	int count_alphabet_char = 0;

	int realloc_size = size;
	//read in char by char, and store internally
	while ((byte = fread(mychar,1,1,fp)) > 0)
	{
		count += byte;
		count_char += byte;
		//Check the size to see if we reach capacity
		if (count == size - 1)
		{
			realloc_size *= 2;
			char *temp = realloc(myciphertext.ciphertext, realloc_size);
			if (temp == NULL)
			{
				free(mychar);
				free(myciphertext.ciphertext);
				fprintf(stderr, "ERROR: Can't reallocate more space to buffer in order to store more input. Please check memory usage and capacity\n");
				return myciphertext;
			}
			myciphertext.ciphertext = temp;
			count = 1;
		}

		//Check ciphertext input
		if (*mychar != '\n' && (*mychar < 0x20 || *mychar > 0x7e))
		{
			free(mychar);
			free(myciphertext.ciphertext);
			fprintf(stderr, "ERROR: detect invalid character in ciphertext input. Expecting to have char with hex value from 0x20-0x7e\n");
			return myciphertext; //should be NULL
		}
		//Check if char is lowercase English alphabet
		if(*mychar >= 'a' && *mychar <= 'z')
		{
			count_alphabet_char += 1;
		}

		myciphertext.ciphertext[index] = *mychar;
		index += 1;
	}

	myciphertext.total_char = count_char;
	myciphertext.total_alphabet_char = count_alphabet_char;

	free(mychar);
	return myciphertext; //myciphertext.ciphertext will be freed upon function call
}

/*
 * Use Kasiski method to find repetitive phrase with certain length in ciphertext
 * @param: struct that contains char pointer to ciphertext input and total char count
 * @output: Kasis Method report to show the index of repetitive phrase with certain length
 * EX: len=4, i=41, j=131, j-i=90, zstv
 */
void findKasiski(struct ciphertextInfo ciphertext)
{
	fprintf(stdout,"Kasiski Method\n");
	fprintf(stdout,"==============\n");
	int len = 3; //start with phrase of length 4, will increment right after loop begin
	int i = 0;
	int j = 1;

	int foundMatchStr = 1;
	int isMatched = 1;
	int done = 0;
	int atleast1Match = 0;
	//Increment to next len and continue the process
	while(!done)
	{
		len += 1;
		/*
		 * reset i and j for next round of new length
		 */
		i = 0;
		j = 1;
		//Go through cipher text input to find the phrase with length 4 where all letters in the phrase are lowercase in alphabet
		while (i+len < ciphertext.total_char)
		{
			for(int k = i; k < i+len; k++)
			{
				if(ciphertext.ciphertext[k] < 'a' || ciphertext.ciphertext[k] > 'z')
				{
					foundMatchStr = 0;
					break; //increment i
				}
			}
			if (!foundMatchStr)
			{
				//reset foundMatchStr
				foundMatchStr = 1;
			}
			else
			{
				//start with j at where i+1 to continue to search for the remaining ciphertext characters.
				j = i + 1;
				//Go through the rest of cipher text to find if there is a string that matches with the foundMatchStr
				while(j+len < ciphertext.total_char)
				{
					int temp = i;
					for(int k = j; k < j+len; k++)
					{
						if(ciphertext.ciphertext[k] < 'a' || ciphertext.ciphertext[k] > 'z' || ciphertext.ciphertext[k] != ciphertext.ciphertext[temp])
						{
							isMatched = 0;
							break; //increment i
						}
						temp += 1;
					}
					if (!isMatched)
					{
						//reset isMatched for next round if we don't find any match in previous attempt
						isMatched = 1;
					}
					else
					{
						//Found a match, print to stdout
						char *matchStr = malloc(len);
						int temp_index = 0;
						for (int k = j; k < j+len; k++)
						{
							matchStr[temp_index] = ciphertext.ciphertext[k];
							temp_index += 1;
						}
						fprintf(stdout,"len=%d, i=%d, j=%d, j-i=%d, %s\n",len,i,j,j-i,matchStr);
						free(matchStr);
						atleast1Match = 1;
					}
					j += 1;
				}
			}
			//increment i for next round
			i += 1;
		}
		if (!atleast1Match)
		{
			fprintf(stdout,"len=%d, no more matches\n",len);
			done = 1;
		}
		else
		{
			//reset atleast1Match for next round
			atleast1Match = 0;
		}
	}
}

/*
 * Calculate and tabulate average index of coincidence
 * @param:struct that contains char pointer to ciphertext input and total char count
 * @param: maximum number of periods
 * @output: table of average index of coincidence
 */
void avgIndexCoincidence(struct ciphertextInfo ciphertext, int max_t)
{
	char alphabet_ref[] = "abcdefghijklmnopqrstuvwxyz";
	int char_count[26];
	double kp = 0.0658; //define kp constant
	//Initialize char count array to be zero
	for (int k = 0; k < 26; k++)
	{
		char_count[k] = 0;
	}

	//char *mychar = malloc(1);
	int byte = 0;
	while (byte < ciphertext.total_char)
	{
		//Checking ciphertext for invalid char
		if (ciphertext.ciphertext[byte] != '\n' && (ciphertext.ciphertext[byte] < 0x20 || ciphertext.ciphertext[byte] > 0x7e))
		{
			fprintf(stderr, "ERROR: detect undefined character in input file\n");
			//free(mychar);
			return;
		}

		if (ciphertext.ciphertext[byte] >= 'a' && ciphertext.ciphertext[byte] <= 'z')
		{
			char_count[((ciphertext.ciphertext[byte] - 71) % 26)] += 1;
		}
		byte += 1;
	}
	fprintf(stdout,"\nAverage Index of Coincidence\n");
	fprintf(stdout,"============================\n");
	fprintf(stdout,"L=%d\n",ciphertext.total_alphabet_char);
	//calculate IC
	double ic = 0;
	for(int k = 0; k < 26; k++)
	{
		fprintf(stdout,"f('%c')=%d\n",alphabet_ref[k],char_count[k]);
		ic += (char_count[k]*(char_count[k]-1));
	}
	ic = ic / (ciphertext.total_alphabet_char*(ciphertext.total_alphabet_char-1));
	fprintf(stdout,"IC=%.8g\n",ic);

	//Now calculate E(IC)
	for(int k = 1; k <= max_t; k++)
	{
		double eic = (((double)1/k)*((double)(ciphertext.total_alphabet_char-k)/(ciphertext.total_alphabet_char-1))*kp) + (((double)(k-1)/k)*((double)ciphertext.total_alphabet_char/(ciphertext.total_alphabet_char-1))*((double)1/26));
		fprintf(stdout,"t=%d, E(IC)=%.8g\n",k,eic);
	}
}


/*
 * Auto-correlation Index of Coincidence
 * @param:struct that contains char pointer to ciphertext input and total char count
 * @param: maximum number of periods
 * @output: tablet of Aauto-correlation Method
 */
void autoCorrelation(struct ciphertextInfo ciphertext, int max_t)
{
	fprintf(stdout,"\nAuto-correlation Method\n");
	fprintf(stdout,"=======================\n");
	for(int k = 1; k <= max_t; k++)
	{
		int count = 0;
		int i = 0;
		while(i < (ciphertext.total_char - k))
		{
			if(ciphertext.ciphertext[i] >= 'a' && ciphertext.ciphertext[i] <= 'z' && ciphertext.ciphertext[i+k] >= 'a' && ciphertext.ciphertext[i+k] <= 'z' && ciphertext.ciphertext[i] == ciphertext.ciphertext[i+k])
			{
				count += 1;
			}
			i += 1;
		}
		fprintf(stdout,"t=%d, count=%d\n",k,count);
	}
	fprintf(stdout,"\n");
}

/*
 * solve function to call sub function to do Kasiski, average index of coincidence, and auto-correlation of coincidence
 * @param: file pointer to input file and max_t period
 * @output: Kasiski, average index of coincidence, and auto-correlation of coincidence outputs
 */
void solve(FILE *fp, int max_t)
{
	//Read and store ciphertext input file
	struct ciphertextInfo myciphertext = read_ciphertext(fp);

	//Call function Kasiski to output Kasiski outcomes
	findKasiski(myciphertext);

	//Call function average index of coincidence to output avg IoC outcome
	avgIndexCoincidence(myciphertext, max_t);

	//Call function to perform auto-correlation Index of Coincidence method
	autoCorrelation(myciphertext, max_t);

	free(myciphertext.ciphertext);
}
