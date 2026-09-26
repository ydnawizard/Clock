#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <stdint.h>
#include <string.h>
#include "fonts.h"
#include "max_driver.h"
#include <stdlib.h>

//Type animation *ani*
//Holds all relevant info for any animation
//Core type of animation functions
//Standard input for all animation functions
typedef struct ani
{
	uint8_t ani_type; //Animation type
	uint8_t start_chip; //Display chip which effect will start on
	uint8_t end_chip; //Display chip which effect will
	uint8_t threshold; //Distance from start to end chipwise
	uint8_t speed; //Animation speed
	uint8_t duration; //Animation duration
	uint8_t str_len; //String Length
	uint8_t font; //String font
	char * str; //String
	uint8_t ** font_str; //String after font conversion
} ani;

//Type animation sequence *ani_seq*
//Simple way of iterating through aniations
//Core animation controller of ulterior display type
//found in display header
typedef struct ani_seq
{
	uint8_t effect_cnt; //Number of effects
	uint8_t effect_ind; //Effect index
	struct ani * proc; //Procession array of effect structs
} ani_seq;

//Helper function
//Takes in ani struct
//Consults relevant font table to translate string to font
//Font tables are constructed isomorphic to ascii
//Allocates storage for each 8 bit int array 
void ani_str_fetch(struct ani * _ani);

//Helper Function

void ani_str_set(struct ani * _ani, char * str);

//Basic animation init time saver 
//Defaults to all panels, max speed, "Hello", Scroll
//Calls str_fetch
void ani_init(struct ani * _ani);

//Basic scroll left to right horizontal animation
void scroll_horizontal(struct ani * _ani);

//Ripples text up and down horizontally
void recursive_ripple(struct ani * _ani);

#endif //ANIMATION_H_
