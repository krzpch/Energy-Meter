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
extern bool save_measurements;

bool csv_error;

typedef enum {
    Csv_uninitialized,
    Csv_idle,
    Csv_create_file,
    Csv_save_measurement,
    Csv_close_file,
    Csv_error
} Csv_thread_state_t;

void csv_thread(void *argument)
{
    energy_data_t msg = {0};
    Csv_thread_state_t state = Csv_uninitialized;
    energymeter::Csv csv;
    csv_error = false;
    
    while(true)
    {
        switch(state)
        {
        case Csv_uninitialized:
            state = csv.begin() ? Csv_idle : Csv_error;
            break;
        case Csv_create_file:
            state = csv.create_new_file() ? Csv_save_measurement : Csv_error;
            break;
        case Csv_save_measurement:
            if(save_measurements)
            {   
                if (osMessageQueueGetCount(message_queue_sd_card) > 0)
                {
                    if (osMessageQueueGet(message_queue_sd_card, &msg, NULL, 100U) == osOK) 
                    {
                        state = csv.append_measurement(msg.timestamp, msg.voltage, msg.current, msg.power) ? Csv_save_measurement : Csv_error;
                    }
                }
                osDelay(100);
            }
            else
            {
                state = Csv_close_file;
            }
            break;   
        case Csv_close_file:
            state = csv.close_file() ? Csv_idle : Csv_error;
            break;
        case Csv_error:
            csv_error = true;
            csv.end();
            osDelay(100);
            if(csv.begin())
            {
                state = Csv_idle;
                csv_error = false;
            }
            break;
        case Csv_idle:
        default:
            if(save_measurements)
            {
                state = Csv_create_file;
            }
            else
            {
                osDelay(100);
            }
            break;
        }
    }
}
