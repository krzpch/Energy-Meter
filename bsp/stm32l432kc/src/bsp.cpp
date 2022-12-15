#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"


extern "C" void SystemClock_Config(void);
namespace bsp {


    void init() {
        HAL_Init();
        SystemClock_Config();
        MX_GPIO_Init();
        MX_USART2_UART_Init();
        MX_I2C1_Init();
        MX_SPI1_Init();
        MX_SPI3_Init();
        MX_FATFS_Init();
    }


} // namespace bsp
