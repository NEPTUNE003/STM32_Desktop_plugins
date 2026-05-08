#ifndef __BSP_GPIO_BEEP_H
#define __BSP_GPIO_BEEP_H

#include "stm32f10x.h"

/* 定义 BEEP 连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的 BEEP 引脚 */

//BEEP
#define BEEP_GPIO_PORT          GPIOA                           /* GPIO端口 */
#define BEEP_GPIO_CLK_PORT      RCC_APB2Periph_GPIOA            /* GPIO端口时钟 */
#define BEEP_GPIO_PIN           GPIO_Pin_6                      /* 对应PIN脚 */


/* 蜂鸣器响时的IO电平 */
typedef enum 
{
    BEEP_LOW_TRIGGER = 0, 
    BEEP_HIGH_TRIGGER = 1,
}BEEP_TriggerLevel;

void BEEP_GPIO_Config(void);
void BEEP_ON(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BEEP_TriggerLevel beep_soundsstatus);
void BEEP_OFF(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BEEP_TriggerLevel beep_soundsstatus);
void BEEP_TOGGLE(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
#endif /* __BSP_GPIO_BEEP_H */
