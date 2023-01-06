/**
 * @file ina219.cpp
 * @author Krzysztof Półchłopek
 * @author Michał Tomacha
 * @brief 
 * @version 0.1
 * @date 2022-12-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "ina219.hpp"

namespace energymeter
{
    INA219::INA219(uint8_t i2caddr)
    {
        i2c_address = i2caddr;
    }

    void INA219::reset()
    {
        writeInaReg(INA219_REG_CONFIG, INA219_CONFIG_RESET);
    }

    float INA219::getBusVoltage_V()
    {
        return (float) (readInaReg(INA219_REG_BUS_VOLTAGE) >> 3) * 0.004;
    }

    float INA219::getShuntVoltage_mV()
    {
        return (float) readInaReg(INA219_REG_SHUNT_VOLTAGE) * 0.01;
    }

    float INA219::getCurrent_mA()
    {
        return (float) (readInaReg(INA219_REG_CURRENT) / currentDivider_mA);
    }

    float INA219::getPower_mW()
    {
        return (float) (readInaReg(INA219_REG_POWER) * powerMultiplier_mW);
    }

    void INA219::configBusVolRange(Bus_Volt_Range_t range)
    {
        uint16_t reg;
        reg = readInaReg(INA219_REG_CONFIG);
        reg &= ~INA219_CONFIG_BRNG_MASK;
        reg |= range;
        writeInaReg(INA219_REG_CONFIG, reg);
    }

    void INA219::configPGA(PGA_Range_t range)
    {
        uint16_t reg;
        reg = readInaReg(INA219_REG_CONFIG);
        reg &= ~INA219_CONFIG_PG_MASK;
        reg |= range;
        writeInaReg(INA219_REG_CONFIG, reg);
    }

    void INA219::configBusADC(BADC_Resolution_Average_t resavg)
    {
        uint16_t reg;
        reg = readInaReg(INA219_REG_CONFIG);
        reg &= ~INA219_CONFIG_BADC_MASK;
        reg |= resavg;
        writeInaReg(INA219_REG_CONFIG, reg);
    }

    void INA219::configShuntADC(SADC_Resolution_Average_t resavg)
    {
        uint16_t reg;
        reg = readInaReg(INA219_REG_CONFIG);
        reg &= ~INA219_CONFIG_SADC_MASK;
        reg |= resavg;
        writeInaReg(INA219_REG_CONFIG, reg);
    }

    void INA219::configOperatingMode(Operating_Mode_t mode)
    {
        uint16_t reg;
        reg = readInaReg(INA219_REG_CONFIG);
        reg &= ~INA219_CONFIG_MODE_MASK;
        reg |= mode;
        writeInaReg(INA219_REG_CONFIG, reg);
    }

    bool INA219::calibrate(float max_expected_current)
    {       
        float MinimumLSB = max_expected_current/32767;
        float MaximumLSB = max_expected_current/4096;
        
        float CurrentLSB = 0.0;
        float calValueF;
        
        do {
            CurrentLSB += MinimumLSB;
            calValueF = (0.04096 / (CurrentLSB * INA219_SHUNT_RESISTANCE));
        } while ((CurrentLSB < MaximumLSB) && (calValueF > 65536));

        if(calValueF >= 65536)
        {
            return false;
        }

        calValue = (uint16_t)calValueF;

        writeInaReg(INA219_REG_CALIBRATION, calValue);
        
        float PowerLSB = 20 * CurrentLSB;
        
        currentDivider_mA = 0.001 / CurrentLSB; 
        powerMultiplier_mW = PowerLSB / 0.001;

        return true;
    }

    uint16_t INA219::readInaReg(uint8_t reg)
    {
        uint8_t buf[2] = {0};
        readReg(reg, buf, sizeof(buf));
        return (buf[0] << 8) | buf[1];
    }

    void INA219::writeInaReg(uint8_t reg, uint16_t value)
    {
        uint8_t buffer[2] = {(uint8_t)(value >> 8), (uint8_t)(value & 0x00ff)};
        writeReg(reg, buffer, 2);
    }
}