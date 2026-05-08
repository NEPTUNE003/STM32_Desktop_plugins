/**
  ******************************************************************************
  * @file       bsp_gpio_beep.c
  * @author     embedfire
  * @version     V1.0
  * @date        2024
  * @brief      BEEP????
  ******************************************************************************
  * @attention
  *
  * ????  :?? STM32F103C8T6-STM32??? 
  * ??      :http://www.firebbs.cn
  * ??      :https://embedfire.com/
  * ??      :https://yehuosm.tmall.com/
  *
  ******************************************************************************
  */

#include "gpio_beep.h"

/**
  * @brief  ????? BEEP ?IO
  * @param  ?
  * @retval ?
  */
void BEEP_GPIO_Config(void)
{
    /* ???? GPIO ??? */
    GPIO_InitTypeDef gpio_initstruct = {0};
    
#if 1    
    
    /* ?? BEEP ???GPIO??/???? */
    RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK_PORT,ENABLE);
    
    /* IO????????? */
    GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);
    
    /*??????GPIO?????GPIO??? ???????GPIO???50MHz*/
    gpio_initstruct.GPIO_Pin    = BEEP_GPIO_PIN;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_Out_PP;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(BEEP_GPIO_PORT,&gpio_initstruct);
   
#endif 
    
}

/**
  * @brief  ???? BEEP
  * @param  GPIOx:x ??? A,B,C?
  * @param  GPIO_Pin:????pin??
  * @param  beep_soundsstatus:LED????IO????
  * @retval ?
  */
void BEEP_ON(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BEEP_TriggerLevel beep_soundsstatus)
{
    if(beep_soundsstatus == BEEP_LOW_TRIGGER)
    {
        GPIO_ResetBits(GPIOx,GPIO_Pin);
    }
    else
    {
        GPIO_SetBits(GPIOx,GPIO_Pin);
    }
    
}

/**
  * @brief  ???? BEEP
  * @param  GPIOx:x ??? A,B,C?
  * @param  GPIO_Pin:????pin??
  * @param  beep_soundsstatus:??????IO????
  * @retval ?
  */
void BEEP_OFF(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BEEP_TriggerLevel beep_soundsstatus)
{
    if(beep_soundsstatus == BEEP_LOW_TRIGGER)
    {
        GPIO_SetBits(GPIOx,GPIO_Pin);
    }
    else
    {
        GPIO_ResetBits(GPIOx,GPIO_Pin);
    }
}

/**
  * @brief  ???? BEEP
  * @param  GPIOx:x ??? A,B,C?
  * @param  GPIO_Pin:????pin??
  * @retval ?
  */
void BEEP_TOGGLE(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin;

}
/*****************************END OF FILE***************************************/
