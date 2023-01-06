/**
 * @file meter_thread.hpp
 * @author Krzysztof Półchłopek
 * @author Michał Tomacha
 * @brief 
 * @version 0.1
 * @date 2022-12-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef METER_THREAD_H
#define METER_THREAD_H

#include "main.h"
#include "i2c.h"

#include "ina219.hpp"

extern "C" I2C_HandleTypeDef hi2c1;

class ina : public energymeter::INA219
{
private:
	float last_voltage;
	float last_current;
	float last_power;

public:
	ina(uint8_t _i2caddr);
	~ina();

	void read_measurements(void);
	float get_voltage(void);
	float get_current(void);
	float get_power(void);

protected:
	virtual void writeReg(uint8_t reg, uint8_t *pBuf, uint16_t len)
	{
		HAL_I2C_Mem_Write(&hi2c1, this->i2c_address << 1, reg, 1, pBuf, len, 300);
	}

	virtual void readReg(uint8_t reg, uint8_t *pBuf, uint16_t len)
	{
		HAL_I2C_Mem_Read(&hi2c1, this->i2c_address << 1, reg, 1, pBuf, len, 300);
	}

	float calculatePower(float voltage, float current);
};

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

#endif