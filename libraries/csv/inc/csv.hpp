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
        Csv();
        ~Csv();
        bool begin(void);
        bool create_new_file(void);
        bool append_measurement(uint32_t time, float voltage, float current, float power);
        bool close_file(void);
        bool end(void);
        uint8_t get_last_error(void);
    
    };
}

#endif // CSV_HPP