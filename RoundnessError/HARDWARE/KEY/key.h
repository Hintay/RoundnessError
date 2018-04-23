#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//按键驱动代码	   						  
//////////////////////////////////////////////////////////////////////////////////   	 
 
#define KEY0  GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0) // K1
#define KEY1  GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_1) // K2
#define KEY2  GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_2) // K3
#define KEY3  GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_3) // K4
#define KEY4  GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_4) // K5


#define KEY_ENTER	5
#define KEY_UP 		4
#define KEY_LEFT	3
#define KEY_DOWN	2
#define KEY_RIGHT	1

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数					    
#endif
