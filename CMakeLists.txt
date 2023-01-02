cmake_minimum_required(VERSION 3.19)
set(CMAKE_BUILD_TYPE Release)
set(This energy_meter)

project(${This} CXX C ASM)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
    message("Building application for NUCLEO-L432KC")
    add_subdirectory(bsp/bsp_interface)   
    add_subdirectory(bsp/stm32l432kc) # Add implementation of bsp library for stm32l432kc 
    add_subdirectory(libraries/INA219)
    add_subdirectory(libraries/display)
    add_subdirectory(threads)
    set(CMAKE_EXECUTABLE_SUFFIX .elf) # Select executable extension for NUCLEO target
    add_executable(${This} main.cpp)
    target_link_libraries(${This} bsp ina219 display thread)
else()
    enable_testing()
    add_subdirectory(googletest)
    add_subdirectory(tests)
endif()
