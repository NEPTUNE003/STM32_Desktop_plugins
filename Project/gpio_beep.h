#ifndef __BSP_GPIO_BEEP_H
#define __BSP_GPIO_BEEP_H

#include "stm32f10x.h"

/* ?? BEEP ???GPIO??, ??????????????????? BEEP ?? */

//BEEP
#define BEEP_GPIO_PORT          GPIOA                           /* GPIO?? */
#define BEEP_GPIO_CLK_PORT      RCC_APB2Periph_GPIOA            /* GPIO???? */
#define BEEP_GPIO_PIN           GPIO_Pin_6                      /* ??PIN? */


/* ??????IO?? */
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
