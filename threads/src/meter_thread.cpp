/**
 * @file meter_thread.cpp
 * @author Krzysztof Półchłopek
 * @author Michał Tomacha
 * @brief 
 * @version 0.1
 * @date 2023-01-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "meter_thread.hpp"

#include "bsp.hpp"
#include "cmsis_os.h"
#include "FreeRTOS.h"

extern osMessageQueueId_t message_queue_display;
extern osMessageQueueId_t message_queue_sd_card;

ina::ina(uint8_t _i2caddr) : INA219(_i2caddr)
{
}

ina::~ina()
{
}

float ina::calculatePower(float voltage, float current)
{
	return voltage*current;
}

void ina::read_measurements(void)
{
	last_voltage = getBusVoltage_V();
	last_current = (getShuntVoltage_mV()/0.01f)/1000.0f;
	last_power = calculatePower(last_voltage, last_current);
}

float ina::get_voltage(void)
{
	return last_voltage;
}
float ina::get_current(void)
{
	return last_current;
}
float ina::get_power(void)
{
	return last_power;
}

// --------------------------------------------------------------------------------
void meter_thread(void *argument)
{
    osStatus_t status;
    energy_data_t msg;

    ina energy(INA219_I2C_ADDRESS6);

    energy.reset();
	energy.calibrate(1.0f);

    bsp::delay(100);

    while(1)
    {
        // osKernelLock();
        energy.read_measurements();
        // osKernelUnlock();
        msg.voltage = energy.get_voltage();
        msg.current = energy.get_current();
        msg.power = energy.get_power();
        msg.timestamp = 0;
        osMessageQueuePut(message_queue_display, &msg, 0U, 0U);
        osMessageQueuePut(message_queue_sd_card, &msg, 0U, 0U);
        osDelay(100);
    }
}