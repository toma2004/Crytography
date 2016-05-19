/*
 * tablecheck_AES.c
 *
 *  Created on: Mar 12, 2016
 *      Author: ntran
 */

#include "tablecheck_AES.h"

/*
 * Supplemental function to validate key=value of each line in tablefile
 * @param: pointer to line of char, and reference to my struct tablefile
 * @return: struct table with table index and 0 if invalid and 1 if valid
 */
struct table isValidLine (char *line, struct tablefile *table)
{
	struct table mytable;
	mytable.isValid = 1; //assume table file is valid.

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

	int count = 0;
	uint8_t temp1 = 0x0;
	uint8_t temp2 = 0x0;
	uint8_t holder = 0x0;
	unsigned char *temp = malloc(1);

	//Check if key input is a valid key
	if(strcmp(key,"S") == 0)
	{
		int s_row = 0;
		int s_col = 0;
		//array to help check if there is dup number
		int checkDup [16][16];
		for (int i = 0; i < 16; i ++)
		{
			for(int k = 0; k < 16; k++)
			{
				checkDup[i][k] = 0; //initialize checkDup array;
			}
		}
		while(*val != '\0' && *val != '\n')
		{
			if(*val >= '0' && *val <= '9')
			{
				*temp = (unsigned char)*val - 48; //representation of char 0-9 to hex of 0-9
			}
			else if(*val >= 'a' && *val <= 'f')
			{
				*temp = (unsigned char)*val - 87; //representation of char a-f to hex of 10-15
			}
			else
			{
				fprintf(stderr,"Detect invalid key '%c' in S table. Values should be represented in hexstring\n", *val);
				mytable.isValid = 0;
				break;
			}
			count += 1;
			if (count > 512) //max value is 16x16 = 256 for a hex of 2 chars. Therefore, total char expected to be read in is 256x2 = 512
			{
				fprintf(stderr,"S table has too many values\n");
				mytable.isValid = 0;
				break;
			}
			if (count % 2 !=  0)
			{
				temp1 = (uint8_t)*temp;
			}
			else
			{
				temp2 = (uint8_t)*temp;
				//Check to see if we store this byte before, if so, found a duplicate in tablefile => Report error
				if (checkDup[temp1][temp2] == 1)
				{
					fprintf(stderr, "The value %x%x in S table occurs too many times\n", temp1, temp2);
					mytable.isValid = 0;
					break;
				}
				else
				{
					checkDup[temp1][temp2] = 1;
				}
				int temp1_index = 3;
				int temp2_index = 3;
				//now set the first 4 bits of holder to be temp1, and last 4 bits of holder to be temp2
				for(int k = 7; k >= 0; k--)
				{
					if(temp1_index >= 0)
					{
						if((temp1 & ((0x01) << temp1_index)) != 0) //bit is set, then set it in holder
						{
							holder |= ((0x01) << k);
						}
						temp1_index -= 1;
					}
					else
					{
						if((temp2 & ((0x01) << temp2_index)) != 0) //bit is set, then set it in holder
						{
							holder |= ((0x01) << k);
						}
						temp2_index -= 1;
					}
				}
				//Store this byte value into our S-box table
				table->S[s_row][s_col] = holder;
				s_col += 1;
				if (s_col >= 16)
				{
					s_col = 0;
					s_row += 1;
				}
				//reset holder for next round
				holder = 0x0;
			}
			val += 1;
		}
		if (count < 512 && mytable.isValid == 1)
		{
			fprintf(stderr, "S table has too few values\n");
			mytable.isValid = 0;
		}
		free(temp);

		mytable.tableIndex = 0; //set table index
		return mytable;
	}
	else if(strcmp(key,"P") == 0 || strcmp(key, "INVP") == 0)
	{
		int p_index = 0;
		while(*val != '\0' && *val != '\n')
		{
			if(*val >= '0' && *val <= '9')
			{
				*temp = (unsigned char)*val - 48; //representation of char 0-9 to hex of 0-9
			}
			else if(*val >= 'a' && *val <= 'f')
			{
				*temp = (unsigned char)*val - 87; //representation of char a-f to hex of 10-15
			}
			else
			{
				fprintf(stderr,"Detect invalid char '%c' in table %s. Poly should be represented in hexstring\n", *val, key);
				mytable.isValid = 0;
				break;
			}
			count += 1;
			if (count > 8) //expect to have 8 chars or 4 bytes for a(x) coefficient
			{
				fprintf(stderr,"%s table has too many values\n", key);
				mytable.isValid = 0;
				break;
			}
			if (count % 2 !=  0)
			{
				temp1 = (uint8_t)*temp;
			}
			else
			{
				temp2 = (uint8_t)*temp;
				int temp1_index = 3;
				int temp2_index = 3;
				//now set the first 4 bits of holder to be temp1, and last 4 bits of holder to be temp2
				for(int k = 7; k >= 0; k--)
				{
					if(temp1_index >= 0)
					{
						if((temp1 & ((0x01) << temp1_index)) != 0) //bit is set, then set it in holder
						{
							holder |= ((0x01) << k);
						}
						temp1_index -= 1;
					}
					else
					{
						if((temp2 & ((0x01) << temp2_index)) != 0) //bit is set, then set it in holder
						{
							holder |= ((0x01) << k);
						}
						temp2_index -= 1;
					}
				}
				//Store this byte value into our P or INVP array representing a(x) or a-1(x) coeff respectively.
				if(strcmp(key, "P") == 0)
				{
					table->P[p_index] = holder;
				}
				else
				{
					table->INVP[p_index] = holder;
				}
				p_index += 1;
				//reset holder for next round
				holder = 0x0;
			}
			val += 1;
		}
		if (count < 8 && mytable.isValid == 1)
		{
			fprintf(stderr, "%s table has too few values\n", key);
			mytable.isValid = 0;
		}
		free(temp);

		if(strcmp(key, "P") == 0)
		{
			mytable.tableIndex = 1; //set table index to be P table
		}
		else
		{
			mytable.tableIndex = 2; //set table index to be INVP table
		}
		return mytable;
	}
	else
	{
		fprintf(stderr, "Detect an invalid table %s in table file\n", key);
		free(temp);
		mytable.isValid = 0;
		return mytable;
	}
}

/*
 * Supplemental function to translate of 2 int inputs into 1 uint8_t output
 * @param: 2 int input, give that they are within range 0-15
 * @output: uint8_t
 */
uint8_t translateIntsToUint8(int i, int k)
{
	uint8_t myreturn = 0x0;
	int i_index = 3; //only care the last 4 bits of int i
	int k_index = 3; //only care the last 4 bits of int k
	for(int j = 7; j >= 0; j--)
	{
		if(i_index >= 0) //copy last 4 bits of int i into the first 4 bits of myreturn uint8_t
		{
			if((i & ((0x01) << i_index)) != 0)
			{
				myreturn |= ((0x01) << j);
			}
			i_index -= 1;
		}
		else //copy the last 4 bits of int k into the last 4 bits of myreturn uint8_t
		{
			if((k & ((0x01) << k_index)) != 0)
			{
				myreturn |= ((0x01) << j);
			}
			k_index -= 1;
		}
	}
	return myreturn;
}

/*
 * Supplemental function to translate a uint8_t into 2 int representing row and col of S-box
 * @param: uint8_t value
 * @output: struct s_box_coor containing the row and col in order to locate the value in s_box
 */
struct s_box_coor translateUint8toInt(uint8_t input)
{
	struct s_box_coor returnCoor;

	int i = 0;
	int i_index = 3; //only care last 4 bit of i
	int k = 0;
	int k_index = 3; //only care last 4 bit of k

	for(int j = 7; j >= 0; j--)
	{
		if(j >=4) //first 4 bits stored in row
		{
			if((input & ((0x01) << j)) != 0)
			{
				i |= ((0x01) << i_index);
			}
			i_index -= 1;
		}
		else
		{
			if((input & ((0x01) << j)) != 0)
			{
				k |= ((0x01) << k_index);
			}
			k_index -= 1;
		}
	}
	returnCoor.row = i;
	returnCoor.col = k;

	return returnCoor;
}

/*
 * Suuplemental function to compute inverse of S table
 * @param: pointer to tablefile struct which contains S table
 * @output: computed inverse S table
 */
void computeSinv(struct tablefile *table)
{
	uint8_t temp_row = 0x0;
	int temp_row_index = 3;
	uint8_t temp_col = 0x0;
	int temp_col_index = 3;
	for(int i = 0; i < 16; i++)
	{
		for(int k = 0; k < 16; k++)
		{
			//store the first 4 bits to temp_row and last 4 bits to temp_col
			for(int j = 7; j >= 0; j--)
			{
				if(temp_row_index >= 0)
				{
					if((table->S[i][k] & ((0x01) << j)) != 0)
					{
						temp_row |= ((0x01) << temp_row_index);
					}
					temp_row_index -= 1;
				}
				else
				{
					if((table->S[i][k] & ((0x01) << j)) != 0)
					{
						temp_col |= ((0x01) << temp_col_index);
					}
					temp_col_index -= 1;
				}
			}
			//using the temp_col and temp_row as indices to store in INVS table
			table->INVS[temp_row][temp_col] = translateIntsToUint8(i,k);

			//reset values for next round
			temp_row = 0x0;
			temp_row_index = 3;
			temp_col = 0x0;
			temp_col_index = 3;
		}
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
	 * 0-S, 1-P, 2-INVP
	 */
	int tablecheck[3];
	for (int k = 0; k < 3; k++)
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

	//Check to see if we have all 3 tables checked
	for (int k = 0; k < 3; k++)
	{
		if (tablecheck[k] < 1)
		{
			if(k == 0)
			{
				fprintf(stderr,"Missing table S in table file\n");
			}
			else if(k == 1)
			{
				fprintf(stderr,"Missing table P in table file\n");
			}
			else
			{
				fprintf(stderr,"Missing table INVP in table file\n");
			}
			return table;
		}
	}
	//Check to see if P CIRCLEX INVP = {00}{00}{00}{01}
	struct modprod_result P_INVP_check = modprod(table.P, table.INVP, 0);
	if(P_INVP_check.modprod_poly[0] != 0x00 || P_INVP_check.modprod_poly[1] != 0x00 || P_INVP_check.modprod_poly[2] != 0x00 || P_INVP_check.modprod_poly[3] != 0x01)
	{
		fprintf(stderr, "Detect INVP is not a multiplicative inverse of P, please double check your input\n");
		return table;
	}

	//compute the  inverse of S box
	computeSinv(&table);

	//Everything is valid
	table.isValid = 1;
	return table;
}
