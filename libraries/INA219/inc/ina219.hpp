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

#define INA219_SHUNT_RESISTANCE     (0.01) // shunt resistance in ohms

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

#define INA219_CONFIG_BRNG_MASK     (0x2000) // Bus Voltage Range Mask
#define INA219_CONFIG_PG_MASK       (0x1800) // Bus Voltage Range Mask
#define INA219_CONFIG_BADC_MASK     (0x0780) // Bus Voltage Range Mask
#define INA219_CONFIG_SADC_MASK     (0x0078) // Bus Voltage Range Mask
#define INA219_CONFIG_MODE_MASK     (0x0007) // Bus Voltage Range Mask

typedef enum {
    Bus_Volt_Range_16V = 0x0000,     // 16V FSR
    Bus_Volt_Range_32V = 0x2000      // 32V FSR
} Bus_Volt_Range_t;

typedef enum {
    PGA_Range_40mV = 0x0000,         // GAIN: 1, Range ±40 mV
    PGA_Range_80mV = 0x0800,         // GAIN: /2, Range ±80 mV
    PGA_Range_160mV = 0x1000,        // GAIN: /4, Range ±160 mV
    PGA_Range_320mV = 0x1800         // GAIN: /8, Range ±320 mV
} PGA_Range_t;

typedef enum {
    BADC_Res_9bit_Ave_1 = 0x0000,
    BADC_Res_10bit_Ave_1 = 0x0080,
    BADC_Res_11bit_Ave_1 = 0x0100,
    BADC_Res_12bit_Ave_1 = 0x0180,
    BADC_Res_12bit_Ave_2 = 0x0480,
    BADC_Res_12bit_Ave_4 = 0x0500,
    BADC_Res_12bit_Ave_8 = 0x0580,
    BADC_Res_12bit_Ave_16 = 0x0600,
    BADC_Res_12bit_Ave_32 = 0x0680,
    BADC_Res_12bit_Ave_64 = 0x0700,
    BADC_Res_12bit_Ave_128 = 0x0780
} BADC_Resolution_Average_t;

typedef enum {
    SADC_Res_9bit_Ave_1 = 0x0000,
    SADC_Res_10bit_Ave_1 = 0x0008,
    SADC_Res_11bit_Ave_1 = 0x0010,
    SADC_Res_12bit_Ave_1 = 0x0018,
    SADC_Res_12bit_Ave_2 = 0x0048,
    SADC_Res_12bit_Ave_4 = 0x0050,
    SADC_Res_12bit_Ave_8 = 0x0058,
    SADC_Res_12bit_Ave_16 = 0x0060,
    SADC_Res_12bit_Ave_32 = 0x0068,
    SADC_Res_12bit_Ave_64 = 0x0070,
    SADC_Res_12bit_Ave_128 = 0x0078
} SADC_Resolution_Average_t;

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

namespace energymeter
{
    class INA219
    {
    private:
        uint16_t calValue;
        float currentDivider_mA;
        float powerMultiplier_mW;
    public:
        INA219(uint8_t i2caddr);
        void reset();
        float getBusVoltage_V();
        float getShuntVoltage_mV();
        float getCurrent_mA();
        float getPower_mW();
        void configBusVolRange(Bus_Volt_Range_t value);
        void configPGA(PGA_Range_t range);
        void configBusADC(BADC_Resolution_Average_t resavg);
        void configShuntADC(SADC_Resolution_Average_t resavg);
        void configOperatingMode(Operating_Mode_t mode);

        /**
         * @brief Function for calibrating the sensor
         * 
         * @param max_expected_current max value of expected current current in A
         * @return true if callibration was succesfull, false otherwise
         * @note max_expected_current 0.3 is minimum known good value
         */
        bool calibrate(float max_expected_current);

    protected:
        uint16_t i2c_address;
        virtual void writeReg(uint8_t reg, uint8_t *pBuf, uint16_t len) = 0;
        virtual void readReg(uint8_t reg, uint8_t *pBuf, uint16_t len) = 0; 
        uint16_t readInaReg(uint8_t reg);
        void writeInaReg(uint8_t reg, uint16_t value);   
    };
}

#endif // INA219_HPP