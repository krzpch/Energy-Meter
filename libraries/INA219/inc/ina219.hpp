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
    BADC_Res_9bit_Ave_1 = 0x0000,    // 9 bit resolution, 1 average
    BADC_Res_10bit_Ave_1 = 0x0080,   // 10 bit resolution, 1 average
    BADC_Res_11bit_Ave_1 = 0x0100,   // 11 bit resolution, 1 average
    BADC_Res_12bit_Ave_1 = 0x0180,   // 12 bit resolution, 1 average
    BADC_Res_12bit_Ave_2 = 0x0480,   // 12 bit resolution, 2 averages
    BADC_Res_12bit_Ave_4 = 0x0500,   // 12 bit resolution, 4 averages
    BADC_Res_12bit_Ave_8 = 0x0580,   // 12 bit resolution, 8 averages
    BADC_Res_12bit_Ave_16 = 0x0600,  // 12 bit resolution, 16 averages
    BADC_Res_12bit_Ave_32 = 0x0680,  // 12 bit resolution, 32 averages
    BADC_Res_12bit_Ave_64 = 0x0700,  // 12 bit resolution, 64 averages
    BADC_Res_12bit_Ave_128 = 0x0780  // 12 bit resolution, 128 averages
} BADC_Resolution_Average_t;

typedef enum {
    SADC_Res_9bit_Ave_1 = 0x0000,    // 9 bit resolution, 1 average
    SADC_Res_10bit_Ave_1 = 0x0008,   // 10 bit resolution, 1 average
    SADC_Res_11bit_Ave_1 = 0x0010,   // 11 bit resolution, 1 average
    SADC_Res_12bit_Ave_1 = 0x0018,   // 12 bit resolution, 1 average
    SADC_Res_12bit_Ave_2 = 0x0048,   // 12 bit resolution, 2 averages
    SADC_Res_12bit_Ave_4 = 0x0050,   // 12 bit resolution, 4 averages
    SADC_Res_12bit_Ave_8 = 0x0058,   // 12 bit resolution, 8 averages
    SADC_Res_12bit_Ave_16 = 0x0060,  // 12 bit resolution, 16 averages
    SADC_Res_12bit_Ave_32 = 0x0068,  // 12 bit resolution, 32 averages
    SADC_Res_12bit_Ave_64 = 0x0070,  // 12 bit resolution, 64 averages
    SADC_Res_12bit_Ave_128 = 0x0078  // 12 bit resolution, 128 averages
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
        uint16_t calValue;          // Calibration value
        float currentDivider_mA;    // Divider used in reading current
        float powerMultiplier_mW;   // Multiplier used in reading power
    public:
        /**
         * @brief Construct a new INA219 object
         * 
         * @param[in] i2caddr I2C address of INA219 sensor
         */
        INA219(uint8_t i2caddr);

        /**
         * @brief Resets sensor to default settings
         */
        void reset(void);

        /**
         * @brief Get measured bus voltage
         * 
         * @return float Value of measured bus voltage in V
         */
        float getBusVoltage_V(void);

        /**
         * @brief Get measured shunt resistor voltage
         * 
         * @return float Value of measured shunt resistor voltage in mV
         */
        float getShuntVoltage_mV(void);

        /**
         * @brief Get measured current
         * 
         * @note Sensor must be first calibrated otherwise 0 is returned
         * 
         * @return float Value of measured current in mA
         */
        float getCurrent_mA(void);

        /**
         * @brief Get measured power
         * 
         * @note Sensor must be first calibrated otherwise 0 is returned
         * 
         * @return float Value of measured power in mW
         */
        float getPower_mW(void);

        /**
         * @brief Set range of bus voltage
         * 
         * @param[in] range selected voltage range
         */
        void configBusVolRange(Bus_Volt_Range_t range);

        /**
         * @brief Set range of programmable gain amplifier
         * 
         * @param[in] range selected voltage range
         */
        void configPGA(PGA_Range_t range);

        /**
         * @brief Set bus ADC resolution and number of averages
         * 
         * @param[in] resavg selected resolution and number of averages
         */
        void configBusADC(BADC_Resolution_Average_t resavg);

        /**
         * @brief Set shunt ADC resolution and number of averages
         * 
         * @param[in] resavg selected resolution and number of averages
         */
        void configShuntADC(SADC_Resolution_Average_t resavg);

        /**
         * @brief Set sensor operating mode
         * 
         * @param[in] mode selected mode
         */
        void configOperatingMode(Operating_Mode_t mode);

        /**
         * @brief Calibrates the sensor
         * 
         * @note max_expected_current 0.3 is minimum known good value
         * 
         * @param [in] max_expected_current maximum expected current to be measured in A
         * @return true if calibration succeded
         * @return false if max_expected_current has invalid value
         */
        bool calibrate(float max_expected_current);

    protected:
        uint16_t i2c_address; // I2C address of INA219 sensor

        /**
         * @brief Pure Virtual Function used to write INA219 register using I2C (hardware dependent)
         * 
         * @param[in] reg register address
         * @param[in] pBuf buffer with data
         * @param[in] len length of data in bytes
         */
        virtual void writeReg(uint8_t reg, uint8_t *pBuf, uint16_t len) = 0;

        /**
         * @brief Pure Virtual Function used to read INA219 register using I2C (hardware dependent)
         * 
         * @param[in] reg register address
         * @param[out] pBuf buffer for output data
         * @param[in] len length of data in bytes
         */
        virtual void readReg(uint8_t reg, uint8_t *pBuf, uint16_t len) = 0; 

        /**
         * @brief Function used to get INA219 register
         * 
         * @param[in] reg register address
         * @return uint16_t INA219 register value
         */
        uint16_t readInaReg(uint8_t reg);

        /**
         * @brief Function uset to set INA219 register
         * 
         * @param[in] reg register address
         * @param[in] value desired register value
         */
        void writeInaReg(uint8_t reg, uint16_t value);   
    };
}

#endif // INA219_HPP