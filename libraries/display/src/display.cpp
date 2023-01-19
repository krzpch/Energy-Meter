/**
 * @file display.cpp
 * @author Krzysztof Półchłopek
 * @author Michał Tomacha
 * @brief 
 * @version 0.1
 * @date 2022-12-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "display.hpp"

namespace energymeter
{
	DISPLAY::DISPLAY(const u8g2_cb_t *rotation, uint8_t (*gpio_and_delay_func)(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr), uint8_t (*byte_hw_interface_func)(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)) : U8G2()
	{
		u8g2_Setup_sh1106_128x64_noname_f(&u8g2, rotation, byte_hw_interface_func, gpio_and_delay_func);
	}
} // namespace energymeter
