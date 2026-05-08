/**
  ******************************************************************************
  * @file       bsp_gpio_key.c
  * @author     embedfire
  * @version     V1.0
  * @date        2024
  * @brief      扫描按键函数接口
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

#include "key/bsp_gpio_key.h"
#include "delay/bsp_delay.h"
#include "systick/bsp_systick.h"

KEY_Info key1_info  = {KEY1_GPIO_PORT,KEY1_GPIO_PIN,KEY_GENERAL_TRIGGER,KEY_INIT,0,0,EVENT_ATTONITY,KEY_NONE_CLICK};
KEY_Info key2_info  = {KEY2_GPIO_PORT,KEY2_GPIO_PIN,KEY_GENERAL_TRIGGER,KEY_INIT,0,0,EVENT_ATTONITY,KEY_NONE_CLICK};

/**
  * @brief  配置 KEY 中断配置
  * @param  无
  * @retval 无
  */
void KEY_NVIC_Config(void)
{
    /* 定义一个 NVIC 结构体 */
    NVIC_InitTypeDef nvic_initstruct = {0};
    
    /* 开启 AFIO 相关的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
    
#if 1    
    /* 配置中断源 */
    nvic_initstruct.NVIC_IRQChannel                     = KEY1_EXTI_IRQ;
    /* 配置抢占优先级 */
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority   =  1;
    /* 配置子优先级 */
    nvic_initstruct.NVIC_IRQChannelSubPriority          =  0;
    /* 使能配置中断通道 */
    nvic_initstruct.NVIC_IRQChannelCmd                  =  ENABLE;

    NVIC_Init(&nvic_initstruct);
#endif
    
#if 1    
    /* 配置中断源 */
    nvic_initstruct.NVIC_IRQChannel                     = KEY2_EXTI_IRQ;
    /* 配置抢占优先级 */
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority   =  1;
    /* 配置子优先级 */
    nvic_initstruct.NVIC_IRQChannelSubPriority          =  0;
    /* 使能配置中断通道 */
    nvic_initstruct.NVIC_IRQChannelCmd                  =  ENABLE;

    NVIC_Init(&nvic_initstruct);
#endif

}


/**
  * @brief  初始化控制 KEY 的IO
  * @param  无
  * @retval 无
  */
void KEY_GPIO_Config(void)
{
    /* 定义一个 GPIO 结构体 */
    GPIO_InitTypeDef gpio_initstruct = {0};
    
/**************************** 核心板载按键 *****************************/   
#if 1    
    
    /* 开启 KEY 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK_PORT,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_SetBits(KEY1_GPIO_PORT,KEY1_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 浮空输入、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Pin    = KEY1_GPIO_PIN;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IN_FLOATING;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(KEY1_GPIO_PORT,&gpio_initstruct);
   
#endif 
    
#if 1   
    
    /* 开启 KEY 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(KEY2_GPIO_CLK_PORT,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_SetBits(KEY2_GPIO_PORT,KEY2_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 浮空输入、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Pin    = KEY2_GPIO_PIN;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IN_FLOATING;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(KEY2_GPIO_PORT,&gpio_initstruct);
   
#endif 

/**************************** 用户自定义扩展按键 *****************************/

#if 1    
    
    /* 开启 KEY 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(KEY3_GPIO_CLK_PORT,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_SetBits(KEY3_GPIO_PORT,KEY3_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 下拉输入、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Pin    = KEY3_GPIO_PIN;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IPD;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(KEY3_GPIO_PORT,&gpio_initstruct);
   
#endif 

#if 0    
    
    /* 开启 KEY 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(KEY4_GPIO_CLK_PORT,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_SetBits(KEY4_GPIO_PORT,KEY4_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 上拉输入、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Pin    = KEY4_GPIO_PIN;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(KEY4_GPIO_PORT,&gpio_initstruct);
   
#endif 

#if 0    
    
    /* 开启 KEY 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(KEY5_GPIO_CLK_PORT,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_SetBits(KEY5_GPIO_PORT,KEY5_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 上拉输入、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Pin    = KEY5_GPIO_PIN;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(KEY5_GPIO_PORT,&gpio_initstruct);
   
#endif 

#if 0    
    
    /* 开启 KEY 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(KEY6_GPIO_CLK_PORT,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_SetBits(KEY6_GPIO_PORT,KEY6_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 上拉输入、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Pin    = KEY6_GPIO_PIN;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(KEY6_GPIO_PORT,&gpio_initstruct);
   
#endif 
}

/**
  * @brief  配置 KEY 模式
  * @param  无
  * @retval 无
  */
void KEY_Mode_Config(void)
{
    /* 定义一个 EXTI 结构体 */
    EXTI_InitTypeDef exti_initstruct = {0};
   
    /* 开启 AFIO 相关的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
    
#if 1 
    
	/* 选择中断信号源*/
    GPIO_EXTILineConfig(KEY1_EXTI_PORTSOURCE,KEY1_EXTI_PINSOURCE);
    
    /* 选择中断LINE */
    exti_initstruct.EXTI_Line       = EXTI_Line0;
    /* 选择中断模式*/
    exti_initstruct.EXTI_Mode       = EXTI_Mode_Interrupt;
    /* 选择触发方式*/
    exti_initstruct.EXTI_Trigger    = EXTI_Trigger_Falling;
    /* 使能中断*/
    exti_initstruct.EXTI_LineCmd    = ENABLE;
    
    EXTI_Init(&exti_initstruct);
#endif
    
#if 1 

	/* 选择中断信号源*/
    GPIO_EXTILineConfig(KEY2_EXTI_PORTSOURCE,KEY2_EXTI_PINSOURCE);
    
    /* 选择中断LINE */
    exti_initstruct.EXTI_Line       = EXTI_Line13;
    /* 选择中断模式*/
    exti_initstruct.EXTI_Mode       = EXTI_Mode_Interrupt;
    /* 选择触发方式*/
    exti_initstruct.EXTI_Trigger    = EXTI_Trigger_Falling;
    /* 使能中断*/
    exti_initstruct.EXTI_LineCmd    = ENABLE;
    
    EXTI_Init(&exti_initstruct);
#endif

}


/**
  * @brief  KEY 初始化
  * @param  无
  * @retval 无
  */
void KEY_Init(void)
{
    /* 配置 KEY 中断配置 */
    KEY_NVIC_Config();
    
    /* 对应的 GPIO 的配置 */
    KEY_GPIO_Config();
    
    /* 配置 KEY 模式 */
    KEY_Mode_Config();
    
    /* 配置 KEY 初始电平 */
    KeyLevel_Init(&key1_info);
    KeyLevel_Init(&key2_info);
    
}

/**
  * @brief  基础检测按键
  * @param  GPIOx：x 可以是 A，B，C等
  * @param  GPIO_Pin：待操作的pin脚号
  * @param  key_pressstatus：按键按下时的IO电平状态
  * @retval KEY_UP(没有触发按键)、KEY_DOWN(触发按键)
  */
KEY_Status KEY_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, KEY_TriggerLevel key_pressstatus)
{
    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == key_pressstatus)
    {
        Rough_Delay_Ms(20);
        while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == key_pressstatus);
        Rough_Delay_Ms(20);
        return KEY_DOWN;
    }
    else
    {
        return KEY_UP;
    }
}

/**
  * @brief  获取当前操作按键的电平
  * @param  key_info： 操作按键的信息结构体
  * @note   无
  * @retval 当前操作按键的电平
  */
static uint8_t Get_KeyCurrentLevel(KEY_Info* key_info)
{
    return GPIO_ReadInputDataBit(key_info->GPIOx,key_info->GPIO_Pin);
}

/**
  * @brief  获取当前操作按键空闲/抬起后的电平
  * @param  key_info： 操作按键的信息结构体
  * @note   无
  * @retval 无
  */
void KeyLevel_Init(KEY_Info* key_info)
{
    if(key_info->triggerlevel == KEY_GENERAL_TRIGGER)
    {
        /* 上电后程序启动短时间内默认按键未按，那么根据当前未按下电平，反推出按下的电平 */
        key_info->triggerlevel = Get_KeyCurrentLevel(key_info)? KEY_LOW_TRIGGER:KEY_HIGH_TRIGGER;
    }

}

/**
  * @brief  使用定时器扫描方式获取当前操作按键的事件类型
  * @param  key_info： 操作按键的信息结构体
  * @note   无
  * @retval 按键的事件类型
  */
KEY_Event KEY_SystickScan(KEY_Info* key_info)
{
    uint64_t time_time = 0;
    if(Get_KeyCurrentLevel(key_info) ==  key_info->triggerlevel)
    {
        key_info->status = KEY_DOWN;
    }
    else
    {
        key_info->status = KEY_UP;
    }
    switch(key_info->event)
    {
        case EVENT_ATTONITY :   //上一个事件处于 空闲/无操作 时
            if(key_info->status == KEY_DOWN)
            {
                key_info->event = EVENT_PRESS;
                key_info->press_time = SysTick_GetCount();
            }
            break;
            
        case EVENT_PRESS:       //上一个事件处于 按下 时
            
            if(key_info->status == KEY_DOWN)
            {
                time_time = SysTick_GetCount();  
                if(time_time - key_info->press_time >1000)  //按下时间超过自定义长按时间 还继续按下时属于 长按
                {
                    key_info->event = EVENT_LONG;           //进入 长按 事件
                } 
            }
            else
            {
                key_info->event = EVENT_SHORT;              //进入 短按 事件 
            }
            break;
            
        case EVENT_SHORT:
            key_info->event = EVENT_SHORT_RELEASE;          //上一轮返回了EVENT_SHORT，无进阶事件，重回EVENT_ATTONITY
//            key_info->clicktype = KEY_SINGLE_CLICK;
        break;
        
        case EVENT_LONG:
            if(key_info->status == KEY_DOWN)
            {
                key_info->event = EVENT_LONG;           //保持 长按 事件
            }
            else
            {
                key_info->event = EVENT_LONG_RELEASE;      //上一轮返回了EVENT_LONG，无进阶事件，重回EVENT_ATTONITY
//                key_info->clicktype = KEY_LONG_CLICK;           
            }
            break;
        
        default :
            key_info->event = EVENT_ATTONITY;
            break;        
    }
    return key_info->event;
}

/*****************************END OF FILE***************************************/
