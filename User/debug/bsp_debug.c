/**
  ******************************************************************************
  * @file       bsp_debug.c
  * @author     embedfire
  * @version     V1.0
  * @date        2024
  * @brief      调试串口函数接口
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
  
#include "debug/bsp_debug.h"
#include "usart/usart_com.h"

DEBUG_DataTypeDef debug_receive = {0};

/**
  * @brief  配置 DEBUG 串口中断配置
  * @param  无
  * @retval 无
  */
void DEBUG_NVIC_Config(void)
{
    /* 定义一个 NVIC 结构体 */
    NVIC_InitTypeDef nvic_initstruct = {0};
    
    /* 开启 AFIO 相关的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
      
    /* 配置中断源 */
    nvic_initstruct.NVIC_IRQChannel                     = DEBUG_IRQ;
    /* 配置抢占优先级 */
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority   =  1;
    /* 配置子优先级 */
    nvic_initstruct.NVIC_IRQChannelSubPriority          =  0;
    /* 使能配置中断通道 */
    nvic_initstruct.NVIC_IRQChannelCmd                  =  ENABLE;

    NVIC_Init(&nvic_initstruct);
    
}

/**
 * @brief  初始化控制 DEBUG 串口 的IO
 * @param  无
 * @retval 无
 */
void DEBUG_USART_PinConfig(void)
{
    
    /* 定义一个 GPIO 结构体 */
    GPIO_InitTypeDef gpio_initstruct = {0};
    
#if 1  
    
    /* 开启 DEBUG 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(DEBUG_TX_GPIO_CLK_PORT,ENABLE);

    /*选择要控制的GPIO引脚、设置GPIO模式为 推挽复用、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_AF_PP;
    gpio_initstruct.GPIO_Pin    = DEBUG_TX_GPIO_PIN;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(DEBUG_TX_GPIO_PORT,&gpio_initstruct); 
    
#endif 
    
#if 1    
    
    /* 开启 DEBUG 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(DEBUG_RX_GPIO_CLK_PORT,ENABLE);

    /*选择要控制的GPIO引脚、设置GPIO模式为 上拉输入/浮空输入、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Pin    = DEBUG_RX_GPIO_PIN;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(DEBUG_RX_GPIO_PORT,&gpio_initstruct); 
    
#endif  
}

/**
 * @brief  配置 DEBUG 串口 模式
 * @param  无
 * @retval 无
 */
void DEBUG_USART_ModeConfig(void)
{
  
    /* 定义一个 USART 结构体 */
    USART_InitTypeDef usart_initstruct = {0};
    
    /* 开启 DEBUG 相关的GPIO外设/端口时钟 */
    DEBUG_APBXCLKCMD(DEBUG_USARTX_CLK_PORT,ENABLE);

    /* 配置串口的工作参数 */
    usart_initstruct.USART_BaudRate                 =  DEBUG_BAUDRATE;                  //配置波特率
    usart_initstruct.USART_HardwareFlowControl      =  USART_HardwareFlowControl_None;  //配置硬件流控制
    usart_initstruct.USART_Mode                     =  USART_Mode_Tx|USART_Mode_Rx;     //配置工作模式
    usart_initstruct.USART_Parity                   =  USART_Parity_No;                 //配置校验位    
    usart_initstruct.USART_StopBits                 =  USART_StopBits_1;                //配置停止位
    usart_initstruct.USART_WordLength               =  USART_WordLength_8b;             //配置帧数据字长
    
    USART_Init(DEBUG_USARTX, &usart_initstruct);
    
    USART_ITConfig(DEBUG_USARTX,USART_IT_RXNE,ENABLE);//开启串口数据接收中断
    USART_ITConfig(DEBUG_USARTX,USART_IT_IDLE,ENABLE);//开启串口数据空闲中断
    
}

/**
 * @brief  DEBUG 串口 初始化
 * @param  无
 * @retval 无
 */
void DEBUG_USART_Init(void)
{
    /* 配置 DEBUG 串口中断配置 */
    DEBUG_NVIC_Config();
    
    /* 配置 USARTX 模式 */
    DEBUG_USART_ModeConfig();

    /* 对应的 GPIO 的配置 */
    DEBUG_USART_PinConfig();
    
    /* 使能串口 */
    USART_Cmd(DEBUG_USARTX,ENABLE);

}

/*

1.因为数组下标从0开始，定义buffer[DEBUG_BUFFER_SIZE]时，可取的末尾是buffer[DEBUG_BUFFER_SIZE-1] 

  0   1   2   3  4  5  6  ……   DEBUG_BUFFER_SIZE-3   DEBUG_BUFFER_SIZE-2    DEBUG_BUFFER_SIZE-1

2.因为数组下标从0开始，当接收了len个，buffer[len-1]为最近一次保存位置，buffer[len]指向的是下一个准备接收空位，所以用len与DEBUG_BUFFER_SIZE-1比较。

3.此串口驱动需求为收发字符形式，所以为了简化处理作以下规定：
   在BUFFER末尾固定预留'\0'（为了方便直接将BUFFER给string系列函数使用。不会把'\0'算进len计数，其他字符都算进len计数）如果刚好接满时就如下面所示。

*/

/**
  * @brief  DEBUG 串口 中断回调函数
  * @param  无
  * @retval 无
  */
void DEBUG_IRQHANDLER(void)
{
    uint8_t data_temp = NULL;
    if(USART_GetITStatus(DEBUG_USARTX, USART_IT_RXNE) == SET)  
    {
        data_temp = USART_ReceiveData(DEBUG_USARTX);                                    //读取数据寄存器的数据，读取后对应的寄存器会被复位
        
        if((debug_receive.len < DEBUG_BUFFER_SIZE-1) && debug_receive.read_flag == 0)   //未接收满且程序不正在读取缓冲区，才把数据添加进缓冲区
        {
            debug_receive.buffer[debug_receive.len] = data_temp;
            debug_receive.len++;
        }
        if(debug_receive.len == DEBUG_BUFFER_SIZE-1)                    //如果接满数据包结束
        {
            debug_receive.buffer[debug_receive.len] = '\0';             //插入字符串结尾标志
            debug_receive.read_flag = 1;                                //正在读取数据标志置1，暂不接收
        }
        USART_ClearITPendingBit(DEBUG_USARTX,USART_IT_RXNE);            //清除接收标志位
    }
    if(USART_GetITStatus(DEBUG_USARTX, USART_IT_IDLE) == SET)  
    {
        USART_ReceiveData(DEBUG_USARTX);                                //特别提示，根据手册实际描述（读取获取USART_IT_IDLE、数据寄存器的数据，读取后对应的寄存器会被复位），用这样方式清除IDLE
        debug_receive.buffer[debug_receive.len] = '\0';                 //插入字符串结尾标志
        debug_receive.read_flag = 1;                                    //正在读取数据标志置1，暂不接收
    }
    
}

/*****************************END OF FILE***************************************/

