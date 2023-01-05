/**
 * @file display_thread.cpp
 * @author Krzysztof Półchłopek
 * @author Michał Tomacha
 * @brief 
 * @version 0.1
 * @date 2023-01-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "display_thread.hpp"

#include <stdio.h>

#include "bsp.hpp"
#include "main.h"
#include "spi.h"

#define TX_TIMEOUT		100

extern "C" SPI_HandleTypeDef hspi1;

static uint8_t gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    //STM32 supports HW SPI, Remove unused cases like U8X8_MSG_DELAY_XXX & U8X8_MSG_GPIO_XXX
    switch(msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        //Insert codes for initialization
        break;
    case U8X8_MSG_DELAY_MILLI:
        //ms Delay */
        bsp::delay(arg_int);
        break;
    case U8X8_MSG_GPIO_CS:
        //Insert codes for SS pin control
        HAL_GPIO_WritePin(DISP_CS_GPIO_Port, DISP_CS_Pin, arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
        break;
    case U8X8_MSG_GPIO_DC:
        //Insert codes for DC pin control
        HAL_GPIO_WritePin(DISP_DC_GPIO_Port, DISP_DC_Pin, arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
        break;
    case U8X8_MSG_GPIO_RESET:
        //Insert codes for RST pin control
        HAL_GPIO_WritePin(DISP_RST_GPIO_Port, DISP_RST_Pin, arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
        break;
    }
    return 1;
}

static uint8_t byte_hw_interface(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch(msg) {
    case U8X8_MSG_BYTE_SEND:
        //Insert codes to transmit data
        if(HAL_SPI_Transmit(&hspi1, (uint8_t*) arg_ptr, arg_int, TX_TIMEOUT) != HAL_OK) return 0;
        break;
    case U8X8_MSG_BYTE_INIT:
        //Insert codes to begin SPI transmission
        break;
    case U8X8_MSG_BYTE_SET_DC:
        //Control DC pin, U8X8_MSG_GPIO_DC will be called
        u8x8_gpio_SetDC(u8x8, arg_int);
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        //Select slave, U8X8_MSG_GPIO_CS will be called
        u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
        bsp::delay(1);
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        bsp::delay(1);
        //Insert codes to end SPI transmission
        u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
        break;
    default:
        return 0;
    }
    return 1;
}



display::display() : DISPLAY(U8G2_R2, gpio_and_delay, byte_hw_interface)
{
    begin();
    setFont(u8g2_font_6x13_mf);
}

uint8_t display::calculateHoursFromSeconds(uint32_t sec)
{
    if (sec < 3600)
    {
        return 0;
    }
    else
    {
        return sec/3600;
    }
    
    
}
uint8_t display::calculateMinutesFromSeconds(uint32_t sec)
{
    if (sec < 60)
    {
        return 0;
    }
    else
    {
        return (sec/60)%60;
    }
    
}

void display::updateMeasurments(float voltage, float current, float power, uint32_t seconds)
{
    uint8_t hh = calculateHoursFromSeconds(seconds);
    uint8_t mm = calculateMinutesFromSeconds(seconds);
    char buffer [50];

    sprintf(buffer, "Time:    %02u:%02u:%02u", hh, mm, seconds%60);
    clearBuffer();
    drawStr(0, 15, buffer);
    sprintf(buffer, "Voltage: %.3f V", voltage);
    drawStr(0, 30, buffer);
    sprintf(buffer, "Current: %.3f mA", current);
    drawStr(0, 45, buffer);
    sprintf(buffer, "Power:   %.3f mW", power);
    drawStr(0, 60, buffer);
    sendBuffer();
}