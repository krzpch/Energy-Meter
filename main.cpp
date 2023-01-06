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

#include "meter_thread.hpp"
#include "display_thread.hpp"
#include "csv_thread.hpp"

extern "C" UART_HandleTypeDef huart2;

#define DEBUG(_msg) (HAL_UART_Transmit(&huart2, (uint8_t*) _msg, strlen(_msg), 100))

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
	ina energy(INA219_I2C_ADDRESS6);
	display oled;

  bsp::delay(1000);
  uint32_t temp_time = 0;


  energymeter::Csv csv;
  myprintf("last_result: %d\r\n", csv.last_result);
  if(!csv.create_new_file())
  {
    myprintf("create_new_file failed\r\n");
    myprintf("last_result: %d\r\n", csv.last_result);
    while(1);
  }

    // osKernelInitialize();
    // sdTaskHandle = osThreadNew(sdTask, NULL, &sdTask_attributes);
    // osKernelStart();

	energy.reset();
	energy.calibrate(1.0f);

  // myprintf("TEST3");
  // csv.append_measurement(1.0f, 2.0f, 3.0f);
  // myprintf("TEST4");

  while (temp_time < 100) {
    myprintf("TEST1");
    energy.read_measurements();
    bsp::delay(100);
    myprintf("TEST2");
    oled.updateMeasurments(energy.get_voltage(), energy.get_current(), energy.get_power(), ++temp_time);
    bsp::delay(100);
    myprintf("TEST3");
    csv.append_measurement(energy.get_voltage(), energy.get_current(), energy.get_power());
    bsp::delay(1000);
    myprintf("TEST4");
  }

  if(!csv.close_file())
  {
    myprintf("close_file\r\n");
    myprintf("last_result: %d\r\n", csv.last_result);
    while(1);
  }

  while(true)
  {

  }
}
