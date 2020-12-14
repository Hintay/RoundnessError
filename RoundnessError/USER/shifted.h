#pragma once

/*!
* @file	USER\shifted.h.
*
* @brief	位移传感计算头文件
*/

#include "sys.h"

//#define SHIFTED_RANGE 20	// 量程 单位 mm
#define ADC_RANGE 4095		// ADC 转换的量程

double get_relative_shifted_value(u16 adc_value);
double get_shifted_value_base_95mm(u16 adc_value);
void get_adc_value_pre_mm();
