#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h" 
void TIM4_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr, u16 psc);


extern u8 timerFlag;
extern u8 timerFlag_50ms;
 
#endif
