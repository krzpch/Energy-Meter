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

#ifdef USE_CPP

	DISPLAY::DISPLAY(const u8g2_cb_t *rotation, uint8_t (*gpio_and_delay_func)(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr), uint8_t (*byte_hw_interface_func)(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)) : U8G2()
	{
		u8g2_Setup_sh1106_128x64_noname_f(&u8g2, rotation, byte_hw_interface_func, gpio_and_delay_func);
    	// u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
	}

#else
    DISPLAY::DISPLAY()
    {
    }
    
    DISPLAY::~DISPLAY() 
    {
		u8g2_ClearDisplay(&u8g2);
    }

	// uint8_t DISPLAY::u8x8_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
	// {
	// 	return u8x8_gpio_and_delay_cb(u8x8, msg, arg_int, arg_ptr);
	// }
    // uint8_t DISPLAY::u8x8_byte_hw_interface_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
	// {
	// 	return u8x8_byte_hw_interface_cb(u8x8, msg, arg_int, arg_ptr);
	// }

	void DISPLAY::setFont(const uint8_t *font)
	{
		u8g2_SetFont(&u8g2, font);
	}

	void DISPLAY::init(void)
	{
		u8g2_Setup_sh1106_128x64_noname_f(&u8g2, U8G2_R2, [](u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr){disp->u8x8_byte_hw_interface(u8x8, msg, arg_int, arg_ptr);}, &DISPLAY::u8x8_gpio_and_delay_cb);
        u8g2_InitDisplay(&u8g2);
        u8g2_SetPowerSave(&u8g2, 0);
	}

	void DISPLAY::drawStr(const uint16_t x, const uint16_t y, const char *str)
	{
		u8g2_DrawStr(&u8g2, x, y, str);
	}

	void DISPLAY::drawLine(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2)
	{
		u8g2_DrawLine(&u8g2, x1, y1, x2, y2);
	}

	void DISPLAY::sendBuffer(void)
	{
		u8g2_SendBuffer(&u8g2);
	}

	void DISPLAY::clearDisplay(void)
	{
		u8g2_ClearDisplay(&u8g2);
	}
#endif
} // namespace energymeter
