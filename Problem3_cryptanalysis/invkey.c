/*
 * invkey.c
 *
 *  Created on: Feb 15, 2016
 *      Author: ntran
 */

#include "invkey.h"

/*
 * function to invert a given key file
 * @param keyfile
 * @output inverted key file
 */
void invkey (FILE *fp)
{
	char ref[] = "abcdefghijklmnopqrstuvwxyz";
	char result[] = "abcdefghijklmnopqrstuvwxyz";
	char *buffer = malloc(27); //account for newline character
	char *temp = buffer;
	int byte = 0;
	int index = 0;
	while ((byte = fread(buffer,1,27,fp)) > 0)
	{
		temp = buffer;
		while (*temp != '\n' || index < 26)
		{
			//Check each character to see if they are in range of a-z lower case
			if (*temp < 'a' || *temp > 'z')
			{
				//Malformed input file, expect to have exactly 26 characters and follow by a newline character
				fprintf(stderr, "ERROR: detect incorrect keyfile\n");
				free(buffer);
				exit(1);
			}
			unsigned int index_result = (((unsigned int) *(temp)) - 71) % 26;
			//assign the value of index to index_result
			result[index_result] = ref[index];
			index += 1;
			temp += 1;
		}

		if (*temp != '\n' || index != 26)
		{
			//Malformed input file, expect to have exactly 26 characters and follow by a newline character
			fprintf(stderr, "ERROR: detect incorrect keyfile\n");
			free(buffer);
			exit(1);
		}
		fwrite(result,1,sizeof(result)-1,stdout);
		fwrite("\n", 1, 1, stdout);
		strcpy(result,"abcdefghijklmnopqrstuvwxyz");
		index = 0;
	}
	free(buffer);
}
