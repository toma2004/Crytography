/*
 * encrypt.c
 *
 *  Created on: Feb 25, 2016
 *      Author: ntran
 */

#include "encrypt.h"

/*
 * Supplemental function to check if key is valid
 * @param: pointer to key file
 * @reutrn: struct key containing key and 0 if invalid, and 1 if valid
 */
struct key isValidKey(char *key)
{
	char *mychar = key;
	struct key mykey;
	for (int k = 0; k < 8; k++)
	{
		mykey.key[k] = '\0'; //initialize key array
	}

	unsigned char *temp = malloc(1);
	int flip = 1;
	int index = 0;
	int count = 0;
	mykey.isValid = 1;

	while(*mychar != '\0')
	{
		count += 1;
		if (count > 16)
		{
			fprintf(stderr,"Key file has too many values. Expect to have only 64-bit key, count = %d\n", count);
			mykey.isValid = 0;
			break;
		}
		if(*mychar >= '0' && *mychar <= '9')
		{
			*temp = (unsigned char)*mychar - 48; //representation of char 0-9 to hex of 0-9
		}
		else if(*mychar >= 'a' && *mychar <= 'f')
		{
			*temp = (unsigned char)*mychar - 87; //representation of char a-f to hex of 10-15
		}
		else
		{
			fprintf(stderr,"Detect invalid key '%c'. Key should be represented in hexstring\n", *mychar);
			mykey.isValid = 0;
			break;
		}
		//each character will represent 4 bits in key string
		if(flip) //shift to left by 4 bits
		{
			*temp = *temp << 4;
			mykey.key[index] |= *temp;
			flip = 0;
		}
		else
		{
			mykey.key[index] |= *temp;
			flip = 1;
		}

		if(count % 2 == 0)
		{
			index += 1;
		}
		mychar += 1; //next char in key

	}
	if (count != 16 && mykey.isValid != 0)
	{
		fprintf(stderr,"Key file has too few values. Expect to have only 64-bit key\n");
		mykey.isValid = 0;
	}

	free(temp);
	return mykey;
}

//Supplemental function to convert an inteter to char pointer
char *itoa (int n)
{
	char *myreturn = (char *) malloc(32);
	char *mytemp = myreturn;
	char *temp = (char *) malloc(32);
	char *tempfirst = temp;

	while (n > 0)
	{
		*(temp) =  (n % 10) + '0';
		temp += 1;
		n = n / 10;
	}

	temp -= 1;
	while(tempfirst <= temp)
	{
		*mytemp = *(temp);
		mytemp += 1;
		temp -= 1;
	}
	*(mytemp) = '\0';
	free (tempfirst);

	return myreturn; //Will be freed upon function calling it
}

/*
 * Construct 16 48-bit subkeys from PC1, PC2, and key
 * @param: struct tablefile containig table PC1 and PC2, and key
 * @output: 2-D array pointing to 16 subkeys
 */
struct subkeys keySchedule(struct tablefile mytable, struct key mykey)
{
	struct subkeys mysubkeys;
	mysubkeys.isValid = 1;
	//Sanity check tablefile and key
	if (mytable.isValid == 0 || mykey.isValid == 0)
	{
		fprintf(stderr, "Detect an invalid tablefile or key while trying to perform key scheduling\n");
		mysubkeys.isValid = 0;
		return mysubkeys;
	}
	unsigned char c0[4]; //only use 28 bits
	unsigned char d0[4]; //only use 28 bits
	for(int k=0; k < 4; k++)
	{
		c0[k] = '\0';
		d0[k] = '\0'; //initialize c0 and d0 array
	}

	int c0_index = 0;
	int c0_bitTH = 7; //Don't use the last 4 bit of the last byte
	int d0_bitTH = 7; //Don't use the last 4 bit of the last byte
	int d0_index = 0;
	int byteTH = 0;
	int bitTH = 0;
	unsigned char holder = '\0';
	//Construct C0 and D0
	for(int k=0; k < 56; k++)
	{
		byteTH = (mytable.PC1[k] - 1) / 8; //byteTH should never equal or greater than 8 since PC1 has value from 1-64 inclusive
		bitTH = 7 - ((mytable.PC1[k] - 1) % 8);
		holder = mykey.key[byteTH] & ((0x01) << bitTH); //check if this bit is set
		if (holder != 0)
		{
			if( k <= 27) //The first 28 bit in PC1 is C0
			{
				//add this value to c0
				c0[c0_index] |= ((0x01) << c0_bitTH); //If it's set, set the bit in the correct position
				c0_bitTH -= 1; //go to next bit in c0
			}
			else //The last 28 bit in PC1 is D0
			{
				//add this value to d0
				d0[d0_index] |= ((0x01) << d0_bitTH); //If it's set, set the bit in the correct position
				d0_bitTH -= 1; //go to next bit in d0
			}

		}
		else
		{
			//Don't set the bit and continue to next bit in c0 or d0
			if (k <= 27)
			{
				c0_bitTH -= 1;
			}
			else
			{
				d0_bitTH -= 1;
			}
		}

		if (c0_bitTH < 0) //Finished a byte, go to next byte in c0
		{
			c0_index += 1;
			c0_bitTH = 7;
		}

		if (d0_bitTH < 0) //Finished a byte, go to next byte in d0
		{
			d0_index += 1;
			d0_bitTH = 7;
		}
	}
	if (c0_index != 3 || d0_index != 3)
	{
		printf("wrong expected index, c0 index = %d, d0 index = %d\n",c0_index, d0_index);
	}
	//We should have C0 and D0 by now. Convert it to unsigned 32 bit number to easily perform bitwise left shift
	uint32_t c0num = (uint32_t)c0[0] << 24 | (uint32_t)c0[1] << 16 | (uint32_t)c0[2] << 8 | (uint32_t)c0[3];
	uint32_t d0num = (uint32_t)d0[0] << 24 | (uint32_t)d0[1] << 16 | (uint32_t)d0[2] << 8 | (uint32_t)d0[3];

	fprintf (stderr, "(C0,D0)=%07x%07x\n",c0num>>4, d0num>>4);

	//Start to generate Ci, Di, and ki
	for (int k = 0; k < 16; k++)
	{
		int v = mytable.V[k];
		while (v > 0)
		{
			//check to see if the first bit is set
			if ((c0num & ((0x01) << 31)) != 0)
			{
				//shift it and set the last bit to 1 for circular shifting
				c0num <<= 1;
				c0num |= ((0x01)<< 4);
			}
			else
			{
				c0num <<= 1;
			}
			//check to see if the first bit is set
			if ((d0num & ((0x01) << 31)) != 0)
			{
				//shift it and set the last bit to 1 for circular shifting
				d0num <<= 1;
				d0num |= ((0x01) << 4);
			}
			else
			{
				d0num <<= 1;
			}
			v -= 1;
		}
		//We should have Ci and Di by now, print it out and continue with PC2
		char *number = itoa(k+1);
		char *cPrint = malloc(5);
		strcpy(cPrint, "C");
		strcat(cPrint, number);
		char *dPrint = malloc(5);
		strcpy(dPrint, "D");
		strcat(dPrint, number);
		fprintf(stderr, "(%s,%s)=%07x%07x\n",cPrint,dPrint,c0num>>4,d0num>>4);
		free(cPrint);
		free(dPrint);

		/*
		 * compute ki from PC2
		 * go through table PC2, and copy bit by bit to ki
		 * ki has 48 bits and it's the same of number of elements in PC2
		 */
		int subkeybyteTH = 0;
		int subkeybitTH = 7;
		for(int j = 0; j < 6; j++)
		{
			mysubkeys.subkeys[k][j] = '\0'; //initialize each subkey
		}

		for(int j = 0; j < 48; j++)
		{
			int valPC2 = mytable.PC2[j];
			if (valPC2 <= 28) //Use Dnum
			{
				if((c0num & ((0x01) << (28-valPC2 + 4))) != 0)
				{
					mysubkeys.subkeys[k][subkeybyteTH] |= ((0x01) << subkeybitTH);
				}
			}
			else //use Cnum
			{
				if((d0num & ((0x01) << (56-valPC2 + 4))) != 0)
				{
					mysubkeys.subkeys[k][subkeybyteTH] |= ((0x01) << subkeybitTH);
				}
			}
			subkeybitTH -= 1;
			if (subkeybitTH < 0)
			{
				subkeybitTH = 7;
				subkeybyteTH += 1;
			}
		}
		//We should have ki by now, print it and continue to next round
		char *keyPrint = malloc(5);
		strcpy(keyPrint, "k");
		strcat(keyPrint, number);
		fprintf(stderr, "%s=", keyPrint);
		free(keyPrint);
		free(number); //from itoa function
		for(int j = 0; j < 6; j++)
		{
			fprintf(stderr, "%02x", mysubkeys.subkeys[k][j]);
		}
		fprintf(stderr,"\n");
	}

	return mysubkeys;
}

/*
 * sub-function for encrypt to perform IP table
 * @param: 64-bit input char, and tablefile
 * @output: uint32_t l0 and r0
 */
struct LR performIP(struct tablefile mytable, char *input)
{
	struct LR myLR;
	uint32_t l0 = 0;
	int l0_index = 31;
	uint32_t r0 = 0;
	int r0_index = 31;
	//Go through IP to get l0 and r0
	int byteTH = 0;
	int bitTH = 0;
	for(int k = 0; k < 64; k++)
	{
		byteTH = (mytable.IP[k] - 1) / 8; //byteTH should never equal or greater than 8 since PC1 has value from 1-64 inclusive
		bitTH = 7 - ((mytable.IP[k] - 1) % 8);
		if(l0_index >= 0) //copy 32 bit to l0
		{
			if ((input[byteTH] & ((0x01) << bitTH)) != 0)
			{
				l0 |= ((0x01) << l0_index);
			}
			l0_index -= 1;
		}
		else //copy 32 bit to r0
		{
			if ((input[byteTH] & ((0x01) << bitTH)) != 0)
			{
				r0 |= ((0x01) << r0_index);
			}
			r0_index -= 1;
		}
	}
	myLR.l0 = l0;
	myLR.r0 = r0;

	return myLR;
}

/*
 * sub-function to perform expansion
 * @param r0
 * @output: 48-bit expansion of 32-bit r0
 */
struct Exp performExp(struct tablefile mytable, uint32_t r0)
{
	//Let R0 go through E function
	struct Exp my_r0_expand; //48-bit output
	unsigned char r0_expand[6];
	for(int k = 0; k < 6; k++)
	{
		r0_expand[k] = '\0'; //initialize the r0_expand array
	}
	//Go through E table
	int e_output_bitTH = 7;
	int e_output_byteTH = 0;
	for(int k = 0; k < 48; k++)
	{
		int e_index = mytable.E[k];
		//Check to see this bit is set, if so, set it in output
		if ((r0 & ((0x01) << (32-e_index))) != 0)
		{
			r0_expand[e_output_byteTH] |= ((0x01) << e_output_bitTH);
		}
		e_output_bitTH -= 1;
		if (e_output_bitTH < 0)
		{
			e_output_bitTH = 7;
			e_output_byteTH += 1;
		}
	}
	for(int k = 0; k < 6; k++)
	{
		my_r0_expand.r0_expand[k] = r0_expand[k];
	}
	return my_r0_expand;
}

/*
 * sub-function to perform S-boxes substitution
 * @param: unsigned char 48 bits, and tablefile
 * @output: 32-bit uint32_t
 */
uint32_t performSbox(struct tablefile mytable,unsigned char xor_ki[])
{
	uint32_t Sbox_result = 0;
	int sBox_bit_index = 31;

	//Go through all 48 bits of xor_ki array, and will process 6 bits at a time
	unsigned char holder = '\0';
	int count = 0;
	int xor_byteTH = 0;
	int xor_bitTH = 7;
	int holder_bitTH = 7;
	int bit_set_holder[6]; //use to keep track of which bit is set in holder
	for(int k = 0; k < 6; k++)
	{
		bit_set_holder[k] = 0; //initialize bit_set_holder
	}
	int sBox_index = 0;
	for(int k = 0; k < 48; k++)
	{
		//copy bit to holder
		if((xor_ki[xor_byteTH] & ((0x01) << xor_bitTH)) != 0)
		{
			holder |= ((0x01) << holder_bitTH);
			bit_set_holder[7-holder_bitTH] = 1;
		}
		holder_bitTH -= 1;
		xor_bitTH -= 1;
		if(xor_bitTH < 0)
		{
			xor_bitTH = 7;
			xor_byteTH += 1;
		}
		count += 1;
		if (count == 6)
		{
			//Calculate r and c in S box
			int r = 2*bit_set_holder[0] + bit_set_holder[5];
			int c = 8*bit_set_holder[1] + 4*bit_set_holder[2] + 2*bit_set_holder[3] + bit_set_holder[4];
			if (r > 3 || r < 0 || c > 15 || c < 0)
			{
				fprintf(stderr, "row or col in Sbox calculation is out of bound!\n");
				return 0;
			}
			int mySbox = mytable.S[sBox_index][(16*r)+c];
			unsigned char fourBits = mySbox+'0';
			fourBits -= 48; //convert char 7 into value of hex 7

			//copy this 4 bits into uint32_t
			int bitTH = 3;
			while(bitTH >= 0)
			{
				if((fourBits & ((0x01) << bitTH)) != 0)
				{
					Sbox_result |= ((0x01) << sBox_bit_index);
				}
				sBox_bit_index -= 1;
				bitTH -= 1;
			}
			sBox_index += 1;
			//reset value for next 6 bits
			holder = '\0';
			holder_bitTH = 7;
			count = 0;
			for(int k = 0; k < 6; k++)
			{
				bit_set_holder[k] = 0; //initialize bit_set_holder
			}
		}
	}

	return Sbox_result;
}

/*
 * sub-function to perform permutation for table P
 * @param: tablefile, and uint32_t number from the result of SBox
 * @output: 32-bit number permutated from table P
 */
uint32_t performP(struct tablefile mytable, uint32_t result_sBox)
{
	uint32_t resultP = 0;
	int resultIndex = 31;
	//Go through all 32 values of table P
	for(int k = 0; k < 32; k++)
	{
		//Check if the bit at P[k] in result_sBox is set. If so, set it in the resultP number
		if((result_sBox & ((0x01) << (32-mytable.P[k]))) != 0)
		{
			resultP |= ((0x01) << resultIndex);
		}
		resultIndex -= 1;
	}
	return resultP;
}

/*
 * sub-function to perform IPInv
 * @param: tablefile, uint31_t R16, uint32_t L16
 * @output: 64-bit ciphertext
 */
unsigned char *performIPInv(struct tablefile mytable, uint32_t r16, uint32_t l16)
{
	unsigned char *cipher = malloc(8); //Will be freed upon the calling function
	for(int k = 0; k < 8; k++)
	{
		cipher[k] = '\0'; //initialize cipher array
	}
	int bitTH = 7;
	int byteTH = 0;
	//go through each element of IPInv table
	for(int k = 0; k < 64; k++)
	{
		int index = mytable.IPInv[k];
		if(index <= 32)
		{
			if((r16 & ((0x01) << (32-index))) != 0)
			{
				cipher[byteTH] |= ((0x01) << bitTH);
			}
		}
		else
		{
			if((l16 & ((0x01) << (64-index))) != 0)
			{
				cipher[byteTH] |= ((0x01) << bitTH);
			}
		}
		bitTH -= 1;
		if(bitTH < 0)
		{
			bitTH = 7;
			byteTH += 1;
		}
	}
	return cipher;
}

/*
 * encrypt block input text of 8 byte at a time using DES table driven method
 * @param: struct tablefile containing all tables, subkeys struct containing all necessary subkeys,input text
 * @output: print encrypted cipher text to stdout
 */
void encrypt(struct tablefile mytable, struct subkeys mysubkeys, FILE *fp)
{
	//char *myinput = malloc(1); //read char by char to avoid the 0a character
	//char *block = malloc(8);
	int byte = 0;
	//int count_input = 0;
	struct LR myLR;
	int isFirst8Bytes = 0;
	//int countDebug = 0;
	char *myinput = malloc(8);
	while((byte = fread(myinput,1,8,fp)) >= 8)
	{
		//Get L0 and R0
		myLR = performIP(mytable, myinput);
		if(!isFirst8Bytes)
		{
			fprintf(stderr, "(L0,R0)=%08x%08x\n",myLR.l0,myLR.r0);
		}
		/*
		 * Now go into a loop of 16 to go though all 16 sub-encryptions
		 */
		uint32_t li = myLR.l0;
		uint32_t ri = myLR.r0;
		/*Perform encryption 16 times per DES requirement */
		for (int i = 0; i < 16; i++)
		{
			//Let R0 go through E function
			struct Exp r0_expand = performExp(mytable, ri);

			//We should have E(r0) by now, let's XOR with ki
			unsigned char xor_ki[6]; // = r0_expand.r0_expand ^ mysubkeys.subkeys[i];
			for(int k = 0; k < 6; k++)
			{
				xor_ki[k] = r0_expand.r0_expand[k] ^ mysubkeys.subkeys[i][k];
				//fprintf(stderr, "my xor ki bit = %x, r0-expand = %x, subkeys = %x\n", xor_ki[k], r0_expand.r0_expand[k], mysubkeys.subkeys[i][k]);
			}
			//Now we need to go through S-boxes
			uint32_t result_sBox = performSbox(mytable, xor_ki);

			//Now we need to go through P table
			uint32_t resultP = performP(mytable, result_sBox);

			//Now we XOR with L of i-1, and this value will be come R of i
			uint32_t temp_li = li;

			//update Li and Ri for next round
			li = ri; //li is the previous ri
			ri = resultP ^ temp_li; //update ri

			//Print info to stderr, only for the first 8-byte block
			if(!isFirst8Bytes)
			{
				char *number = itoa(i+1);
				char *myPrintL0 = malloc(5);
				strcpy(myPrintL0, "L");
				strcat(myPrintL0, number);
				char *myPrintR0 = malloc(5);
				strcpy(myPrintR0, "R");
				strcat(myPrintR0, number);
				fprintf(stderr, "(%s,%s)=%08x%08x\n",myPrintL0,myPrintR0,li,ri);
				free(myPrintL0);
				free(myPrintR0);
				free(number); //result from itoa() function
			}
		}
		//Now we perform IPinv on "irregular swap"
		unsigned char *cipher = performIPInv(mytable, ri, li);
		fwrite(cipher,1,8,stdout);

		free(cipher); //free from performIPInv() function
		isFirst8Bytes = 1;

	}

	if (byte > 0) //take care of last case where there is not enough 8 bytes
	{
		//padding input with zeros
		for(int k = byte; k < 8; k++)
		{
			myinput[k] = '\0';
		}
		//Get L0 and R0
		myLR = performIP(mytable, myinput);
		if(!isFirst8Bytes)
		{
			fprintf(stderr, "(L0,R0)=%08x%08x\n",myLR.l0,myLR.r0);
		}
		/*
		 * Now go into a loop of 16 to go though all 16 sub-encryptions
		 */
		uint32_t li = myLR.l0;
		uint32_t ri = myLR.r0;
		/*Perform encryption 16 times per DES requirement */
		for (int i = 0; i < 16; i++)
		{
			//Let R0 go through E function
			struct Exp r0_expand = performExp(mytable, ri);

			//We should have E(r0) by now, let's XOR with ki
			unsigned char xor_ki[6]; // = r0_expand.r0_expand ^ mysubkeys.subkeys[i];
			for(int k = 0; k < 6; k++)
			{
				xor_ki[k] = r0_expand.r0_expand[k] ^ mysubkeys.subkeys[i][k];
			}
			//Now we need to go through S-boxes
			uint32_t result_sBox = performSbox(mytable, xor_ki);

			//Now we need to go through P table
			uint32_t resultP = performP(mytable, result_sBox);

			//Now we XOR with L of i-1, and this value will be come R of i
			uint32_t temp_li = li;

			//update Li and Ri for next round
			li = ri; //li is the previous ri
			ri = resultP ^ temp_li; //update ri

			//Print info to stderr, only for the first 8-byte block
			if(!isFirst8Bytes)
			{
				char *number = itoa(i+1);
				char *myPrintL0 = malloc(5);
				strcpy(myPrintL0, "L");
				strcat(myPrintL0, number);
				char *myPrintR0 = malloc(5);
				strcpy(myPrintR0, "R");
				strcat(myPrintR0, number);
				fprintf(stderr, "(%s,%s)=%08x%08x\n",myPrintL0,myPrintR0,li,ri);
				free(myPrintL0);
				free(myPrintR0);
				free(number); //result from itoa() function
			}
		}
		//Now we perform IPinv on "irregular swap"
		unsigned char *cipher = performIPInv(mytable, ri, li);
		fwrite(cipher,1,8,stdout);

		free(cipher); //free from performIPInv() function
		isFirst8Bytes = 1;
	}
	free(myinput);
}

/*
 * supplemental encryption function to support encrypt3 using EDE 3DES
 * @param: struct tablefile containing all tables, subkeys struct containing all necessary char array of 8-byte input, key part
 * @output: char* pointer to the encrypted ciphertext
 */
unsigned char *sup_encrypt(struct tablefile mytable, struct subkeys mysubkeys, char *myinput, int key_part)
{
	struct LR myLR;

	//Get L0 and R0
	myLR = performIP(mytable, myinput);
	if(key_part == 0)
	{
		fprintf(stderr, "(L0,R0)=%08x%08x\n",myLR.l0,myLR.r0);
	}
	/*
	 * Now go into a loop of 16 to go though all 16 sub-encryptions
	 */
	uint32_t li = myLR.l0;
	uint32_t ri = myLR.r0;
	/*Perform encryption 16 times per DES requirement */
	for (int i = 0; i < 16; i++)
	{
		//Let R0 go through E function
		struct Exp r0_expand = performExp(mytable, ri);

		//We should have E(r0) by now, let's XOR with ki
		unsigned char xor_ki[6]; // = r0_expand.r0_expand ^ mysubkeys.subkeys[i];
		for(int k = 0; k < 6; k++)
		{
			xor_ki[k] = r0_expand.r0_expand[k] ^ mysubkeys.subkeys[i][k];
		}
		//Now we need to go through S-boxes
		uint32_t result_sBox = performSbox(mytable, xor_ki);

		//Now we need to go through P table
		uint32_t resultP = performP(mytable, result_sBox);

		//Now we XOR with L of i-1, and this value will be come R of i
		uint32_t temp_li = li;

		//update Li and Ri for next round
		li = ri; //li is the previous ri
		ri = resultP ^ temp_li; //update ri

		//Print info to stderr, only for the first 8-byte block
		if(key_part == 0)
		{
			char *number = itoa(i+1);
			char *myPrintL0 = malloc(5);
			strcpy(myPrintL0, "L");
			strcat(myPrintL0, number);
			char *myPrintR0 = malloc(5);
			strcpy(myPrintR0, "R");
			strcat(myPrintR0, number);
			fprintf(stderr, "(%s,%s)=%08x%08x\n",myPrintL0,myPrintR0,li,ri);
			free(myPrintL0);
			free(myPrintR0);
			free(number); //result from itoa() function
		}
	}
	//Now we perform IPinv on "irregular swap"
	unsigned char *cipher = performIPInv(mytable, ri, li);
	return cipher; //Will be freed upon calling function
}
