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

#ifndef CSV_HPP
#define CSV_HPP

#include <cstdio>
#include "fatfs.h"

#define MAX_FILES 10

namespace energymeter
{
    class Csv
    {
    private:
        FATFS *FatFs; 	        //Fatfs handle
        FIL *file; 		        //File handle
        FRESULT last_result;    //result from last Fatfs operation
        
    public:
        /**
         * @brief Construct a new Csv object
         */
        Csv();

        /**
         * @brief Destroy the Csv object
         */
        ~Csv();

        /**
         * @brief Mounts SD card to FatFs
         * 
         * @return true if SD card succesfully mounted
         * @return false if any FatFs error occured during mount
         */
        bool begin(void);

        /**
         * @brief Creates an initializes .csv file on SD card
         * 
         * @return true if file created succefully
         * @return false if any FatFs error occured during creation
         */
        bool create_new_file(void);

        /**
         * @brief Appends measurement to currently open .csv file
         * 
         * @note For measurements to be saved file need to be properly closed using close_file() method
         * 
         * @param[in] time timestamp of measurement
         * @param[in] voltage measured voltage
         * @param[in] current measured voltage
         * @param[in] power measured voltage
         * @return true if measurement appended succefully
         * @return false if any FatFs error occured
         */
        bool append_measurement(uint32_t time, float voltage, float current, float power);

        /**
         * @brief Closes currently open .csv file
         * 
         * @return true if measurement appended succefully
         * @return false if any FatFs error occured
         */
        bool close_file(void);

        /**
         * @brief Unmounts SD card from FatFs
         * 
         * @return true if SD card unmounted succefully
         * @return false if any FatFs error occured
         */
        bool end(void);

        /**
         * @brief Returns last FatFs error
         * 
         * @return uint8_t FatFs error number 
         */
        uint8_t get_last_error(void);
    
    };
}

#endif // CSV_HPP