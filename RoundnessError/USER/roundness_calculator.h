#ifndef _ROUNDNESS_CALULATOR_H
#define _ROUNDNESS_CALULATOR_H
#include "sys.h"

struct roundness_error
{
	double u1;		// Æ«ÒÆÔ²ÐÄ x
	double u2;		// Æ«ÒÆÔ²ÐÄ y
	double r0;
	double r_max;
	double r_min;
	double error;
};
extern struct roundness_error re_dev;

void calculate_roundness_error_by_square(double* offset_list, u8 list_len);
void test_calculate_roundness_error(void);

#endif
