#include "animation.h"

//Translates input string into characters from specified font
//Allocates space for string
void ani_str_fetch(struct ani * _ani)
{
	_ani->font_str = malloc(_ani->str_len * sizeof(uint8_t *));
	for(uint8_t i =  0; i < _ani->str_len; i++)
	{
		_ani->font_str[i] = malloc(8 * sizeof(uint8_t));
		for(uint8_t k = 0; k < 8; k++)
		{
			_ani->font_str[i][k] = standard_font_8[_ani->str[i] - 32][k];
		}
	}
}

void ani_init(struct ani * _ani)
{
	_ani->ani_type = 0;
	_ani->start_chip = 0;
	_ani->end_chip = 3;
	_ani->threshold = (_ani->end_chip - _ani->start_chip) + 1;
	_ani->speed = 1;
	_ani->duration = 0;
	_ani->font = 0;
	_ani->str_len = 5;
	_ani->str = malloc(5 * sizeof(char));
	_ani->str = "Hello";
	ani_str_fetch(_ani);
}

void ani_str_set(struct ani * _ani, char * str)
{
	_ani->font_str = realloc(_ani->font_str, 0);
	_ani->str_len = strlen(str);
	_ani->str = str;
	ani_str_fetch(_ani);
}

void scroll_horizontal(struct ani * _ani)
{
	//Init
	uint8_t mask,
		threshold,
		output_index,
		input_index = 0;
	uint8_t output[_ani->threshold][8];
	uint8_t input[_ani->str_len][8];
	//Copy font string into input for mut
	for(uint64_t i = 0; i < _ani->str_len; i++)
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			input[i][j] = _ani->font_str[i][j];
		}
	}
	//Set ouput to 0x00
	for(uint8_t i = 0; i < _ani->threshold; i++)
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			output[i][j] = 0x00;
		}
	}
	//For one entire ani cycle
	// One cycle = First char appears to last letter dissapears
	for(uint64_t i = 0;
			i < 8 * (_ani->threshold + _ani->str_len);
			i++)
	{
		//For each chip in specified threshold
		for(uint8_t j = _ani->start_chip;
				j <= _ani->end_chip;
				j++)
		{
			//For each row in chip
			for(uint8_t k = 0; k < 8; k++)
			{
				//If current Chip is first chip
				if(j == _ani->start_chip)
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
					//If last row, update input index
					if( k == 7)
					{
						input_index = (input_index + 1) % (8 *_ani->str_len);
					}
					//Delay (speed)
					//Volatile read from RAM
					for(volatile int i = 0; i < _ani->speed; i ++) {}
				}
				//Otherwise
				else
				{
					//Cycle in leading character from previous matrix
					//and assign to mask
					mask = (output[output_index-1][k] & 0x80) >> 7;
					//Shift output left and OR with mask
					output[output_index][k] = (output[output_index][k] << 1) | mask;
					//Write to chip
					MAX_Chip_Send_Target(j, k+1 , output[output_index][k]);
					//Delay
					for(volatile int i = 0; i < _ani->speed ; i ++) {}
				}
			}
			//increment output index mod threshold
			//Mod ensures output increments in cycles of threshold
			output_index = (output_index + 1) % _ani->threshold;
		}
	}
}
void recursive_ripple(struct ani * _ani)
{
}
