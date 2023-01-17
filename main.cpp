#include "bsp.hpp"

#include "usart.h"

#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "meter_thread.hpp"
#include "display_thread.hpp"
#include "csv_thread.hpp"

extern "C" UART_HandleTypeDef huart2;

bool save_measurements = false;

void myprintf(const char *fmt, ...) {
	static char buffer[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	int len = strlen(buffer);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, -1);
}

osMessageQueueId_t message_queue_display;
osMessageQueueId_t message_queue_sd_card;

osThreadId_t meter_id; 
osThreadId_t display_id; 
osThreadId_t csv_id; 

const osThreadAttr_t thread1_attr1 = {
  	.name = "meter",
  	.stack_size = 256,
  	.priority = osPriorityHigh5,
};

const osThreadAttr_t thread1_attr2 = {
  	.name = "display",
  	.stack_size = 256,
  	.priority = osPriorityNormal7,
};

const osThreadAttr_t thread1_attr3 = {
  	.name = "csv",
  	.stack_size = 1024,
  	.priority = osPriorityHigh5,
};

uint8_t display_cb_mem[100];
uint8_t display_mem[32*(sizeof(energy_data_t))];
const osMessageQueueAttr_t display_queue_attr = 
{
	.name = "temp",
	.attr_bits = 0,
	.cb_mem = display_cb_mem,
	.cb_size = 100,
	.mq_mem = display_mem,
	.mq_size = 32*(sizeof(energy_data_t)),
};

uint8_t csv_cb_mem[100];
uint8_t csv_mem[32*(sizeof(energy_data_t))];
const osMessageQueueAttr_t csv_queue_attr = 
{
	.name = "temp",
	.attr_bits = 0,
	.cb_mem = csv_cb_mem,
	.cb_size = 100,
	.mq_mem = csv_mem,
	.mq_size = 32*(sizeof(energy_data_t)),
};

int main() {
  	// HAL init
	bsp::init();

	bsp::delay(1000);

	if (osKernelInitialize() != osOK)
	{
		while(1);
	}
	else
	{
		myprintf("Kernel initialized\r\n");
	}

	message_queue_display = osMessageQueueNew(32, sizeof(energy_data_t), &display_queue_attr);
	message_queue_sd_card = osMessageQueueNew(32, sizeof(energy_data_t), &csv_queue_attr);
	if (message_queue_display == NULL || message_queue_sd_card == NULL) {
		myprintf("queue creation failed\r\n");
		while(1);
	}

	meter_id = osThreadNew(meter_thread, NULL, &thread1_attr1);
	display_id = osThreadNew(display_thread, NULL, &thread1_attr2);
	csv_id = osThreadNew(csv_thread, NULL, &thread1_attr3);

	osKernelStart();

	while(true)
	{

	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == BUTTON_Pin)
	{
		save_measurements = !save_measurements;
		myprintf("measurements are %s\r\n", save_measurements ? "being saved!" : "not being saved!");
	}  
}