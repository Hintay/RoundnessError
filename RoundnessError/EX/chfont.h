/*!
* @file	EX\chfont.c.
*
* @brief	������ģ����ͷ�ļ�
*/

#ifndef __CHFONT_H
#define __CHFONT_H
#include "sys.h"
#include "common.h"

/*!
 * @struct	font_data
 *
 * @brief	������������ݣ����ڱ������ֵȹ̶���������
 *
 * @author	Hintay
 * @date	2018/4/21
 */

struct font_data
{
	u8 font_size;
	u8 data_len;
	const u8(*titile_font_data)[16];
};

extern const struct font_data main_title_font;
#if TEST_MODE
extern const struct font_data debug_title_font;
extern const u8 regular_font_16[][16];
extern const u8 regular_font_24[][16];
#endif
extern const u8 contest_font[][16];
extern const u8 block_font[][16];

#endif
