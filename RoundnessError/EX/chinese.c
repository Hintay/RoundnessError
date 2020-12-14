/*!
* @file	EX\chinese.c.
*
* @brief	通用中文字模显示
*
* @author	Hintay
* @date		2018/4/21
*/

#include "chinese.h"
#include "chfont.h"
#include "lcd.h"
#include <math.h>

/*!
 * @fn	void show_chinese_char(u16 x, u16 y, u8 index, const u8 font_array[][16], u8 font_size)
 *
 * @brief	显示中文字模通用函数
 *
 * @author	Hintay
 * @date	2018/4/16
 *
 * @param	x			  	显示坐标 x.
 * @param	y			  	显示坐标 y.
 * @param	index		  	字体数组中的索引
 * @param	font_array[16]	字体数组名
 * @param	font_size	  	字体大小，应与字模匹配
 */

void show_chinese_char(u16 x, u16 y, u8 index, const u8(*font_array)[16], u8 font_size)
{
	u8 temp, t1, font_byte, font_subarray_count;
	u16 y0 = y;
	const u8 * p;

	// 若要支持大号字体需要使用 u16
	font_byte = ceil(font_size / 8.0) * font_size;		// 字体字节
	font_byte = font_byte / 8 * 8;						// 修正 PCtoLCD 的 BUG
	font_subarray_count = ceil(font_byte / 16.0);		// 字体子数组个数

	p = font_array[index * font_subarray_count];	// 获取指针
	while(font_byte--)
	{
		temp = *p++;
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)LCD_Fast_DrawPoint(x, y, POINT_COLOR);//画实心点
			else LCD_Fast_DrawPoint(x, y, BACK_COLOR);  //画空白点（使用背景色）
			temp <<= 1;
			if ((++y - y0) == font_size)
			{
				y = y0;
				x++;
				break;
			}
		}
	}
}

/*!
 * @fn	void show_chinese_string(u16 x, u16 y, const u8 index_list[], const u8(*font_array)[16], u8 font_size)
 *
 * @brief	根据索引显示中文字符串
 *
 * @author	Hintay
 * @date	2018/4/21
 *
 * @param 		  	x		  	显示坐标 x.
 * @param 		  	y		  	显示坐标 y.
 * @param 		  	index_list	字符串索引
 * @param [in,out]	font_array	字体数组名
 * @param 		  	font_size 	字体大小，应与字模匹配
 */

void show_chinese_string(u16 x, u16 y, const u8 index_list[], const u8(*font_array)[16], u8 font_size)
{
	u8 i;
	for(i = 0; i < index_list[0]; i++)
	{
		show_chinese_char(x + font_size*i, y, index_list[1+i], font_array, font_size);
	}
}
