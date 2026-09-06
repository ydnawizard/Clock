#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <stdint.h>
#include "fonts.h"
#include "max_driver.h"
#include <stdlib.h>

typedef struct animation
{
	int phase_count;
	int * effect_sequence;
	char ** string_sequence;
	int ** modifiers;
} animation;

void String_Fetch(
		char * input_str,
		uint8_t *** output_str,
		uint8_t str_len
		);
void Cat_String_864(
		uint8_t ** input_str,
		uint64_t * output_str,
		uint8_t str_len
		);

void Recursive_Scroll_Horizontal(
		uint64_t ** input_str,
		uint8_t str_len,
		uint8_t start_chip,
		uint8_t end_chip,
		uint8_t speed
		);

void Recursive_Scroll_Horizontal_New(
		uint64_t ** input_str,
		uint8_t str_len,
		uint8_t start_chip,
		uint8_t end_chip,
		uint8_t speed
		);



void Recursive_Ripple(
		);

#endif //ANIMATION_H_
