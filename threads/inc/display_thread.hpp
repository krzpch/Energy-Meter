/**
 * @file display_thread.hpp
 * @author Krzysztof Półchłopek
 * @author Michał Tomacha
 * @brief 
 * @version 0.1
 * @date 2022-12-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DISPLAY_THREAD_H
#define DISPLAY_THREAD_H

#define TX_TIMEOUT		100

#include "bsp.hpp"
#include "main.h"
#include "spi.h"

#include "display.hpp"

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

extern "C" SPI_HandleTypeDef hspi1;





class display : public energymeter::DISPLAY
{
public:
    display();
};

#ifdef USE_CPP
display::display() : DISPLAY(U8G2_R2, gpio_and_delay, byte_hw_interface)
{
}
#else
display::display(const u8g2_cb_t *rotation) : DISPLAY()
{
}
#endif



#endif // DISPLAY_THREAD_H