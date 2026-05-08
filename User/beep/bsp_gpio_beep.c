/**
  ******************************************************************************
  * @file       bsp_gpio_beep.c
  * @author     embedfire
  * @version     V1.0
  * @date        2024
  * @brief      BEEP函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台  ：野火 STM32F103C8T6-STM32开发板 
  * 论坛      ：http://www.firebbs.cn
  * 官网      ：https://embedfire.com/
  * 淘宝      ：https://yehuosm.tmall.com/
  *
  ******************************************************************************
  */

#include "beep/bsp_gpio_beep.h"

/**
  * @brief  初始化控制 BEEP 的IO
  * @param  无
  * @retval 无
  */
void BEEP_GPIO_Config(void)
{
    /* 定义一个 GPIO 结构体 */
    GPIO_InitTypeDef gpio_initstruct = {0};
    
#if 1    
    
    /* 开启 BEEP 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK_PORT,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 推挽模式、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Pin    = BEEP_GPIO_PIN;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_Out_PP;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(BEEP_GPIO_PORT,&gpio_initstruct);
   
#endif 
    
}

/**
  * @brief  开启对应 BEEP
  * @param  GPIOx：x 可以是 A，B，C等
  * @param  GPIO_Pin：待操作的pin脚号
  * @param  beep_soundsstatus：LED灯亮时的IO电平状态
  * @retval 无
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
  * @brief  关闭对应 BEEP
  * @param  GPIOx：x 可以是 A，B，C等
  * @param  GPIO_Pin：待操作的pin脚号
  * @param  beep_soundsstatus：蜂鸣器响时的IO电平状态
  * @retval 无
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
  * @brief  翻转对应 BEEP
  * @param  GPIOx：x 可以是 A，B，C等
  * @param  GPIO_Pin：待操作的pin脚号
  * @retval 无
  */
void BEEP_TOGGLE(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin;

}
/*****************************END OF FILE***************************************/
