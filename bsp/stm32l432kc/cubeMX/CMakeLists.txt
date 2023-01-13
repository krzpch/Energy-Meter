add_library(stm32_hal INTERFACE)
target_link_libraries(stm32_hal INTERFACE stm32_hal_obj $<TARGET_OBJECTS:stm32_hal_obj>)

add_library(
  stm32_hal_obj OBJECT
  Core/Src/main.c
  Core/Src/gpio.c
  Core/Src/freertos.c
  Core/Src/i2c.c
  Core/Src/spi.c
  Core/Src/usart.c
  Core/Src/stm32l4xx_it.c
  Core/Src/stm32l4xx_hal_msp.c
  Core/Src/stm32l4xx_hal_timebase_tim.c
  FATFS/Target/user_diskio.c
  FATFS/Target/user_diskio_spi.c
  FATFS/App/fatfs.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi_ex.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c
  Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c
  Core/Src/system_stm32l4xx.c
  #Middlewares/Third_Party/FatFs/src/diskio.c
  Core/Src/diskio.c
  Middlewares/Third_Party/FatFs/src/ff.c
  Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
  Middlewares/Third_Party/FatFs/src/option/syscall.c
  Middlewares/Third_Party/FreeRTOS/Source/croutine.c
  Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
  Middlewares/Third_Party/FreeRTOS/Source/list.c
  Middlewares/Third_Party/FreeRTOS/Source/queue.c
  Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
  Middlewares/Third_Party/FreeRTOS/Source/tasks.c
  Middlewares/Third_Party/FreeRTOS/Source/timers.c
  Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
  Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
  Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c  
  startup_stm32l432xx.s)

target_include_directories(
  stm32_hal_obj
  PUBLIC  Core/Inc
          Drivers/STM32L4xx_HAL_Driver/Inc
          Drivers/STM32L4xx_HAL_Driver/Inc/Legacy
          Drivers/CMSIS/Device/ST/STM32L4xx/Include
          Drivers/CMSIS/Include
          FATFS/App
          FATFS/Target
          Middlewares/Third_Party/FatFs/src
          Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
          Middlewares/Third_Party/FreeRTOS/Source/include
          Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F)

target_compile_definitions(stm32_hal_obj PUBLIC USE_HAL_DRIVER STM32L432xx)
