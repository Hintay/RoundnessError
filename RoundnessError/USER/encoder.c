#include "encoder.h"
#include <stdio.h>
#include <lcd.h>
#include "shifted.h"
#include <adc.h>
#include "common.h"

// 每度脉冲数
u16 pulse_pre_times = EDGE_TIMES / 360;	// 12960 脉冲每度 36 个脉冲

void Encoder_Init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	/* Encoder unit connected to TIM5, 4X mode */
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* TIM3 clock source enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	/* Enable GPIOA, clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	/* Configure PA.00,01 as encoder input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable the TIM5 Update Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Timer configuration in Encoder mode */
	TIM_DeInit(TIM5);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
	TIM_TimeBaseStructure.TIM_Period = pulse_pre_times * 4 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);

	// Clear all pending interrupts
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	//Reset counter
	TIM_SetCounter(TIM5, 0);

	//TIM_Cmd(TIM5, ENABLE);
}

double angle_shifted_base95[360];	// 保存每度的位移值

int encoder_count = 0;
void TIM5_IRQHandler(void)
{	// 每度一次的中断函数 采集每度的数据
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  // 检查TIM5更新中断发生与否
	{
		// 从 CR1 的 DIR 位读取计数方向
		//if (READ_BIT(TIM5->CR1, TIM_CR1_DIR))
		//	encoder_count--;
		//else
		//	encoder_count++;
		encoder_count++;

		if (encoder_count == 360)
			encoder_count = 0;

#if !TEST_MODE
		angle_shifted_base95[encoder_count] = get_relative_shifted_value(current_adc_value - saved_info.adc_95mm);
#endif
	}
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
}
