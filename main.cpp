#include "bsp.hpp"

#include "usart.h"
#include "spi.h"

#include <cstring>
#include <cstdio>
#include <cstdarg>

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"

extern "C" UART_HandleTypeDef huart2;
extern "C" SPI_HandleTypeDef hspi1;

#define DEBUG(_msg) (HAL_UART_Transmit(&huart2, (uint8_t*) _msg, strlen(_msg), 100))

FATFS FatFs; 	//Fatfs handle
FIL fil; 		//File handle
FRESULT fres; //Result after operations
BYTE readBuf[30];
DWORD free_clusters, free_sectors, total_sectors;
FATFS* getFreeFs;

osThreadId_t sdTaskHandle;
const osThreadAttr_t sdTask_attributes = {
  .name = "sdTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void sdTask(void *argument);

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

    osKernelInitialize();

    sdTaskHandle = osThreadNew(sdTask, NULL, &sdTask_attributes);

    osKernelStart();
        
    while (true) {

    }
}

void sdTask(void *argument)
{
	myprintf("test\r\n");

	//Open the file system
	fres = f_mount(&FatFs, "", 1); //1=mount now
	if (fres != FR_OK) {
	myprintf("f_mount error (%i)\r\n", fres);
	while(1);
	}

	fres = f_getfree("", &free_clusters, &getFreeFs);
	if (fres != FR_OK) {
	myprintf("f_getfree error (%i)\r\n", fres);
	while(1);
	}

	//Formula comes from ChaN's documentation
	total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
	free_sectors = free_clusters * getFreeFs->csize;

	myprintf("SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);

	//Now let's try and write a file "write.txt"
	fres = f_open(&fil, "write.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
	if(fres == FR_OK) {
	myprintf("I was able to open 'write.txt' for writing\r\n");
	} else {
	myprintf("f_open error (%i)\r\n", fres);
	}

	//Copy in a string
	strncpy((char*)readBuf, "test", 5);
	UINT bytesWrote;
	fres = f_write(&fil, readBuf, 19, &bytesWrote);
	if(fres == FR_OK) {
	myprintf("Wrote %i bytes to 'write.txt'!\r\n", bytesWrote);
	} else {
	myprintf("f_write error (%i)\r\n");
	}

	//Be a tidy kiwi - don't forget to close your file!
	f_close(&fil);

	//Now let's try to open file "write.txt"
	fres = f_open(&fil, "write.txt", FA_READ);
	if (fres != FR_OK) {
	myprintf("f_open error (%i)\r\n");
	while(1);
	}
	myprintf("I was able to open 'write.txt' for reading!\r\n");

	//We can either use f_read OR f_gets to get data out of files
	//f_gets is a wrapper on f_read that does some string formatting for us
	TCHAR* rres = f_gets((TCHAR*)readBuf, 20, &fil);
	if(rres != 0) {
	myprintf("Read string from 'write.txt' contents: %s\r\n", readBuf);
	} else {
	myprintf("f_gets error (%i)\r\n", fres);
	}

	//Be a tidy kiwi - don't forget to close your file!
	f_close(&fil);

	//We're done, so de-mount the drive
	f_mount(NULL, "", 0);
}
