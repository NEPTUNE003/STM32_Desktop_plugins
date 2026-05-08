#ifndef __APP_DHT11_H
#define __APP_DHT11_H

#include "stm32f10x.h"//或#include "stdint.h"

#include "dht11/bsp_dht11.h" 

typedef struct
{
    uint32_t cycle;
    uint32_t timer;
    uint8_t  flag;
}Dht11_TaskInfo;

extern Dht11_TaskInfo dht11_rd_task;
extern DHT11_DATA_TYPEDEF dht11_data;

void Dht11_TaskReset(void);
void Dht11_TaskInit(uint32_t dht11_rd_task_cycle);
void Dht11_Task(void);
#endif /* __APP_DHT11_H  */


