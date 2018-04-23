#pragma once
#include "sys.h"
#include "chinese.h"
#include "chfont.h"
#include "lcd.h"
#include "key.h"
#include "timer.h"
#include "24cxx.h"
#include "math.h"
#include "usart.h"

/// 主界面 ///
void show_page_adc(void);
void show_page_counter(void);
void show_page_roundness_test(void);
void show_page_debug(void);
void show_page_settings(void);

/*!
 * @struct	pages_tab
 *
 * @brief	页数显示的函数定义
 *
 * @author	Hintay
 * @date	2018/4/16
 */

struct pages_tab
{
	void* func;					// 页显示函数指针
	const u8 * name;			// 页面显示在界面中的名称(查找串)
	bool is_chinese;			// 是否显示中文
	//const u8 chinese_index;		// 中文显示索引
};

struct pages_info
{
	bool backable;			// 是否可返回
	u16 background_color;	// 背景色

	/*! @brief	标题设定 */
	u16 title_x;
	u16 title_y;
	u16 title_color;				// 标题颜色
	const struct font_data *title_font;

	/*! @brief	选项设定 */
	u16 option_x;
	u16 option_y;
	u16 option_color;				// 选项颜色
	u16 option_highlight_color;		// 选项搞两个颜色
	u16 option_height;				// 选项高度
	u16 option_width;				// 选项宽度
	u16 option_highlight_padding_x;	// 高亮选项四周的边距
	u16 option_highlight_padding_y;	// 高亮选项四周的边距
	u16 option_spacing;				// 选项之间的间距
	const struct pages_tab *options;
	u8 option_length;
};

/*! @brief	保存中文菜单的索引，最多 9 个字 */
const u8 chinese_menu_index[][10] =
{
	{ 4, 0, 1, 2, 3 },	/* 4字, 圆度测量 */
	{ 4, 4, 5, 6, 7 },	/* 4字, 参数设置 */
	{ 4, 8, 9, 10, 11 }	/* 4字, 调试菜单 */
};

const u8 output_index[][10] =
{
	{ 5, 0, 1, 2, 3, 4 },		/* 5字, 参数设定： */
	{ 6, 5, 6, 7, 2, 3, 4 },	/* 6字, 标准圆设定： */
	{ 5, 8, 9, 10, 11, 4 },		/* 5字, 电机转速： */
	{ 5, 12, 13, 14, 15, 4 },	/* 5字, 测量角度： */
	{ 5, 12, 13, 16, 17, 4 },	/* 5字, 测量半径： */
};

const struct pages_tab pages[] = {
	{ (void*)show_page_debug,			NULL,		true },
	{ (void*)show_page_settings,		NULL,		true },
	{ (void*)show_page_debug,			NULL,		true },
};

const struct pages_tab debug_pages[] = {
	{ (void*)show_page_adc,				"Shifted Linear Test",	false},
	{ (void*)show_page_counter,			"Counter Test",			false },
	{ (void*)show_page_roundness_test,	"Random Calculate",		false },
};

const struct pages_info main_page =
{
	false,
	LGRAY,

	40,
	30,
	DARKBLUE,
	&main_title_font,

	60,
	100,
	DARKBLUE,
	LIGHTBLUE,
	16,
	180,
	10,
	10,
	5,
	pages,
	sizeof(pages) / sizeof(struct pages_tab)
};

const struct pages_info debug_page =
{
	true,
	WHITE,
	60,
	30,
	RED,
	&debug_title_font,

	45,
	80,
	BLUE,
	BLUE,
	16,
	200,
	10,
	5,
	3,
	debug_pages,
	sizeof(debug_pages) / sizeof(struct pages_tab)
};

/*!
* @fn	void select_page_highligt_option(u8 n, u16 color, const struct pages_info info)
*
* @brief	通用界面选项选择
*
* @author	Hintay
* @date	2018/4/16
*
* @param	n	 	An u8 to process.
* @param	color	The color.
* @param	info 	The information.
*/

void select_page_highligt_option(u8 n, u16 color, const struct pages_info info)
{
	POINT_COLOR = color;

	// 矩形
	LCD_DrawRectangle(
		info.option_x - info.option_highlight_padding_x,
		info.option_y - info.option_highlight_padding_y + (info.option_spacing + info.option_height + info.option_highlight_padding_y * 2) * n,
		info.option_width + info.option_highlight_padding_x,
		info.option_y + info.option_highlight_padding_y + info.option_height + (info.option_spacing + info.option_height + info.option_highlight_padding_y * 2) * n
	);
	// 矩形加粗
	LCD_DrawRectangle(
		info.option_x - info.option_highlight_padding_x + 1,
		info.option_y - info.option_highlight_padding_y + (info.option_spacing + info.option_height + info.option_highlight_padding_y * 2) * n + 1,
		info.option_width + info.option_highlight_padding_x - 1,
		info.option_y + info.option_highlight_padding_y + info.option_height + (info.option_spacing + info.option_height + info.option_highlight_padding_y * 2) * n - 1
	);
}

void init_select_page(u8 select_option, const struct pages_info info)
{
	u8 i;
	// 设定背景颜色
	LCD_Clear(info.background_color);
	BACK_COLOR = info.background_color;

	// 显示标题
	POINT_COLOR = info.title_color;
	for (i = 0; i < info.title_font->data_len; i++)
		show_chinese_char(info.title_x + info.title_font->font_size * i, info.title_y, i, info.title_font->titile_font_data, info.title_font->font_size);

	// 显示选项
	POINT_COLOR = info.option_color;
	if(info.options->is_chinese)
	{
		for (i = 0; i < info.option_length; i++)
		{
			show_chinese_string(
				info.option_x,
				info.option_y + (info.option_height + info.option_highlight_padding_y * 2 + info.option_spacing) * i,
				chinese_menu_index[i /*info.options->chinese_index*/],
				info.option_height == 16 ? regular_font_16 : regular_font_24,
				info.option_height
			);
		}
	}
	else
	{
		for (i = 0; i < info.option_length; i++)
		{
			LCD_ShowString(
				info.option_x,
				info.option_y + (info.option_height + info.option_highlight_padding_y * 2 + info.option_spacing) * i,
				info.option_width,
				info.option_height,
				info.option_height,
				(u8*)(info.options[i].name)
			);
		}
	}
	

	select_page_highligt_option(select_option, info.option_highlight_color, info);
}

void show_lcd_select_page(const struct pages_info info)
{
	bool loop_flag = true;
	u8 kv, select_option = 0;

	init_select_page(select_option, info);

	while (loop_flag)
	{
		if (timerFlag)
		{
			timerFlag = 0;

			/// 按键扫描 ///
			kv = KEY_Scan(0);	//得到键值
			if (kv)
			{
				switch (kv)
				{
				case KEY_UP:
				case KEY_DOWN:	// 切换选项
					select_page_highligt_option(select_option, info.background_color, info);	// 使用背景色清除原边框
					select_option = (info.option_length + select_option + (kv == KEY_DOWN ? 1 : -1)) % info.option_length;
					select_page_highligt_option(select_option, info.option_highlight_color, info);
					break;

				case KEY_ENTER:
				case KEY_RIGHT:	// 进入子界面
					if (select_option >= 0 && select_option < info.option_length)
					{
						LCD_Clear(info.background_color);		// 清屏
						(*(u32(*)())(info.options[select_option].func))();	// 页面显示容器函数，需要内置循环，否则会跳出

						init_select_page(select_option, info);
					}
					break;
				case KEY_LEFT:	// 返回上一级界面
					if (info.backable)
						loop_flag = 0;
					break;
				}
			}
		}
	}
}

/*!
 * @struct	settings_page
 *
 * @brief	设置界面结构
 *
 * @author	Hintay
 * @date	2018/4/19
 */

//#define  BUFF_INT_SIZE 6		// 小数点前位数
//#define BUFF_DECIMAL_SIZE 6	// 小数点后位数
//
//void get_buff_from_float(double input, u8 *buff)
//{
//	double temp = input;
//
//	u16 temp_int = temp;	// 拆分小数点前的数据
//	if(temp_int != 0)
//	{
//		for (signed char i = BUFF_DECIMAL_SIZE - 1; i >= 0; i--)
//		{
//			buff[i] = temp_int % 10;
//			printf("%d, ", buff[i]);
//			temp_int /= 10;
//			//if (temp_int == 0)	break;
//		}
//	}
//
//	for (u8 i = BUFF_INT_SIZE; i < BUFF_INT_SIZE+BUFF_DECIMAL_SIZE; i++)
//	{
//		temp *= 10;
//		buff[i] = ((int)temp) % 10;
//		printf("%d, ", buff[i]);
//	}
//}
//
//// 从一个 Buff 中整合出浮点数
//float get_float_from_buff(u8 *buff)
//{
//	// 整数部分
//	u16 temp = 0;
//	for (u8 i = 0; i < BUFF_INT_SIZE; i++)
//	{
//		temp += buff[i];
//		temp *= 10;
//	}
//
//	// 小数部分
//	double temp_float = buff[BUFF_INT_SIZE + BUFF_DECIMAL_SIZE - 1];	// 最后一位
//	for (u8 i = BUFF_INT_SIZE + BUFF_DECIMAL_SIZE - 1; i >= BUFF_INT_SIZE; i--)
//	{
//		temp_float /= 10;
//		temp_float += buff[i];
//	}
//	return (float)(temp_float + temp);
//}

// 将浮点数存储到一个 Buff 中
/*void get_buff_from_float(double input, u16 *float_buff)
{
	double temp = input;

	float_buff[0] = temp;	// 保存小数点前的数据
	for(u8 i = 1; i < 7; i++)
	{
		temp *= 10;
		float_buff[i] = (int)temp % 10;
	}
}

// 从一个 Buff 中整合出浮点数
float get_float_from_buff(u16 *float_buff)
{
	double temp = float_buff[6];
	for(signed char i=5; i>=0; i--)
	{
		temp /= 10;
		temp += float_buff[i];
	}
	return (float)temp;
}

void get_buff_from_u16(u16 input, u8 size, u16 *int_buff)
{
	u16 temp = input;
	for(signed char i = size-1; i >= 0; i--)
	{
		int_buff[i] = temp % 10;
		temp /= 10;
	}
}

u16 get_u16_from_buff(u16 *int_buff, u8 size)
{
	u16 temp = 0;
	for(u8 i = 0; i < size; i++)
	{
		temp += int_buff[i];
		temp *= 10;
	}
	return temp;
}*/

//u8 settings_item_x[100];				// x 位置			结构 { x1, x2, x3, x4 ....}
//u8 settings_item_rows[10];				// x 结束位置索引	结构 { 1x_e, 2x_e ....}
//u8 settings_item_row_length = 0;		// 几行
//u16 *settings_item_row_data_p[20];		// 数组指针，每行两个 小数点前，小数点后
//u8 *settings_item_x_p = settings_item_x;
//u16 **settings_item_row_data_p_p = settings_item_row_data_p;
//u8 *settings_item_rows_p = settings_item_rows;
//u16 settings_item_x_index = 0;

//void show_changeable_nums(u16 x, u16 y, /*u8 row, u8 start_column,*/ u16 *float_buff)
//{
//	*settings_item_row_data_p_p = NULL;
//
//	for (u8 i = 0; i<7; i++)
//	{
//		if (i == 0)
//		{
//			u16 temp_num = float_buff[i], num_size = 1;
//			for (; temp_num > 10; temp_num /= 10)
//				num_size++;	// 获取数据大小
//
//			//LCD_ShowNum(x, y, float_buff[i], num_size, 16);
//			u16 int_buff[5];
//			get_buff_from_u16(float_buff[i], num_size, int_buff);
//			for(u8 j=0; j<num_size; j++)
//			{
//				LCD_ShowChar(x, y, int_buff[j] + '0', 16, 0);
//				*settings_item_x_p++ = x;
//				settings_item_x_index++;
//
//				x += 8;
//			}
//			*settings_item_row_data_p_p = int_buff;
//
//			LCD_ShowChar(x, y, '.', 16, 0);
//			x += 8;
//		}
//		else
//		{
//			LCD_ShowChar(x, y, float_buff[i] + '0', 16, 0);
//			*settings_item_x_p++ = x;
//			settings_item_x_index++;
//
//			x += 8;
//		}
//	}
//	*(++settings_item_row_data_p_p) = float_buff;
//	*settings_item_rows_p++ = settings_item_x_index;	// 保存 x 的索引
//
//	settings_item_row_data_p_p++;	// 移动储存数据的指针
//	settings_item_row_length++;		// 行数+1
//}

#define NUMBER_SIZE 8

/*!
 * @fn	void show_fake_float(u8 x, u8 y, u8 point_index, u32 num, u8 size, u8 highlight_index)
 *
 * @brief	显示放大后的浮点数
 *
 * @author	Hintay
 * @date	2018/4/20
 *
 * @param	x			   	An u8 to process.
 * @param	y			   	An u8 to process.
 * @param	point_index	   	Zero-based index of the point.
 * @param	num			   	Number of.
 * @param	size		   	The size.
 * @param	highlight_index	Zero-based index of the highlight.
 */

void show_fake_float(u8 x, u8 y, u8 point_index, u32 num, u8 size, u8 highlight_index)
{
	
	u8 temp_num, i;

	/*char temp_text[20];
	sprintf(temp_text, "%ld", num);
	LCD_ShowString(40, 200, 200, 16, 16, temp_text);*/

	for(i=0; i<size; i++)
	{
		temp_num = (num / LCD_Pow(10, size - i -1)) % 10;

		if (highlight_index && highlight_index-1 == i)
		{	// 高亮
			// 取反背景色与前景色
			u16 temp_color = POINT_COLOR;
			POINT_COLOR = BACK_COLOR;
			BACK_COLOR = temp_color;

			LCD_ShowChar(x, y, temp_num + '0', 16, 0);

			// 取反背景色与前景色
			temp_color = POINT_COLOR;
			POINT_COLOR = BACK_COLOR;
			BACK_COLOR = temp_color;
		}
		else
		{
			LCD_ShowChar(x, y, temp_num + '0', 16, 0);
		}

		x += NUMBER_SIZE;
		if(point_index && point_index-1 == i)
		{	// 小数点位置 0为无
			LCD_ShowChar(x, y, '.', 16, 0);
			x += NUMBER_SIZE;
		}
	}
}

/*!
* @struct	saved_info
*
* @brief	用于保存的信息
*
* @author	Hintay
* @date	2018/4/19
*/

struct saved_info
{
	u16 base_shifted_adc;	// 基准位移 ADC 值
	float allowable_error;	// 允许误差
	double test_value1;
	double test_value2;
};
struct saved_info saved_info = { 0, 0, 31.567, 3.14159 };

#define SAVED_INFO_SIZE sizeof(saved_info)
#define SAVED_INFO_START_ADDR 0

void save_info()
{
	AT24CXX_Write(SAVED_INFO_START_ADDR, (u8*)&saved_info, SAVED_INFO_SIZE);
}

void read_info()
{
	AT24CXX_Read(SAVED_INFO_START_ADDR, (u8*)&saved_info, SAVED_INFO_SIZE);
}

/*!
 * @struct	settings_item
 *
 * @brief	保存设置界面的参数
 *
 * @author	Hintay
 * @date	2018/4/20
 */

struct settings_item
{
	u16 x;
	u16 y;
	u8* info_text;
	double *vaule;				// 保存的值
	const u32 magnification;	// 放大倍数
	u8 show_len;				// 要显示的位数
	u8 point_index;
};

struct settings_item settings_items[] =
{
	{ 40, 90,	"Test Setting1 (mm):", &saved_info.test_value1,	1000,	5,	2 },
	{ 40, 150,	"Test Setting2 (mm):", &saved_info.test_value2,	100000,	6,	1 }
};
const u8 settings_row = sizeof(settings_items) / sizeof(struct settings_item);

u16 get_decimals_index(double num) {
	num = num - (u16)num;
	for (int i = 0; i<10; i++) {
		num *= 10;
		if (num - (u16)num == 0) {
			return i + 1;
		}
	}
}

float motor_rpm = 5.0;

bool is_set_95 = false;
bool is_set_105 = false;


#define FINAL_PAGE_START_X	20
#define FINAL_PAGE_START_Y	30

