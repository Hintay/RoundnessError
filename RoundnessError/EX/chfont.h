#ifndef __CHFONT_H
#define __CHFONT_H	 
#include "sys.h"

struct font_data
{
	u8 font_size;
	u8 data_len;
	const u8(*titile_font_data)[16];
};

extern const struct font_data main_title_font;
extern const struct font_data debug_title_font;
extern const u8 regular_font_16[][16];
extern const u8 regular_font_24[][16];
extern const u8 output_font[][16];
extern const u8 block_font[][16];

#endif
