#include "bsp.hpp"

#include "usart.h"

#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"

// #include "meter_thread.hpp"
#include "display_thread.hpp"

extern "C" UART_HandleTypeDef huart2;



#define DEBUG(_msg) (HAL_UART_Transmit(&huart2, (uint8_t*) _msg, strlen(_msg), 100))

FATFS FatFs; 	//Fatfs handle
FIL fil; 		//File handle
FRESULT fres; //Result after operations
BYTE readBuf[30];
DWORD free_clusters, free_sectors, total_sectors;
FATFS* getFreeFs;


void myprintf(const char *fmt, ...) {
  static char buffer[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  int len = strlen(buffer);
  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, -1);
}

int main() {
    // HAL init
    bsp::init();
    bsp::delay(1000);

	// initialize peripherals
	// ina energy(INA219_I2C_ADDRESS6);
	// display oled(U8G2_R2);
	
  // oled.begin();

	// oled.setFont(u8g2_font_6x13_mf);
	// oled.drawStr(0, 15, "TEST");

    // osKernelInitialize();
    // sdTaskHandle = osThreadNew(sdTask, NULL, &sdTask_attributes);
    // osKernelStart();

	// energy.reset();
	// energy.calibrate(0.01, 0.01);
        
    while (true) {
		// myprintf("Voltage: %.4f, Current: %.8f, Power: %.4f \n\r", energy.getBusVoltage_V(), energy.getCurrent_mA() * (0.01 / 32768), energy.getPower_mW());
		bsp::delay(1000);
    }
}
