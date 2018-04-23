#pragma once

/**
* 位移传感
*/

#include "sys.h"

//#define SHIFTED_RANGE 20	// 量程 单位 mm
#define ADC_RANGE 4095		// ADC 转换的量程

double get_shifted_value(u16 adc_value);
double get_shifted_value_final(u16 adc_value);
void get_adc_value_pre_mm();

double adc_value_pre_mm;
u16 adc_95mm;
u16 adc_105mm;
