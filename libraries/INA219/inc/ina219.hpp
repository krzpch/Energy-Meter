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

#ifndef INA219_HPP
#define INA219_HPP

#include <stdint.h>

#define INA219_I2C_ADDRESS1         (0x40) // I2C ADDRESS 1
#define INA219_I2C_ADDRESS2         (0x41) // I2C ADDRESS 2
#define INA219_I2C_ADDRESS3         (0x42) // I2C ADDRESS 3
#define INA219_I2C_ADDRESS4         (0x43) // I2C ADDRESS 4
#define INA219_I2C_ADDRESS5         (0x44) // I2C ADDRESS 5
#define INA219_I2C_ADDRESS6         (0x45) // I2C ADDRESS 6
#define INA219_I2C_ADDRESS7         (0x46) // I2C ADDRESS 7
#define INA219_I2C_ADDRESS8         (0x47) // I2C ADDRESS 8
#define INA219_I2C_ADDRESS9         (0x48) // I2C ADDRESS 9
#define INA219_I2C_ADDRESS10        (0x49) // I2C ADDRESS 10
#define INA219_I2C_ADDRESS11        (0x4A) // I2C ADDRESS 11
#define INA219_I2C_ADDRESS12        (0x4B) // I2C ADDRESS 12
#define INA219_I2C_ADDRESS13        (0x4C) // I2C ADDRESS 13
#define INA219_I2C_ADDRESS14        (0x4D) // I2C ADDRESS 14
#define INA219_I2C_ADDRESS15        (0x4E) // I2C ADDRESS 15
#define INA219_I2C_ADDRESS16        (0x4F) // I2C ADDRESS 16

#define INA219_REG_CONFIG           (0x00) // Config register
#define INA219_REG_SHUNT_VOLTAGE    (0x01) // Shunt Voltage Register
#define INA219_REG_BUS_VOLTAGE      (0x02) // Bus Voltage Register
#define INA219_REG_POWER            (0x03) // Power Register
#define INA219_REG_CURRENT          (0x04) // Current Register
#define INA219_REG_CALIBRATION      (0x05) // Register Calibration

#define INA219_CONFIG_RESET         (0x8000) // Config reset register

namespace energymeter
{
    class INA219
    {
    public:
        typedef enum {
            Bus_Volt_Range_16V,     // 16V FSR
            Bus_Volt_Range_32V      // 32V FSR
        } Bus_Volt_Range_t;

        typedef enum {
            PGA_Range_40mV,         // GAIN: 1, Range ±40 mV
            PGA_Range_80mV,         // GAIN: /2, Range ±80 mV
            PGA_Range_160mV,        // GAIN: /4, Range ±160 mV
            PGA_Range_320mV         // GAIN: /8, Range ±320 mV
        } PGA_Range_t;

        typedef enum {
            ADC_Res_9bit,
            ADC_Res_10bit,
            ADC_Res_11bit,
            ADC_Res_12bit,
        } ADC_Resolution_t;

        typedef enum {
            ADC_Ave_1,
            ADC_Ave_2,
            ADC_Ave_4,
            ADC_Ave_8,
            ADC_Ave_16,
            ADC_Ave_32,
            ADC_Ave_64,
            ADC_Ave_128
        } ADC_Average_t;

        typedef enum {
            PowerDown,              // Power-down
            ShuntVoltTrig,          // Shunt voltage, triggered
            BusVoltTrig,            // Bus voltage, triggered
            ShutAndBbusVoltTrig,    // Shunt and bus, triggered
            AdcOff,                 // ADC off (disabled)
            ShuntVoltCont,          // Shunt voltage, continuous
            BusVoltCont,            // Bus voltage, continuous
            ShutAndBusVoltCont      // Shunt and bus, continuous
        } Operating_Mode_t;

    public:
        INA219(uint8_t i2caddr);
        void reset();
        float getBusVoltage_V();
        float getShuntVoltage_mV();
        float getCurrent_mA();
        float getPower_mW();
        void configBusVolRange(Bus_Volt_Range_t value);
        void configPGA(PGA_Range_t range);
        void configBusADC(ADC_Resolution_t res, ADC_Average_t avg);
        void configShuntADC(ADC_Resolution_t res, ADC_Average_t avg);
        void configOperatingMode(Operating_Mode_t mode);
        void calibrate(float max_current, float shunt_resistance);

    protected:
        uint16_t i2c_address;
        virtual void writeReg(uint8_t reg, uint8_t *pBuf, uint16_t len) = 0;
        virtual void readReg(uint8_t reg, uint8_t *pBuf, uint16_t len) = 0; 
        uint16_t readInaReg(uint8_t reg);
        void writeInaReg(uint8_t reg, uint16_t value);   
    };
}

#endif // INA219_HPP