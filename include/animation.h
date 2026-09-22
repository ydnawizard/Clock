#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <stdint.h>
#include "fonts.h"
#include "max_driver.h"
#include <stdlib.h>

typedef struct animation
{
	int effect_count;
	int * effect_sequence;
	int * duration;
	int font;
	char * string;
} animation;

typedef struct animation_sequence
{
	int sequence_index;
	struct animation * procession;
} animation_sequence;

void string_fetch(
		char * input_str,
		uint8_t *** output_str,
		uint8_t str_len
		);

void animation_create(
		int effect_count,
		int * effect_sequence,
		int * duration,
		int font,
		char * string,
		animation * target
		);

void recursive_scroll_horizontal(
		uint8_t ** input_str,
		uint8_t str_len,
		uint8_t start_chip,
		uint8_t end_chip,
		uint8_t speed
		);


void recursive_ripple(
		);

#endif //ANIMATION_H_
