#ifndef __APP_OLED_H
#define	__APP_OLED_H

#include "stm32f10x.h"

extern uint8_t menu;
extern uint8_t menu_show_flag;
extern uint8_t content_show_flag;
extern uint8_t left_shift_flag;
extern uint8_t right_shift_flag;
extern uint8_t led_state_flag;
extern uint32_t wooden_fish_num;

void Boot_Task(void);
void Menu_Task(void);
#endif 
