#include "animation.h"

//Translates input string into characters from specified font
void String_Fetch(char * input_str, uint8_t *** output_str, uint8_t str_len)
{
	for(uint8_t i = 0; i < str_len; i++)
	{
		(*output_str)[i] = calloc(8,str_len * sizeof(uint8_t));
		for(uint8_t k = 0; k < 8; k++)
		{
			(*output_str)[i][k] = standard_font_8[input_str[i] - 32][k];
		}
	}
}


//Concatenates array of font characters into 8 row uint64_t array
void Cat_String_864(uint8_t ** input_str, uint64_t * output_str, uint8_t str_len)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		uint64_t placeholder = 0;
		for(uint8_t j = 0; j < str_len; j++)
		{
			placeholder = (placeholder << 8) | (uint64_t)input_str[j][i];
		}
		output_str[i] = placeholder;
	}
}

//Scrolls string of 64 bit rows from start to end 8 bit matrices
void Recursive_Scroll_Horizontal(
		uint64_t ** input_str, //Concatenated 64 bit 8 row string
		uint8_t str_len,       //Input string length
		uint8_t start_chip,    //Entry point for scroll animation
		uint8_t end_chip,      //Exit point
		uint8_t speed          //Speed for animation
		)
{
	uint8_t mask, threshold, output_index;
	threshold = (end_chip - start_chip) + 1; // Threshold for managing output matrix
	output_index = 0; 			 // Keeps track of place in output matrix
	uint64_t input[8];			 // Place to copy input contents so that original
						 // Pointer remains constant
	uint8_t output[threshold][8];		 // Initialize output destination
	for(int i = 0; i < 8; i++) 		 // Copy input contents
	{
		input[i] = (*input_str)[i];
	}
	for(int i = 0; i < threshold; i++)       // assign empty values to each entry in output
	{
		for(int j = 0; j < 8; j++)
		{
			output[i][j] = 0x00;
		}
	}
	for(uint16_t i = 0; i < 19 * str_len; i++)		//Arbitrary value that gives string enough
								//cycles to move across string
								//I tried mathing this and could not figure
								//it out so I just wound up with 19 through
								//trial and error (18 is not enough)
	{
		for(uint8_t j = start_chip; j <= end_chip; j++) //For each chip in specified chip threshold
		{
			for(uint8_t k = 0; k < 8; k++)		//For each row in chip
			{
				if(j == start_chip)		//If current chip is first chip
				{
					//Cycle in the leading characters from each row of input
					//and assign to mask
					mask = (input[k] & 0x8000000000000000ULL) >> 63;
					//Shift output left 1 and OR with mask
					output[0][k] = (output[0][k] << 1) | mask;
					//Shift input left 1
					input[k] = input[k] << 1;
					//Write to chip
					MAX_Chip_Send_Target(j, k + 1, output[0][k]);
					//Delay (speed)
					//Volatile read from RAM
					for(volatile int i = 0; i < speed * 100; i ++) {}
				}
				else				//Otherwise
				{
					//Cycle in leading character from previous matrix
					//and assign to mask
					mask = (output[output_index-1][k] & 0x80) >> 7;
					//Shift output left and OR with mask
					output[output_index][k] = (output[output_index][k] << 1) | mask;
					//Write to chip
					MAX_Chip_Send_Target(j, k+1 , output[output_index][k]);
					//Delay
					for(volatile int i = 0; i < speed ; i ++) {}
				}
			}
			//increment output index mod threshold
			//Mod ensures output increments in cycles of threshold
			output_index = (output_index + 1) % threshold;
		}
	}
}

void Recursive_Scroll_Horizontal_New(
		uint64_t ** input_str, //Concatenated 64 bit 8 row string
		uint8_t str_len,       //Input string length
		uint8_t start_chip,    //Entry point for scroll animation
		uint8_t end_chip,      //Exit point
		uint8_t speed          //Speed for animation
		)
{
	uint8_t mask, threshold, output_index;
	threshold = (end_chip - start_chip) + 1; // Threshold for managing output matrix
	output_index = 0; 			 // Keeps track of place in output matrix
	uint64_t input[8];			 // Place to copy input contents so that original
						 // Pointer remains constant
	uint64_t output[8];		 // Initialize output destination
	for(int i = 0; i < 8; i++) 		 // Copy input contents
	{
		input[i] = (*input_str)[i];
	}
	for(int i = 0; i < 8; i++)       // assign empty values to each entry in output
	{
		output[i] = 0x00;
	}
	for(uint16_t i = 0; i < 19 * str_len; i++)		//Arbitrary value that gives string enough
	{
		for(uint8_t k = 0; k < 8; k++)		//For each row in chip
		{
			mask = (input[k] & 0x8000000000000000ULL) >> 63;
			input[k] = input[k] << 1;
			output[k] = (output[k] << 1) | mask;
			
		}
		for(uint8_t j = start_chip; j <= threshold; j++)
		{
			for(uint8_t k = 0; k < 8; k++)
			{
				MAX_Chip_Send_Target(j, k+1 , output[k] >> (8 * j) );
			}
		}
		//for(volatile int i = 0; i < speed ; i ++) {}
	}
}
void Recursive_Ripple()
{
}
