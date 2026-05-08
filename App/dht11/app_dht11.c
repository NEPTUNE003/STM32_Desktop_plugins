#include "dht11/app_dht11.h"
#include "dht11/bsp_dht11.h" 
#include "debug/bsp_debug.h"
#include "oled/app_oled.h"

Dht11_TaskInfo dht11_rd_task = {0};     //周期、定时器和标志位
DHT11_DATA_TYPEDEF dht11_data = {0};    //温湿度数据

void Dht11_TaskInit(uint32_t cycle)
{
    dht11_rd_task.cycle = cycle;
    dht11_rd_task.timer = cycle;
    dht11_rd_task.flag  = 0;                  //1表示需要读取
}

void Dht11_Task(void)
{
    if (dht11_rd_task.timer == 0)
    {
        dht11_rd_task.flag = 1;
    }

    if (dht11_rd_task.flag == 1 && (menu & 0xf0) == 0x20)
    {
        if (DHT11_ReadData(&dht11_data) != SUCCESS)
            printf("READ_DHT11_DATA ERROR!\r\n");

        content_show_flag = 1;
        dht11_rd_task.timer = dht11_rd_task.cycle;
        dht11_rd_task.flag  = 0;           
    }
}