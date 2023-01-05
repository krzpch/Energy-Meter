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

#include "display.hpp"

class display : public energymeter::DISPLAY
{
protected:
    uint8_t calculateHoursFromSeconds(uint32_t sec);
    uint8_t calculateMinutesFromSeconds(uint32_t sec);

public:
    display();

    void updateMeasurments(float voltage, float current, float power, uint32_t seconds);
};

#endif // DISPLAY_THREAD_H