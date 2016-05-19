/*
 * decrypt.c
 *
 *  Created on: Mar 20, 2016
 *      Author: ntran
 */

#include "decrypt.h"

/*
 * decrypt input using AES-128
 * @param: pointer to input file, struct tablefile containing tables, and struct mykey containing key
 * @output: decrypted plaintext and print to stdout each round processes
 */
void decrypt(struct tablefile mytable, struct mykey key, FILE *fp)
{
	//define constant in keyexpand (Nk, Nb, Nr)

	//construct subkeys using keyexpansion function
	struct mysubkeys subkeys = keyexpansion(key, mytable, 0);

	//Read input
	struct myinput input = readInput(fp);
	if(input.isValid)
	{
		uint8_t state[16];
		//copy input into state
		for(int k=0; k < 16; k++)
		{
			state[k] = input.input[k];
		}
		//print round 0 output
		printOutput(0, "iinput", state, 0);
		struct myholder roundkey = getRoundKey(subkeys, Nr*Nb, (Nr+1)*Nb-1);
		printOutput(0, "ik_sch", roundkey.holder, 0);
		//Perform AddRoundKey operation - state XOR roundKey
		for(int k=0; k < 16; k++)
		{
			state[k] = state[k] ^ roundkey.holder[k];
		}
		printOutput(1, "istart", state, 0);

		/*
		 * Start for loop to go through all 10 rounds
		 */
		for(int round=Nr-1; round > 0; round--)
		{
			//Perform inverse shift row and copy return data to state
			struct myholder myShiftRows = shiftrows(state, 0);
			memcpy(state, myShiftRows.holder, 16);
			printOutput(Nr-round, "is_row", state, 0);

			//Perform inverse subByte and copy return data to state
			struct myholder mySubByte = subByte(state, mytable, 0);
			memcpy(state, mySubByte.holder, 16);
			printOutput(Nr-round, "is_box", state, 0);

			//Perform add round key operation on state with new key rounds
			struct myholder roundKey2 = getRoundKey(subkeys, round*Nb, (round+1)*Nb-1);
			printOutput(Nr-round, "ik_sch", roundKey2.holder, 0);
			//Perform AddRoundKey operation - state XOR roundKey
			for(int k=0; k < 16; k++)
			{
				state[k] = state[k] ^ roundKey2.holder[k];
			}
			printOutput(Nr-round, "ik_add", state, 0);

			//Perform mix cols and copy return data to state
			struct myholder myMixCols = mixcols(state, mytable, 0);
			memcpy(state, myMixCols.holder, 16);
			printOutput(Nr-round+1, "istart", state, 0);

		}

		/*
		 * Perform the last round to obtain output
		 */
		//Perform inverse shift row and copy return data to state
		struct myholder myShiftRows = shiftrows(state, 0);
		memcpy(state, myShiftRows.holder, 16);
		printOutput(10, "is_row", state, 0);

		//Perform inverse subByte and copy return data to state
		struct myholder mySubByte = subByte(state, mytable, 0);
		memcpy(state, mySubByte.holder, 16);
		printOutput(10, "is_box", state, 0);

		//Don't do mix cols for last operation

		//Perform add round key operation on state with new key rounds
		struct myholder roundKey2 = getRoundKey(subkeys, 0, Nb-1);
		printOutput(10, "ik_sch", roundKey2.holder, 0);
		//Perform AddRoundKey operation - state XOR roundKey
		for(int k=0; k < 16; k++)
		{
			state[k] = state[k] ^ roundKey2.holder[k];
		}
		printOutput(10, "ioutput", state, 0);

	}
}
