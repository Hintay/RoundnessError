#pragma once
/*!
* @file	USER\common.c.
*
* @brief	通用包含头文件
*/

#include "sys.h"

#define TEST_MODE 0	// 是否为测试模式？

/*!
* @struct	saved_info
*
* @brief	用于保存通用的信息
*
* @author	Hintay
* @date	2018/4/21
*/

struct saved_info
{
#if TEST_MODE
	u16 base_shifted_adc;	// 基准位移 ADC 值
	float allowable_error;	// 允许误差
	double test_value1;
	double test_value2;
#else
	int motor_rpm;				// 电机转速
	u16 adc_95mm;				// 95mm 直径标准圆的 ADC 值
	u16 adc_105mm;				// 105mm 直径标准圆的 ADC 值
	double adc_value_pre_mm;	// 每毫米 ADC 值
#endif
};

struct saved_info saved_info;
void save_info();
void read_info();

#define SAVED_INFO_SIZE sizeof(saved_info) / sizeof(struct saved_info)
#define SAVED_INFO_START_ADDR 0
