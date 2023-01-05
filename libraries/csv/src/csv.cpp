/**
 * @file csv.cpp
 * @author Krzysztof Półchłopek
 * @author Michał Tomacha
 * @brief 
 * @version 0.1
 * @date 2023-01-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "csv.hpp"

namespace energymeter
{
    Csv::Csv()
    {
        last_result = f_mount(&FatFs, "", 1);
    }

    Csv::~Csv()
    {
        f_mount(NULL, "", 0);
    }

    bool Csv::create_new_file(void)
    {
        BYTE writeBuf[30];
        BYTE fileName[30];
        UINT fileNumber = 1;
        UINT bytesToWrite;
        UINT bytesWritten;

        do {
            sprintf((char*)fileName, "meas%d.csv", fileNumber++);
            last_result = f_open(&file, (char*)fileName, FA_CREATE_NEW | FA_WRITE);
        } while ((last_result == FR_EXIST) && (fileNumber <= MAX_FILES));
        
        if(last_result == FR_OK)
        {
            bytesToWrite = sprintf((char*)writeBuf, "voltage,current,power\r\n");
            last_result = f_write(&file, writeBuf, bytesToWrite, &bytesWritten);

            if((last_result == FR_OK) && (bytesToWrite == bytesWritten))
            {
                return true;
            }
        }

        return false;
    }

    bool Csv::append_measurement(float voltage, float current, float power)
    {
        BYTE writeBuf[30];
        UINT bytesToWrite;
        UINT bytesWritten;

        bytesToWrite = sprintf((char*)writeBuf, "%f,%f,%f\r\n", voltage, current, power);
        last_result = f_write(&file, writeBuf, bytesToWrite, &bytesWritten);

        if((last_result == FR_OK) && (bytesToWrite == bytesWritten))
        {
            return true;
        }

        return false;
    }

    bool Csv::close_file(void)
    {
        last_result = f_close(&file);

        if(last_result == FR_OK)
        {
            return true;
        }
        return false;
    }
}