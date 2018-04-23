#include "main.h"
#include "led.h"
#include "delay.h"
#include "lcd.h"
#include "adc.h"
#include "roundness_calculator.h"
#include "shifted.h"
#include "encoder.h"

#if 1
void show_page_debug()
{
	show_lcd_select_page(debug_page);
}

void show_page_adc()
{
	/// ADC ��ʾ ///
	u16 adcx;
	char temp_text[20];
	u8 loop_flag = 1;
	u8 kv;

	POINT_COLOR = BLUE;

	while (loop_flag)
	{
		if (timerFlag)
		{
			timerFlag = 0;

			adcx = Get_Adc_Average(ADC_Channel_1, 10);
			sprintf(temp_text, "ADC: %-4d", adcx);
			LCD_ShowString(60, 50, 200, 16, 16, temp_text);

			// ��ʾ��ѹֵ
			sprintf(temp_text, "ADC (V): %0.3f", adcx*(3.3 / 4095));
			LCD_ShowString(60, 70, 200, 16, 16, temp_text);

			// ��ʾλ��ֵ
			LCD_ShowString(60, 90, 200, 16, 16, "Shifted (mm):"); 
			sprintf(temp_text, "      %2.6f", get_shifted_value(adcx));
			LCD_ShowString(60, 110, 200, 16, 16, temp_text);

			/// ����ɨ�� ///
			kv = KEY_Scan(0);	//�õ���ֵ
			if (kv)
			{
				switch (kv)
				{
				case KEY_LEFT:	// ����������
					loop_flag = 0;
					break;
				default: break;
				}
			}
		}
	}
}

void show_page_counter()
{
	u8 kv, loop_flag = 1;
	char temp_text[20];

	TIM_SetCounter(TIM4, 0);	// ����
	TIM_Cmd(TIM4, ENABLE);

	POINT_COLOR = BLUE;

	while (loop_flag)
	{
		if (timerFlag)
		{
			timerFlag = 0;

			// ��ʾ����ֵ
			sprintf(temp_text, "Count: %6d", TIM4->CNT);
			LCD_ShowString(60, 70, 200, 16, 16, temp_text);
			// ��ʾ����ֵ
			sprintf(temp_text, "Test: %6d", encoder_count);
			LCD_ShowString(60, 90, 200, 16, 16, temp_text);

			/// ����ɨ�� ///
			kv = KEY_Scan(0);	//�õ���ֵ
			if (kv)
			{
				switch (kv)
				{
				case KEY_LEFT:	// ����������
					loop_flag = 0;
					break;
				default: break;
				}
			}
		}
	}

	TIM_Cmd(TIM4, DISABLE);
	TIM_SetCounter(TIM4, 0);
}

void show_page_roundness_test()
{
	u8 kv, loop_flag = 1;
	char temp_text[20];

	POINT_COLOR = BLUE;

	test_calculate_roundness_error();

	// ��ʾ����ֵ
	sprintf(temp_text, "u1: %6.6f", re_dev.u1);
	LCD_ShowString(60, 70, 200, 16, 16, temp_text);
	sprintf(temp_text, "u2: %6.6f", re_dev.u2);
	LCD_ShowString(60, 90, 200, 16, 16, temp_text);
	sprintf(temp_text, "r0: %6.6f", re_dev.r0);
	LCD_ShowString(60, 110, 200, 16, 16, temp_text);
	sprintf(temp_text, "r_max: %6.6f", re_dev.r_max);
	LCD_ShowString(60, 130, 200, 16, 16, temp_text);
	sprintf(temp_text, "r_min: %6.6f", re_dev.r_min);
	LCD_ShowString(60, 150, 200, 16, 16, temp_text);
	sprintf(temp_text, "error: %6.6f", re_dev.error);
	LCD_ShowString(60, 170, 200, 16, 16, temp_text);

	while (loop_flag)
	{
		if (timerFlag)
		{
			timerFlag = 0;

			/// ����ɨ�� ///
			kv = KEY_Scan(0);	//�õ���ֵ
			if (kv)
			{
				switch (kv)
				{
				case KEY_LEFT:	// ����������
					loop_flag = 0;
					break;
				default: break;
				}
			}
		}
	}
}

u8 get_settings_num_by_index(u32 temp_u32, u8 cur_pos_row, u8 cur_pos_column)
{
	
	return (u8)((temp_u32 / LCD_Pow(10, settings_items[cur_pos_row].show_len - cur_pos_column - 1)) % 10);
}

void show_page_settings_numbers(/*const u8 *show_size, const u8 *point_index,*/ u8 cur_pos_row, u8 cur_pos_column)
{
	u8 i;
	const u16 current_color = POINT_COLOR;

	for(i=0; i<settings_row; i++)
	{
		LCD_ShowString(settings_items[i].x, settings_items[i].y, 200, 16, 16, settings_items[i].info_text);
		if (cur_pos_row == i)
			POINT_COLOR = RED;
		show_fake_float(settings_items[i].x, settings_items[i].y + 20, settings_items[i].point_index, (u32)(*settings_items[i].vaule * settings_items[i].magnification),
			settings_items[i].show_len, cur_pos_row == i ? cur_pos_column : 0);
		POINT_COLOR = current_color;
	}
}

void show_page_settings()
{
	u8 kv, loop_flag = 1;
	char temp_text[20];
	u16 adcx;

	u8 cur_pos_column = 0;
	u8 cur_pos_row = 0;
	bool is_change_mod = false;

	// ��ʾ����
	POINT_COLOR = DARKBLUE;
	show_chinese_string( 72, 30, chinese_menu_index[1], regular_font_24, 24 );

	POINT_COLOR = BLUE;

	show_page_settings_numbers(cur_pos_row, cur_pos_column);

	while (loop_flag)
	{
		if (timerFlag)
		{
			timerFlag = 0;

			//adcx = Get_Adc_Average(ADC_Channel_1, 10);	// �� ADC ƽ��ֵ

			// ��ʾλ��ֵ
			/*LCD_ShowString(40, 70, 200, 16, 16, "Current Shifted (mm):");
			sprintf(temp_text, "  %2.6f", get_shifted_value(adcx) - get_shifted_value(saved_info.base_shifted_adc));
			LCD_ShowString(40, 90, 200, 16, 16, temp_text);*/

			//LCD_ShowString(40, 110, 200, 16, 16, "Base Shifted (mm):");
			//sprintf(temp_text, "  %2.6f", saved_info.base_shifted);
			//LCD_ShowString(40, 130, 200, 16, 16, temp_text);

			//LCD_ShowString(40, 110, 200, 16, 16, "Allowable Error (mm):");
			//sprintf(temp_text, "  %2.6f", test_float);
			//LCD_ShowString(40, 130, 200, 16, 16, temp_text);
			//show_nums_from_buff(60, 130, float_buff);

			/// ����ɨ�� ///
			kv = KEY_Scan(0);	//�õ���ֵ
			if (kv)
			{
				switch (kv)
				{
				case KEY_RIGHT:
				case KEY_LEFT:	// ���ƻ�����
					if(is_change_mod)
					{
						cur_pos_column = (settings_items[cur_pos_row].show_len + cur_pos_column + (kv == KEY_RIGHT ? 1 : -1)) % settings_items[cur_pos_row].show_len;
						show_page_settings_numbers(cur_pos_row, cur_pos_column+1);	// ˢ��
					}
					else if(cur_pos_row == settings_row)
					{	// ���һ�� ֻ������ ȷ��/ȡ��
						cur_pos_column = (2 + cur_pos_column + (kv == KEY_RIGHT ? 1 : -1)) % 2;
					}
					break;
				case KEY_UP:
				case KEY_DOWN:	// ���ƻ�����
					if(is_change_mod)
					{	// �޸�ģʽ
						u32 temp_u32 = *settings_items[cur_pos_row].vaule * settings_items[cur_pos_row].magnification;
						const u8 current_num = get_settings_num_by_index(temp_u32, cur_pos_row, cur_pos_column );
						if(kv == KEY_UP)
						{
							if (current_num == 9)
								temp_u32 -= 9 * LCD_Pow(10, settings_items[cur_pos_row].show_len - cur_pos_column - 1);
							else
								temp_u32 += LCD_Pow(10, settings_items[cur_pos_row].show_len - cur_pos_column - 1);
						}
						else
						{
							if (current_num == 0)
								temp_u32 += 9 * LCD_Pow(10, settings_items[cur_pos_row].show_len - cur_pos_column - 1);
							else
								temp_u32 -= LCD_Pow(10, settings_items[cur_pos_row].show_len - cur_pos_column - 1);
						}

						*settings_items[cur_pos_row].vaule = temp_u32 / (double)settings_items[cur_pos_row].magnification;
						show_page_settings_numbers(cur_pos_row, cur_pos_column + 1);	// ˢ����ֵ
					}
					else
					{	// �л�ѡ��
						//cur_pos_column = 0; // ����������Ϊ0
						cur_pos_row = (settings_row + 1 + cur_pos_row + (kv == KEY_DOWN ? 1 : -1)) % (settings_row + 1);
						show_page_settings_numbers(cur_pos_row, 0);	// ˢ�¸�����
					}
					
					break;
				case KEY_ENTER:
					// �м��ֻ�� ȷ�� �� ȡ�� �������޸�ģʽʱʹ��
					if(cur_pos_row < settings_row)	// �����ȡ���޸�ģʽ
					{
						is_change_mod = !is_change_mod;
						cur_pos_column = 0;
						show_page_settings_numbers(cur_pos_row, is_change_mod ? cur_pos_column + 1 : 0);	// ˢ����ֵ
					}
					else
					{ // ���ȷ�ϻ�ȡ��
						if(cur_pos_column == 0)
						{	// ���ȷ�� - ���㲢������Ϣ
							save_info();
						}
						loop_flag = 0;
					}
					break;
				default: break;
				}
			}
		}
	}
}

#endif

#define FINAL_PAGE_START_X	20
#define FINAL_PAGE_START_Y	30

void show_final_page_settings_item(u8 cur_row)
{
	u8 x = FINAL_PAGE_START_X, y = FINAL_PAGE_START_Y;
	char temp_text[20];

	if (cur_row == 0)
		POINT_COLOR = BLACK;
	else
		POINT_COLOR = BLUE; 
	y += 30;
	LCD_ShowString(FINAL_PAGE_START_X, y, 200, 16, 16, "95mm");		// 95mm
	x += 34;
	show_chinese_string(x, y, output_index[1], output_font, 16);	// ��׼Բ�趨
	x += output_index[1][0] * 16 + 8;
	show_chinese_char(x, y, 0, block_font, 16);		// ��
	x += 16;
	LCD_ShowChar(x, y, '/', 16, 0);
	x += 8;
	show_chinese_char(x, y, 1, block_font, 16);		// ��
	x = FINAL_PAGE_START_X;		// x ��λ

	if (cur_row == 1)
		POINT_COLOR = BLACK;
	else
		POINT_COLOR = BLUE;
	y += 30;
	LCD_ShowString(FINAL_PAGE_START_X, y, 200, 16, 16, "105mm");		// 95mm
	x += 42;
	show_chinese_string(x, y, output_index[1], output_font, 16);	// ��׼Բ�趨
	x += output_index[1][0] * 16;
	show_chinese_char(x, y, 0, block_font, 16);		// ��
	x += 16;
	LCD_ShowChar(x, y, '/', 16, 0);
	x += 8;
	show_chinese_char(x, y, 1, block_font, 16);		// ��
	x = FINAL_PAGE_START_X;		// x ��λ

	if (cur_row == 2)
		POINT_COLOR = BLACK;
	else
		POINT_COLOR = BLUE;
	y += 30;
	show_chinese_string(x, y, output_index[2], output_font, 16);
	x += output_index[2][0] * 16;
	sprintf(temp_text, "%1.1f rpm", motor_rpm);
	LCD_ShowString(x, y, 200, 16, 16, temp_text);

	POINT_COLOR = BLUE;
}

void show_final_page_settings()
{
	bool loop_flag = true, is_change_mod = false, is_biling = false, can_biling = true;
	u8 kv, cur_row = 0;
	char temp_text[20];

	/// ��ʾ���� ///
	show_chinese_string(FINAL_PAGE_START_X, FINAL_PAGE_START_Y, output_index[0], output_font, 16);	// �����趨

	show_final_page_settings_item(0);

	while (loop_flag)
	{
		// ��ʾ��˸
		//if (is_change_mod && timerFlag_50ms)
		if (can_biling && timerFlag_50ms)
		{
			timerFlag_50ms = 0;

			POINT_COLOR = BLACK;
			switch (cur_row)
			{
			case 0:	// 95mm ��׼Բ�趨
				if(is_set_95)
				{
					if (is_biling)
					{
						LCD_ShowChar(FINAL_PAGE_START_X + 34 + output_index[1][0] * 16 + 8 + 16 + 8, FINAL_PAGE_START_Y + 30, ' ', 16, 0);
						LCD_ShowChar(FINAL_PAGE_START_X + 34 + output_index[1][0] * 16 + 16 + 16 + 8, FINAL_PAGE_START_Y + 30, ' ', 16, 0);
					}
					else
					{
						show_chinese_char(FINAL_PAGE_START_X + 34 + output_index[1][0] * 16 + 8 + 16 + 8, FINAL_PAGE_START_Y + 30, 1, block_font, 16);		// ��
					}
				}
				else
				{
					if (is_biling)
					{
						LCD_ShowChar(FINAL_PAGE_START_X + 34 + output_index[1][0] * 16 + 8, FINAL_PAGE_START_Y + 30, ' ', 16, 0);
						LCD_ShowChar(FINAL_PAGE_START_X + 34 + output_index[1][0] * 16 + 16, FINAL_PAGE_START_Y + 30, ' ', 16, 0);
					}
					else
					{
						show_chinese_char(FINAL_PAGE_START_X + 34 + output_index[1][0] * 16 + 8, FINAL_PAGE_START_Y + 30, 0, block_font, 16);		// ��
					}
				}
				is_biling = !is_biling;
				break;
			case 1:	// 105mm ��׼Բ�趨
				if (is_set_105)
				{
					if (is_biling)
					{
						LCD_ShowChar(FINAL_PAGE_START_X + 34 + output_index[1][0] * 16 + 8 + 16 + 8, FINAL_PAGE_START_Y + 60, ' ', 16, 0);
						LCD_ShowChar(FINAL_PAGE_START_X + 34 + output_index[1][0] * 16 + 16 + 16 + 8, FINAL_PAGE_START_Y + 60, ' ', 16, 0);
					}
					else
					{
						show_chinese_char(FINAL_PAGE_START_X + 34 + output_index[1][0] * 16 + 8 + 16 + 8, FINAL_PAGE_START_Y + 60, 1, block_font, 16);		// ��
					}
				}
				else
				{
					if (is_biling)
					{
						LCD_ShowChar(FINAL_PAGE_START_X + 34 + output_index[1][0] * 16 + 8, FINAL_PAGE_START_Y + 60, ' ', 16, 0);
						LCD_ShowChar(FINAL_PAGE_START_X + 34 + output_index[1][0] * 16 + 16, FINAL_PAGE_START_Y + 60, ' ', 16, 0);
					}
					else
					{
						show_chinese_char(FINAL_PAGE_START_X + 34 + output_index[1][0] * 16 + 8, FINAL_PAGE_START_Y + 60, 0, block_font, 16);		// ��
					}
				}
				is_biling = !is_biling;
				break;
			case 2:	//���ת���趨
				if(is_change_mod)
				{
					if (is_biling)
						LCD_ShowString(FINAL_PAGE_START_X + output_index[2][0] * 16, FINAL_PAGE_START_Y + 30 * 3, 200, 16, 16, "         ");
					else
					{
						sprintf(temp_text, "%1.1f rpm", motor_rpm);
						LCD_ShowString(FINAL_PAGE_START_X + output_index[2][0] * 16, FINAL_PAGE_START_Y + 30 * 3, 200, 16, 16, temp_text);
					}
					is_biling = !is_biling;
				}
				break;
			}
			POINT_COLOR = BLUE;
		}

		if (timerFlag)
		{
			timerFlag = 0;

			/// ����ɨ�� ///
			kv = KEY_Scan(0);	//�õ���ֵ
			if (kv)
			{
				switch (kv)
				{
				case KEY_UP:
				case KEY_DOWN:	// ���ƻ�����
					if (is_change_mod)
					{	// ���ת���޸�ģʽ
						if(cur_row == 2)
						{
							while (kv)	// �õ���ֵ (������ʽ)
							{
								kv = KEY_Scan(1);

								if (kv)
								{
									if (kv == KEY_UP)
										motor_rpm += 0.1;
									else
										motor_rpm -= 0.1;

									// �޶�
									if (motor_rpm < 0)
										motor_rpm = 0;
									if (motor_rpm > 9.9)
										motor_rpm = 9.9;

									show_final_page_settings_item(cur_row);	// ˢ�½���

									delay_ms(50);
								}
							}
							// printf("%f\n", motor_rpm);
							break;
						}
					}
					else
					{	// �л�ѡ��
						cur_row = (3 + cur_row + (kv == KEY_DOWN ? 1 : -1)) % 3;
						if (cur_row == 2)
							is_change_mod = true;
						can_biling = true;
						show_final_page_settings_item(cur_row);	// ˢ�½���
					}

					break;
				case KEY_ENTER:
					switch(cur_row)
					{
					case 0:
						is_set_95 = true;
						adc_95mm = Get_Adc_Average(ADC_Channel_1, 10);
						printf("%d\n", adc_95mm);
						if(adc_95mm && adc_105mm)
							get_adc_value_pre_mm();
						can_biling = false;
						break;
					case 1:
						is_set_105 = true;
						adc_105mm = Get_Adc_Average(ADC_Channel_1, 10);
						printf("%d\n", adc_105mm);
						if (adc_95mm && adc_105mm)
							get_adc_value_pre_mm();
						can_biling = false;
						break;
					case 2:
						can_biling = is_change_mod = !is_change_mod;
						break;
					}
					is_biling = false;
					//can_biling = false;
					show_final_page_settings_item(cur_row);	// ˢ�½���
					break;
				case KEY_LEFT:	// ģʽѡ���
					//if (!is_change_mod)
					//{
						loop_flag = false;
					//}
					break;
				default: break;
				}
			}
		}
	}
}

void show_final_page_check()
{
	bool loop_flag = true;
	u8 kv;
	char temp_text[20];

	u16 start_x[3], start_y[3];

	/// ��ʾ���� ///
	u16 x = FINAL_PAGE_START_X, y = FINAL_PAGE_START_Y;
	show_chinese_string(x, y, output_index[3], output_font, 16);	// �����Ƕ�
	start_x[0] = x += output_index[3][0] * 16;
	sprintf(temp_text, "%03d", 0);
	LCD_ShowString(x, y, 200, 16, 16, temp_text);
	x += 24;
	show_chinese_char(x, y, 2, block_font, 16);		// ��
	x = FINAL_PAGE_START_X;		// x ��λ
	start_y[0] = y;

	start_y[1] = y += 30;
	show_chinese_string(x, y, output_index[4], output_font, 16);	// �����뾶
	start_x[1] = x += output_index[4][0] * 16;
	sprintf(temp_text, "%03.1f mm", 0.0);
	LCD_ShowString(x, y, 200, 16, 16, temp_text);


	/// �������� ///
	x = FINAL_PAGE_START_X+5;
	y += 30;
	LCD_DrawLine(x, y, x, y+120);				// y�ᣬ ���¸�Ԥ�� 10 ����
	LCD_DrawLine(x, y + 60, x + 190, y + 60);	// x��
	// ����ͷ
	LCD_DrawLine(x, y, x - 3, y + 5);
	LCD_DrawLine(x, y, x + 3, y + 5);
	LCD_DrawLine(x + 190, y+60, x + 185, y + 57);
	LCD_DrawLine(x + 190, y+60, x + 185, y + 63);
	start_x[2] = x;
	start_y[2] = y + 60;

	// ���������ֵ
	encoder_count = 0;
	TIM_SetCounter(TIM4, 0);

	while (loop_flag)
	{
		if (timerFlag)
		{
			timerFlag = 0;

			/// ����ɨ�� ///
			kv = KEY_Scan(0);	//�õ���ֵ
			if (kv)
			{
				switch (kv)
				{
				case KEY_RIGHT:	// ���԰���
					// ��� PWM
					TIM_Cmd(TIM3, ENABLE);  //ʹ�� 
					TIM_SetCompare2(TIM3, motor_rpm * 10);

					TIM_Cmd(TIM4, ENABLE);	// ������������

					u16 cur_angle = 0;				// ��ǰ�����ĽǶ�
					u16 cur_encoder_count = 0;		// ��ǰ�Ƕ�
					while(kv)
					{
						kv = KEY_Scan(1);	// �������

						current_adc_value = Get_Adc_Average(ADC_Channel_1, 5);	// ˢ�µ�ǰ ADC ֵ

						// ��ʾ�����뾶
						sprintf(temp_text, "%03.1f mm   ", get_shifted_value_final(current_adc_value));
						LCD_ShowString(start_x[1], start_y[1], 200, 16, 16, temp_text);

						if(cur_encoder_count != encoder_count)
						{	// ���ڽǶ��б仯ʱ���д���
							cur_encoder_count = encoder_count;

							// ˢ�²����Ƕ�
							sprintf(temp_text, "%03d", encoder_count);
							LCD_ShowString(start_x[0], start_y[0], 20, 16, 16, temp_text);

							// ���� ���ж�����ʱ����λ��ֵ�����������в�ʹ��ѭ������
							for (; cur_angle <= encoder_count; cur_angle += 2)
							{
								y = start_y[2] + 50 - angle_shifted_base95[cur_angle] * 20; // ��ʾ��Χ 47.5~52.5 ���� / �� 100 ��ʾ 5 ��Χ��Ҫ���� 20
								LCD_DrawPoint(start_x[2] + cur_angle / 2, y);
							}
						}
					}

					// �رյ�� pwm
					TIM_SetCompare2(TIM3, 0);
					TIM_Cmd(TIM3, DISABLE);

					TIM_Cmd(TIM4, DISABLE);	// �ر�����������
					break;
				case KEY_LEFT:	// ģʽѡ���
					loop_flag = false;
					break;
				default: break;
				}
			}
		}
	}
}

void show_final_first_page()
{
	u8 kv;

	// ��ʾ����
	POINT_COLOR = DARKBLUE;
	for (u8 i = 0; i < main_title_font.data_len; i++)
		show_chinese_char(40 + main_title_font.font_size * i, 130, i, main_title_font.titile_font_data, main_title_font.font_size);

	POINT_COLOR = BLUE;

	while(true)
	{
		if (timerFlag)
		{
			timerFlag = 0;

			/// ����ɨ�� ///
			kv = KEY_Scan(0);	//�õ���ֵ
			if (kv)
			{
				switch (kv)
				{
				case KEY_LEFT:	// ģʽѡ���
					while (true)
					{
						if (timerFlag)
						{
							timerFlag = 0;

							LCD_Clear(BACK_COLOR);		// ����
							show_final_page_settings();

							LCD_Clear(BACK_COLOR);		// ����
							show_final_page_check();
						}
					}
				default: break;
				}
			}
		}
	}
}


int main(void)
{
	delay_init();				// ��ʱ������ʼ��
	NVIC_Configuration();		// ����NVIC�жϷ��� 2:2 λ��ռ���ȼ���2 λ��Ӧ���ȼ�
	uart_init(9600);			// ���ڳ�ʼ��Ϊ 9600
	LED_Init();					// LED �˿ڳ�ʼ��
	LCD_Init();
	Adc_Init();					// ADC ��ʼ��
	KEY_Init();					// ��ʼ���밴�����ӵ�Ӳ���ӿ�
	AT24CXX_Init();				// IIC ��ʼ��

	// ��� IIC �е���Ϣ - ��λ��
	//save_info();
	read_info();	// ��ȡ IIC �е���Ϣ

	Encoder_Init();				// ��ʼ��������
	//OUT_Init();

	// PWM �趨
	TIM3_PWM_Init(100, 0);
	TIM_SetCompare2(TIM3, 0);	// �رն˿�

	/// ��ʼ����ʱ������ʱ��ʹ�� - 50ms ///
	TIM4_Int_Init(4999,719);	//100Khz�ļ���Ƶ�ʣ�������5000Ϊ50ms

	get_adc_value_pre_mm();

	while(true)
	{
		show_final_first_page();
		//show_lcd_select_page(main_page);	// ������
	}
}

