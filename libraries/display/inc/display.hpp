/**
 * @file display.hpp
 * @author Krzysztof Półchłopek
 * @author Michał Tomacha
 * @brief 
 * @version 0.1
 * @date 2022-12-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "U8g2lib.h"

namespace energymeter
{
    class DISPLAY : public U8G2
    {
    public:
        /**
         * @brief Construct a new DISPLAY object
         * 
         * @param [in] rotation rotation in which display is used 
         * @param [in] gpio_and_delay_func function pointer for gpio controll and delays 
         * @param [in] byte_hw_interface_func function pointer for hardware interface controll 
         * @note Visit https://github.com/olikraus/u8g2/wiki/Porting-to-new-MCU-platform for examples on specific function pointers
         */
        DISPLAY(const u8g2_cb_t *rotation, uint8_t (*gpio_and_delay_func)(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr), uint8_t (*byte_hw_interface_func)(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr));

    };
} // namespace energymeter


#endif