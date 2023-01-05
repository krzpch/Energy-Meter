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

#define USE_CPP

#ifdef USE_CPP
#include "U8g2lib.h"
#else
#include "u8g2.h"
#endif

namespace energymeter
{

/*
class U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI : public U8G2 {
  public: U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
    u8g2_Setup_sh1106_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_4wire_sw_spi, u8x8_gpio_and_delay_arduino);
    u8x8_SetPin_4Wire_SW_SPI(getU8x8(), clock, data, cs, dc, reset);
  }
};
*/

#ifdef USE_CPP
    class DISPLAY : public U8G2
    {
    private:


    public:
        DISPLAY(const u8g2_cb_t *rotation, uint8_t (*gpio_and_delay_func)(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr), uint8_t (*byte_hw_interface_func)(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr));

    };

#else

    class DISPLAY
    {
    private:
        u8g2_t u8g2;

    protected:
        virtual uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) = 0;
        virtual uint8_t u8x8_byte_hw_interface(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) = 0;

    static uint8_t u8x8_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
	{
		return u8x8_gpio_and_delay_cb(u8x8, msg, arg_int, arg_ptr);
	}
    static uint8_t u8x8_byte_hw_interface_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
	{
		return u8x8_byte_hw_interface_cb(u8x8, msg, arg_int, arg_ptr);
	}

    public:
        DISPLAY();
        ~DISPLAY();

        void init(void);

        void setFont(const uint8_t *font);
        void drawStr(const uint16_t x, const uint16_t y, const char *str);
        void drawLine(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2);
        void sendBuffer(void);
        void clearDisplay(void);
    };
#endif
} // namespace energymeter


#endif