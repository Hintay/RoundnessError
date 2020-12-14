/*!
* @file	USER\roundness_calculator.c.
*
* @brief	圆度误差计算函数头文件
*/

#ifndef _ROUNDNESS_CALULATOR_H
#define _ROUNDNESS_CALULATOR_H
#include "sys.h"

/*!
 * @struct	roundness_error
 *
 * @brief	保存计算出来的圆度误差数据，用于全局使用
 *
 * @author	Hintay
 * @date	2018/4/18
 */

struct roundness_error
{
	double u1;		// 偏移圆心 x
	double u2;		// 偏移圆心 y
	double r0;
	double r_max;
	double r_min;
	double error;
};
extern struct roundness_error re_dev;

void calculate_roundness_error_by_square(double* offset_list, u8 list_len);
void test_calculate_roundness_error(void);

#endif
