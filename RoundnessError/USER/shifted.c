/**
 * 位移传感
 */

#include "shifted.h"
#include "usart.h"

//#define ADC_VALUE_PRE_MM (ADC_RANGE / (double)SHIFTED_RANGE)

// 初始值
u16 adc_105mm = 4095;
u16 adc_95mm = 0;

/*!
 * @fn	double get_adc_value_pre_mm()
 *
 * @brief	获取每毫米 ADC 的值
 *
 * @author	Hintay
 * @date	2018/4/21
 *
 * @return	The ADC value pre millimetres.
 */

void get_adc_value_pre_mm()
{
	adc_value_pre_mm = (adc_105mm - adc_95mm) / 5.0;	// 直径相差 10 mm 但此处应使用半径值
	printf("%f\n", adc_value_pre_mm);
}


double get_shifted_value(u16 adc_value)
{
	return adc_value / adc_value_pre_mm;
}

double get_shifted_value_final(u16 adc_value)
{
	//printf("%d\n", adc_value);
	return get_shifted_value(adc_value - adc_95mm) + 47.5;	// 直径 95 半径 47.5
}