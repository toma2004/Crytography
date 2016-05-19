/*
 * tablecheck.c
 *
 *  Created on: Feb 25, 2016
 *      Author: ntran
 */

#include "tablecheck.h"

/*
 * Supplemental function to validate key=value of each line in tablefile
 * @param: pointer to line of char, and reference to my struct tablefile
 * @return: struct table with table index and 0 if invalid and 1 if valid
 */
struct table isValidLine (char *line, struct tablefile *table)
{
	struct table mytable;

	char *val = strchr(line,'=');
	if (val == NULL)
	{
		fprintf(stderr, "Detect invalid table file format. Missing \"=\" in one of the lines\n");
		mytable.isValid = 0;
		return mytable;
	}

	char *key = line;
	*val = '\0';
	val++;
	char *token;
	int num = 0;
	int count = 0;
	//Check if key input is a valid key
	if(strcmp(key,"IP") == 0)
	{
		//array to help check if there is dup number
		int checkDup [65];
		for (int k = 0; k < 65; k ++)
		{
			checkDup[k] = 0; //initialize checkDup array;
		}
		//check IP table
		token = strtok(val, ",");
		int index = 0;
		while (token != NULL)
		{
			count += 1;
			num = atoi(token);
			if(num < 1 || num > 64)
			{
				fprintf(stderr, "Detect an invalid number %d in IP table whose value should be from 1-64 inclusive\n", num);
				mytable.isValid = 0;
				return mytable;
			}
			checkDup[num] += 1;
			if (checkDup[num] > 1)
			{
				fprintf(stderr, "The value %d in IP table occurs too many times\n", num);
				mytable.isValid = 0;
				return mytable;
			}
			//add num to IP column
			if(index >= 64)
			{
				fprintf(stderr, "IP table has too many values\n");
				mytable.isValid = 0;
				return mytable;
			}
			table->IP[index] = num;
			index += 1;

			token = strtok(NULL, ",");
		}
		if (count < 64)
		{
			fprintf(stderr, "IP table has too few values\n");
			mytable.isValid = 0;
			return mytable;
		}
		else if (count > 64)
		{
			fprintf(stderr, "IP table has too many values\n");
			mytable.isValid = 0;
			return mytable;
		}
		//Everything is valid
		mytable.isValid = 1;
		mytable.tableIndex = 0;
		return mytable;

	}
	else if (strcmp(key, "E") == 0)
	{
		//check E table
		int checkNum[33];
		for(int k = 0; k < 33; k++)
		{
			checkNum[k] = 0; //initialize checkNum array
		}

		token = strtok(val, ",");
		int index = 0;
		while (token != NULL)
		{
			count += 1;
			num = atoi(token);
			if(num < 1 || num > 32)
			{
				fprintf(stderr, "Detect an invalid number %d in E table whose value should be from 1-32 inclusive\n", num);
				mytable.isValid = 0;
				return mytable;
			}
			checkNum[num] += 1;
			if (checkNum[num] > 2)
			{
				fprintf(stderr, "The value %d in E table occurs too many times.\n", num);
				mytable.isValid = 0;
				return mytable;
			}
			//add num to E table
			if(index >= 48)
			{
				fprintf(stderr, "E table has too many values\n");
				mytable.isValid = 0;
				return mytable;
			}
			table->E[index] = num;
			index += 1;

			token = strtok(NULL, ",");
		}
		if (count < 48)
		{
			fprintf(stderr, "E table has too few values\n");
			mytable.isValid = 0;
			return mytable;
		}
		else if (count > 48)
		{
			fprintf(stderr, "E table has too many values\n");
			mytable.isValid = 0;
			return mytable;
		}

		//check to see if all number appears at least 1
		for(int k = 1; k < 33; k++)
		{
			if (checkNum[k] < 1)
			{
				fprintf (stderr, "Number %d does not appear in table E\n", checkNum[k]);
				mytable.isValid = 0;
				return mytable;
			}
		}

		//everything is valid
		mytable.isValid = 1;
		mytable.tableIndex = 1;
		return mytable;

	}
	else if (strcmp(key, "P") == 0)
	{
		//check P table
		int checkNum[33];
		for(int k = 0; k < 33; k++)
		{
			checkNum[k] = 0; //initialize checkNum array
		}

		token = strtok(val, ",");
		int index = 0;
		while (token != NULL)
		{
			count += 1;
			num = atoi(token);
			if(num < 1 || num > 32)
			{
				fprintf(stderr, "Detect an invalid number %d in P table whose value should be from 1-32 inclusive\n", num);
				mytable.isValid = 0;
				return mytable;
			}
			checkNum[num] += 1;
			if (checkNum[num] > 1)
			{
				fprintf(stderr, "The value %d in P table occurs too many times\n", num);
				mytable.isValid = 0;
				return mytable;
			}
			//add num to P table
			if(index >= 32)
			{
				fprintf(stderr, "P table has too many values\n");
				mytable.isValid = 0;
				return mytable;
			}
			table->P[index] = num;
			index += 1;

			token = strtok(NULL, ",");
		}
		if (count < 32)
		{
			fprintf(stderr, "P table has too few values\n");
			mytable.isValid = 0;
			return mytable;
		}
		else if (count > 32)
		{
			fprintf(stderr, "P table has too many values\n");
			mytable.isValid = 0;
			return mytable;
		}

		//everything is valid
		mytable.isValid = 1;
		mytable.tableIndex = 2;
		return mytable;

	}
	else if (strcmp(key, "S1") == 0 || strcmp(key, "S2") == 0 || strcmp(key, "S3") == 0 || strcmp(key, "S4") == 0 || strcmp(key, "S5") == 0 || strcmp(key, "S6") == 0 || strcmp(key, "S7") == 0 || strcmp(key, "S8") == 0)
	{
		int sNum = -1;
		if(strcmp(key, "S1") == 0)
		{
			sNum = 0;
			mytable.tableIndex = 3;
		}
		else if(strcmp(key, "S2") == 0)
		{
			sNum = 1;
			mytable.tableIndex = 4;
		}
		else if(strcmp(key, "S3") == 0)
		{
			sNum = 2;
			mytable.tableIndex = 5;
		}
		else if(strcmp(key, "S4") == 0)
		{
			sNum = 3;
			mytable.tableIndex = 6;
		}
		else if(strcmp(key, "S5") == 0)
		{
			sNum = 4;
			mytable.tableIndex = 7;
		}
		else if(strcmp(key, "S6") == 0)
		{
			sNum = 5;
			mytable.tableIndex = 8;
		}
		else if(strcmp(key, "S7") == 0)
		{
			sNum = 6;
			mytable.tableIndex = 9;
		}
		else if(strcmp(key, "S8") == 0)
		{
			sNum = 7;
			mytable.tableIndex = 10;
		}

		//check S# tables
		int checkNum[16];
		for(int k = 0; k < 16; k++)
		{
			checkNum[k] = 0; //initialize checkNum array
		}
		int groupElementCount = 0;
		token = strtok(val, ",");
		int index = 0;
		while (token != NULL)
		{
			count += 1;
			groupElementCount += 1;
			num = atoi(token);
			if(num < 0 || num > 15)
			{
				fprintf(stderr, "Detect an invalid number %d in %s table whose value should be from 0-15 inclusive\n", num, key);
				mytable.isValid = 0;
				return mytable;
			}

			checkNum[num] += 1;
			if (checkNum[num] > 1)
			{
				fprintf(stderr, "The value %d in %s table occurs too many times in a group\n", num, key);
				mytable.isValid = 0;
				return mytable;
			}

			if(groupElementCount  == 16)
			{
				//finished 1 group, reset checkNum array for new group
				for(int k = 0; k < 16; k++)
				{
					checkNum[k] = 0; //initialize checkNum array
				}
				groupElementCount = 0;
			}
			//Add num to appropriate S# table
			if(sNum < 0 || sNum >=8 || index >= 64)
			{
				fprintf(stderr, "%s table has too many values\n", key);
				mytable.isValid = 0;
				return mytable;
			}
			table->S[sNum][index] = num;
			index += 1;

			token = strtok(NULL, ",");
		}
		if (count < 64)
		{
			fprintf(stderr, "%s table has too few values\n", key);
			mytable.isValid = 0;
			return mytable;
		}
		else if (count > 64)
		{
			fprintf(stderr, "%s table has too many values\n", key);
			mytable.isValid = 0;
			return mytable;
		}

		//everything is valid
		mytable.isValid = 1;
		return mytable;

	}
	else if (strcmp(key, "V") == 0)
	{
		//check V table
		int checkNum[3];
		for(int k = 0; k < 3; k++)
		{
			checkNum[k] = 0; //initialize checkNum array
		}

		token = strtok(val, ",");
		int index = 0;
		while (token != NULL)
		{
			count += 1;
			num = atoi(token);
			if(num < 1 || num > 2)
			{
				fprintf(stderr, "Detect an invalid number %d in V table whose value should be from 1-2 inclusive\n", num);
				mytable.isValid = 0;
				return mytable;
			}
			checkNum[num] += 1;

			//add num to V table
			if(index >= 16)
			{
				fprintf(stderr, "V table has too many values\n");
				mytable.isValid = 0;
				return mytable;
			}
			table->V[index] = num;
			index += 1;

			token = strtok(NULL, ",");
		}
		if (count < 16)
		{
			fprintf(stderr, "V table has too few values\n");
			mytable.isValid = 0;
			return mytable;
		}
		else if (count > 16)
		{
			fprintf(stderr, "V table has too many values\n");
			mytable.isValid = 0;
			return mytable;
		}

		//Check to see if we have 4 ones and 12 twos
		if (checkNum[1] != 4)
		{
			fprintf(stderr, "Don't have enough 4 ones in V table\n");
			mytable.isValid = 0;
			return mytable;
		}
		else if(checkNum[2] != 12)
		{
			fprintf(stderr, "Don't have enough 12 twos in V table\n");
			mytable.isValid = 0;
			return mytable;
		}

		//everything is valid
		mytable.isValid = 1;
		mytable.tableIndex = 11;
		return mytable;

	}
	else if (strcmp(key, "PC1") == 0)
	{
		//check PC1 table
		int checkNum[65];
		for(int k = 0; k < 65; k++)
		{
			checkNum[k] = 0; //initialize checkNum array
		}

		token = strtok(val, ",");
		int index = 0;
		while (token != NULL)
		{
			count += 1;
			num = atoi(token);
			if(num < 1 || num > 64)
			{
				fprintf(stderr, "Detect an invalid number %d in PC1 table whose value should be from 1-64 inclusive, and should not be multiple of 8 \n", num);
				mytable.isValid = 0;
				return mytable;
			}
			else if (num % 8 == 0)
			{
				fprintf(stderr, "Detect a number %d that is multiple of 8 in PC1 table\n", num);
				mytable.isValid = 0;
				return mytable;
			}
			checkNum[num] += 1;
			if (checkNum[num] > 1)
			{
				fprintf(stderr, "The value %d in PC1 table occurs too many times\n", num);
				mytable.isValid = 0;
				return mytable;
			}
			//add num to PC1 table
			if(index >= 56)
			{
				fprintf(stderr, "PC1 table has too many values\n");
				mytable.isValid = 0;
				return mytable;
			}
			table->PC1[index] = num;
			index += 1;

			token = strtok(NULL, ",");
		}
		if (count < 56)
		{
			fprintf(stderr, "PC1 table has too few values\n");
			mytable.isValid = 0;
			return mytable;
		}
		else if (count > 56)
		{
			fprintf(stderr, "PC1 table has too many values\n");
			mytable.isValid = 0;
			return mytable;
		}

		//everything is valid
		mytable.isValid = 1;
		mytable.tableIndex = 12;
		return mytable;

	}
	else if (strcmp(key, "PC2") == 0)
	{
		//check PC2 table
		int checkNum[57];
		for(int k = 0; k < 57; k++)
		{
			checkNum[k] = 0; //initialize checkNum array
		}

		token = strtok(val, ",");
		int index = 0;
		while (token != NULL)
		{
			count += 1;
			num = atoi(token);
			if(num < 1 || num > 56)
			{
				fprintf(stderr, "Detect an invalid number %d in PC2 table whose value should be from 1-56 inclusive\n", num);
				mytable.isValid = 0;
				return mytable;
			}
			checkNum[num] += 1;
			if (checkNum[num] > 1)
			{
				fprintf(stderr, "The value %d in PC2 table occurs too many times\n",num);
				mytable.isValid = 0;
				return mytable;
			}
			//add num to PC2 table
			if (index >= 48)
			{
				fprintf(stderr, "PC2 table has too many values\n");
				mytable.isValid = 0;
				return mytable;
			}
			table->PC2[index] = num;
			index += 1;

			token = strtok(NULL, ",");
		}
		if (count < 48)
		{
			fprintf(stderr, "PC2 table has too few values\n");
			mytable.isValid = 0;
			return mytable;
		}
		else if (count > 48)
		{
			fprintf(stderr, "PC2 table has too many values\n");
			mytable.isValid = 0;
			return mytable;
		}

		//everything is valid
		mytable.isValid = 1;
		mytable.tableIndex = 13;
		return mytable;

	}
	else
	{
		fprintf(stderr, "Detect an invalid table %s in table file\n", key);
		mytable.isValid = 0;
		return mytable;
	}
}

/*
 * Suuplemental function to compute inverse of IP table
 * @param: pointer to tablefile struct which contains IP table
 * @output: computed inverse IP table
 */
void computeIPinv(struct tablefile *table)
{
	int mappingArr[64];
	int map = 1;
	for(int k = 0; k < 64; k++)
	{
		mappingArr[table->IP[k] - 1] = map;
		map += 1;
	}

	//Now construct the IP inverse
	for (int k = 0; k < 64; k++)
	{
		table->IPInv[k] = mappingArr[k];
	}
}


/*
 * Check if a given tablefile is valid
 * @param: file pointer to tablefile
 * @return: struct tablefile containing all info if it's a valid. Otherwise, valid is set to 0
 */
struct tablefile isValidTablefile (FILE *fp)
{
	struct table mytable;
	struct tablefile table;

	table.isValid = 0;
	/*
	 * check to see if we have all the table and there is no duplicate tables
	 * index will represent table as follow:
	 * 0-IP, 1-E, 2-P, 3-10--S1-S8, 11-V, 12-PC1, 13-PC2
	 */
	int tablecheck[14];
	for (int k = 0; k < 14; k++)
	{
		tablecheck[k] = 0; //initialize tableCheck array
	}

	int size = 1024;
	char *line = malloc(size);
	int realloc_size = size;
	int index = 0;

	int count = 0;
	int byte = 0;
	char *mychar = malloc(1);

	while((byte = fread(mychar,1,1,fp)) > 0)
	{
		count += byte;
		if (count == size-1)
		{
			//reallocate more memory space
			realloc_size *= 2;
			char *temp = realloc(line, realloc_size);
			if (temp == NULL)
			{
				free(mychar);
				free(line);
				fprintf(stderr, "ERROR: Can't reallocate more space to buffer in order to store more input. Please check memory usage and capacity\n");
				return table;
			}
			line = temp;
			count = 1;
		}

		line[index] = *mychar;
		index += 1;

		//If see '\n', finish a line and start to process this line before moving on
		if (*mychar == '\n')
		{
			mytable = isValidLine(line, &table);
			if(mytable.isValid == 0)
			{
				free(mychar);
				free(line);
				return table;
			}
			else
			{
				//save the table that has been checked
				tablecheck[mytable.tableIndex] = 1;
			}
			//Reset all variable for next line
			free(line);
			line = malloc(size);
			realloc_size = size;
			index = 0;
			count = 0;
		}
	}

	free(mychar);
	free(line);

	//Check to see if we have all 14 table checked
	for (int k = 0; k < 14; k++)
	{
		if (tablecheck[k] < 1)
		{
			fprintf(stderr,"Missing at least 1 table in table file\n");
			return table;
		}
	}

	//Before exit, compute the IP inverse
	computeIPinv(&table);

	//Everything is valid
	table.isValid = 1;
	return table;
}
