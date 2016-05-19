/*
 * decrypt.c
 *
 *  Created on: Mar 2, 2016
 *      Author: ntran
 */

#include "decrypt.h"

/*
 * decrypt block input text of 8 byte at a time using DES table driven method
 * @param: struct tablefile containing all tables, subkeys struct containing all necessary subkeys,input text
 * @output: print original plaintext to stdout
 */
void decrypt(struct tablefile mytable, struct subkeys mysubkeys, FILE *fp)
{
	int byte = 0;
	struct LR myLR;
	int isFirst8Bytes = 0;
	char *myinput = malloc(8);
	//Read in 8 byte at a time
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
				xor_ki[k] = r0_expand.r0_expand[k] ^ mysubkeys.subkeys[15-i][k]; //In decrypt, we go from bottom to top; therefore, reverse the subkeys order
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
 * supplemental decryption function to support decrypt3 using EDE 3DES
 * @param: struct tablefile containing all tables, subkeys struct containing all necessary char array of 8-byte input, key part
 * @output: char* pointer to the encrypted ciphertext
 */
unsigned char *sup_decrypt(struct tablefile mytable, struct subkeys mysubkeys, char *myinput, int key_part)
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
			xor_ki[k] = r0_expand.r0_expand[k] ^ mysubkeys.subkeys[15-i][k]; //In decrypt, we go from bottom to top; therefore, reverse the subkeys order
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
