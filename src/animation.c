#include "animation.h"

//Translates input string into characters from specified font
void String_Fetch(char * input_str, uint8_t *** output_str, uint8_t str_len)
{
	(*output_str) = malloc(str_len * sizeof(uint8_t *));
	for(uint8_t i = 0; i < str_len; i++)
	{
		(*output_str)[i] = malloc(8 * sizeof(uint8_t));
		for(uint8_t k = 0; k < 8; k++)
		{
			(*output_str)[i][k] = standard_font_8[input_str[i] - 32][k];
		}
	}
}

void Recursive_Scroll_Horizontal(
		uint8_t ** input_str,
		uint8_t str_len,
		uint8_t start_chip,
		uint8_t end_chip,
		uint8_t speed
		)
{
	uint8_t mask, threshold;
	uint16_t output_index, input_index;
	threshold = (end_chip - start_chip) + 1;
	output_index = 0;
	input_index = 0;
	uint8_t output[threshold][8];
	uint8_t input[str_len][8];
	for(uint64_t i = 0; i < str_len; i++)
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			input[i][j] = input_str[i][j];
		}
	}
	for(uint8_t i = 0; i < threshold; i++)
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			output[i][j] = 0x00;
		}
	}
	for(uint64_t i = 0; i < 8 * (threshold + str_len); i++)
	{
		for(uint8_t j = start_chip; j <= end_chip; j++) //For each chip in specified chip threshold
		{
			for(uint8_t k = 0; k < 8; k++)		//For each row in chip
			{
				if(j == start_chip)		//If current chip is first chip
				{
					//Cycle in the leading characters from each row of input
					//and assign to mask
					mask = (input[input_index/8][k] & 0x80) >> 7;
					//Shift output left 1 and OR with mask
					output[0][k] = (output[0][k] << 1) | mask;
					//Shift input left 1
					input[input_index/8][k] = input[input_index/8][k] << 1;
					//Write to chip
					MAX_Chip_Send_Target(j, k + 1, output[0][k]);
					//Delay (speed)
					//Volatile read from RAM
					for(volatile int i = 0; i < speed; i ++) {}
					if( k == 7)
					{
						input_index = (input_index + 1) % (8 *str_len);
					}
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

void Recursive_Ripple()
{
}
