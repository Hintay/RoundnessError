#include "roundness_calculator.h"
#include "math.h"
#include "lcd.h"
#include <stdio.h>

#define RANDOM_SAMPLES 40

double* generate_random_data()
{
	u8 i;
	static double offset_list[RANDOM_SAMPLES];
	for(i=0; i < RANDOM_SAMPLES; i++)
	{
		offset_list[i] = random()/1000000000.0;
		//printf("%f, ", offset_list[i]);
	}
	printf("\r\n");
	return offset_list;
}

struct roundness_error re_dev;

void calculate_roundness_error_by_square(double* offset_list, u8 list_len)
{
	u8 i;
	double angle_delta, delta, delta_r[list_len];
	double max_r=0, min_r=0;
	re_dev.u1 = re_dev.u2 = re_dev.r0 = 0.0;

	angle_delta = 360.0 / list_len;		// 每个采样的度数
	delta = angle_delta / 180.0 * 3.141592653589793;	// 角度变化量
	for(i=0; i<list_len; i++)
	{
		re_dev.r0 += offset_list[i] / (list_len * 1.0);
		re_dev.u1 += -2 / (list_len * 1.0) * offset_list[i] * cos(delta * i);
		re_dev.u2 += -2 / (list_len * 1.0) * offset_list[i] * sin(delta * i);
	}

	for (i = 0; i<list_len; i++)
	{
		delta_r[i] = offset_list[i] - (re_dev.r0 + re_dev.u1 * cos(delta * i) + re_dev.u2 * sin(delta * i));
		if (delta_r[i] > max_r) max_r = delta_r[i];
		else if (delta_r[i] < min_r) min_r = delta_r[i];
	}
	re_dev.r_max = max_r;
	re_dev.r_min = min_r;
	re_dev.error = max_r - min_r;
}

void test_calculate_roundness_error(void)
{
	calculate_roundness_error_by_square(generate_random_data(), RANDOM_SAMPLES);
}

