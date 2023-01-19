add_library(bsp src/bsp.cpp src/bsp_clock.cpp)

add_subdirectory(cubeMX)

target_include_directories(bsp PUBLIC include)

target_include_directories(
  bsp
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

target_link_libraries(
  bsp
  PUBLIC bsp_interface
  PUBLIC stm32_hal
  INTERFACE stm32_hal)
