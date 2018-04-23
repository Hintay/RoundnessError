#pragma once
#ifndef __ENCODER_H
#define __ENCODER_H

#include "sys.h"

void Encoder_Init(void);

extern int encoder_count;

extern double angle_shifted_base95[360];	// 保存每度的位移值
 
#define EDGE_TIMES 12960 // 脉冲数

#endif
