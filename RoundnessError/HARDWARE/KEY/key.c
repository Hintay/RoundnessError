/*!
 * @file	HARDWARE\KEY\key.c.
 *
 * @brief	按键驱动代码
 */

#include "key.h"
#include "sys.h" 
#include "delay.h"
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	// 使能 PORTF 时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;	//PF 0~4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/*!
 * @fn	u8 KEY_Scan(u8 mode)
 *
 * @brief	按键处理函数
 *			0，没有任何按键按下 1，KEY0按下 2，KEY1按下 3，KEY2按下 4，KEY3按下 WK_UP
 * 			注意此函数有响应优先级, KEY0>KEY1>KEY2>KEY3!!
 *
 * @author	Hintay
 * @date	2018/4/17
 *
 * @param	mode	返回按键值 mode:0,不支持连续按;1,支持连续按;
 *
 * @return	An u8.
 */

u8 KEY_Scan(u8 mode)
{
	static u8 key_up = 1;	// 按键按松开标志
	if (mode)key_up = 1;	// 支持连按
	if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0))
	{
		delay_ms(10);		//去抖动 
		key_up = 0;
		if (KEY0 == 0)
			return KEY_UP;
		if (KEY1 == 0)
			return KEY_LEFT;
		if (KEY2 == 0)
			return KEY_DOWN;
		if (KEY3 == 0)
			return KEY_RIGHT;
		if (KEY4 == 0)
			return KEY_ENTER;
	}
	else if (KEY0 == 1 && KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1) key_up = 1;
	return 0;		// 无按键按下
}
