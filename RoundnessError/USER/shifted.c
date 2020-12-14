/*!
* @file	USER\shifted.c.
*
* @brief	位移传感计算
*/

#include "shifted.h"
#include "usart.h"
#include "common.h"

//#define ADC_VALUE_PRE_MM (ADC_RANGE / (double)SHIFTED_RANGE)

#if !TEST_MODE

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
	saved_info.adc_value_pre_mm = (saved_info.adc_105mm - saved_info.adc_95mm) / 5.0;	// 直径相差 10 mm 但此处应使用半径值
	printf("%f\n", saved_info.adc_value_pre_mm);

	save_info();
}

/*!
 * @fn	double get_relative_shifted_value(u16 adc_value)
 *
 * @brief	获取相对位移值
 *
 * @author	Hintay
 * @date	2018/4/21
 *
 * @param	adc_value	The ADC value.
 *
 * @return	The shifted value.
 */

double get_relative_shifted_value(u16 adc_value)
{
	return adc_value / saved_info.adc_value_pre_mm;
}

/*!
 * @fn	double get_shifted_value_base_95mm(u16 adc_value)
 *
 * @brief	获取基于 95mm 直径 (47.5mm 半径) 的位移值
 *
 * @author	Hintay
 * @date	2018/4/21
 *
 * @param	adc_value	The ADC value.
 *
 * @return	The shifted value final.
 */

double get_shifted_value_base_95mm(u16 adc_value)
{
	//printf("%d\n", adc_value);
	return get_relative_shifted_value(adc_value - saved_info.adc_95mm) + 47.5;	// 直径 95 半径 47.5
}
#endif
