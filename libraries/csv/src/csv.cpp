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
#include "string.h"

namespace energymeter
{
    Csv::Csv()
    {
        FatFs = new FATFS;
        file = new FIL;
    }

    Csv::~Csv()
    {
        delete FatFs;
        delete file;
    }

    bool Csv::begin(void)
    {
        last_result = f_mount(FatFs, "", 1);

        if(last_result == FR_OK)
        {
            return true;
        }

        return false;
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
            last_result = f_open(file, (char*)fileName, FA_CREATE_NEW | FA_WRITE);
        } while ((last_result == FR_EXIST) && (fileNumber <= MAX_FILES));
        
        if(last_result == FR_OK)
        {
            bytesToWrite = sprintf((char*)writeBuf, "time,voltage,current,power\r\n");
            last_result = f_write(file, writeBuf, bytesToWrite, &bytesWritten);

            if((last_result == FR_OK) && (bytesToWrite == bytesWritten))
            {
                return true;
            }
        }

        return false;
    }

    bool Csv::append_measurement(uint32_t time, float voltage, float current, float power)
    {
        BYTE writeBuf[30];
        UINT bytesToWrite;
        UINT bytesWritten;

        bytesToWrite = sprintf((char*)writeBuf, "%d,%f,%f,%f\r\n", time, voltage, current, power);
        last_result = f_write(file, writeBuf, bytesToWrite, &bytesWritten);

        if((last_result == FR_OK) && (bytesToWrite == bytesWritten))
        {
            return true;
        }

        return false;
    }

    bool Csv::close_file(void)
    {
        last_result = f_close(file);

        if(last_result == FR_OK)
        {
            return true;
        }
        return false;
    }

    bool Csv::end(void)
    {
        last_result = f_mount(NULL, "", 0);
        
        memset(FatFs, 0, sizeof(FATFS));
        memset(file, 0, sizeof(FIL));

        if(last_result == FR_OK)
        {
            return true;
        }
        return false;
    }

    uint8_t Csv::get_last_error(void)
    {
        return last_result;
    }
}