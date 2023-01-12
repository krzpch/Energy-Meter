/**
 * @file csv_thread.cpp
 * @author Krzysztof Półchłopek
 * @author Michał Tomacha
 * @brief 
 * @version 0.1
 * @date 2023-01-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "csv_thread.hpp"
#include "meter_thread.hpp"

#include "bsp.hpp"
#include "main.h"

#include "cmsis_os.h"
#include "FreeRTOS.h"

extern osMessageQueueId_t message_queue_sd_card;

void csv_thread(void *argument)
{
    osStatus_t status;
    energy_data_t msg = {0};
    energymeter::Csv csv;
   
    if(!csv.create_new_file())
    {
        while(1)
        {
            osDelay(10000);
        }
    }

    while(1)
    {
        status = osMessageQueueGet(message_queue_sd_card, &msg, NULL, 100U);
        if (status == osOK) 
        {
            osKernelLock();
            csv.append_measurement(msg.voltage, msg.current, msg.power);
            osKernelUnlock();
        }
        osDelay(100);
    }

    if(!csv.close_file())
    {
        while(1);
    }
}