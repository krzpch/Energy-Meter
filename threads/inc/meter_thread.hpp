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
	/* data */
public:
	ina(uint8_t _i2caddr);
	~ina();

protected:
	virtual void writeReg(uint8_t reg, uint8_t *pBuf, uint16_t len)
	{
		HAL_I2C_Mem_Write(&hi2c1, this->i2c_address << 1, reg, 1, pBuf, len, 300);
	}

	virtual void readReg(uint8_t reg, uint8_t *pBuf, uint16_t len)
	{
		HAL_I2C_Mem_Read(&hi2c1, this->i2c_address << 1, reg, 1, pBuf, len, 300);
	}
};

ina::ina(uint8_t _i2caddr) : INA219(_i2caddr)
{
	
}

ina::~ina()
{
}

#endif