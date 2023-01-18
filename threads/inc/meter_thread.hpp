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

typedef struct
{
	float voltage;
	float current;
	float power;
  	uint32_t timestamp;
} energy_data_t;

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

void meter_thread(void *argument);

#endif