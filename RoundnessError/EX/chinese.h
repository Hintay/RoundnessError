﻿/*!
* @file	EX\chinese.h.
*
* @brief	通用中文字模显示头文件
*
* @author	Hintay
* @date		2018/4/21
*/

#ifndef __CHINESE_H
#define __CHINESE_H	 
#include "sys.h"

void show_chinese_char(u16 x, u16 y, u8 index, const u8(*font_array)[16], u8 font_size);
void show_chinese_string(u16 x, u16 y, const u8 index_list[], const u8(*font_array)[16], u8 font_size);

#endif
