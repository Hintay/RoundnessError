/*!
 * @file	HARDWARE\KEY\key.c.
 *
 * @brief	������������
 */

#include "key.h"
#include "sys.h" 
#include "delay.h"
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	// ʹ�� PORTF ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;	//PF 0~4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/*!
 * @fn	u8 KEY_Scan(u8 mode)
 *
 * @brief	����������
 *			0��û���κΰ������� 1��KEY0���� 2��KEY1���� 3��KEY2���� 4��KEY3���� WK_UP
 * 			ע��˺�������Ӧ���ȼ�, KEY0>KEY1>KEY2>KEY3!!
 *
 * @author	Hintay
 * @date	2018/4/17
 *
 * @param	mode	���ذ���ֵ mode:0,��֧��������;1,֧��������;
 *
 * @return	An u8.
 */

u8 KEY_Scan(u8 mode)
{
	static u8 key_up = 1;	// �������ɿ���־
	if (mode)key_up = 1;	// ֧������
	if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0))
	{
		delay_ms(10);		//ȥ���� 
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
	return 0;		// �ް�������
}
