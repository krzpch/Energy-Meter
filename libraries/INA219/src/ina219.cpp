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
        return (float) readInaReg(INA219_REG_CURRENT);
    }

    float INA219::getPower_mW()
    {
        return (float) readInaReg(INA219_REG_POWER) * 20;
    }

    void INA219::configBusVolRange(Bus_Volt_Range_t value)
    {
        uint16_t conf;
        conf = readInaReg(INA219_REG_CONFIG);
        conf &= ~((uint16_t) 1 << 13);
        conf |= (uint16_t) value << 13;
        writeInaReg(INA219_REG_CONFIG, conf);
    }

    void INA219::configPGA(PGA_Range_t range)
    {
        uint16_t conf;
        conf = readInaReg(INA219_REG_CONFIG);
        conf &= ~((uint16_t) 0x03 << 11);
        conf |= (uint16_t) range << 11;
        writeInaReg(INA219_REG_CONFIG, conf);
    }

    void INA219::configBusADC(ADC_Resolution_t res, ADC_Average_t avg)
    {
        uint16_t conf;
        uint16_t value = 0;
        if(res < ADC_Res_12bit && avg > ADC_Ave_1)
        return;
        if(res < ADC_Res_12bit)
        {
            value = res;
        }
        else
        {
            value = 0x08 | avg;
        }
        conf = readInaReg(INA219_REG_CONFIG);
        conf &= ~((uint16_t) 0x0f << 7);
        conf |= (uint16_t) value << 7;
        writeInaReg(INA219_REG_CONFIG, conf);
    }

    void INA219::configShuntADC(ADC_Resolution_t res, ADC_Average_t avg)
    {
        uint16_t conf;
        uint16_t value = 0;
        if(res < ADC_Res_12bit && avg > ADC_Ave_1)
        return;
        if(res < ADC_Res_12bit)
        {
            value = res;
        }
        else
        {
            value = 0x08 | avg;
        }
        conf = readInaReg(INA219_REG_CONFIG);
        conf &= ~((uint16_t) 0x0f << 3);
        conf |= (uint16_t) value << 3;
        writeInaReg(INA219_REG_CONFIG, conf);
    }

    void INA219::configOperatingMode(Operating_Mode_t mode)
    {
        uint16_t conf;
        conf = readInaReg(INA219_REG_CONFIG);
        conf &= ~((uint16_t) 0x07);
        conf |= (uint16_t) mode;
        writeInaReg(INA219_REG_CONFIG, conf);
    }

    void INA219::calibrate(float max_current, float shunt_resistance)
    {
        uint16_t cal;

        cal = (uint16_t) (0.04096/(shunt_resistance * (max_current / 32768)));

        writeInaReg(INA219_REG_CALIBRATION, cal);
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