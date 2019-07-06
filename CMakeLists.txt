cmake_minimum_required (VERSION 3.13)
project(speedctrl C ASM)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_STDPERIPH_DRIVER -DSTM32F070x6")

include_directories(BEFORE
    vendor/stm/Drivers/STM32F0xx_HAL_Driver/Inc
    vendor/stm/Drivers/CMSIS/Device/ST/STM32F0xx/Include
    vendor/stm/Drivers/CMSIS/Include
    src
)

add_executable(${CMAKE_PROJECT_NAME}.elf
    src/main.c
    src/ac_synth.c
    src/nco.c
    src/system_stm32f0xx.c
    src/syscalls.c
    src/startup_stm32f070x6.s
	src/stm32f0xx_it.c
    vendor/stm/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c
    vendor/stm/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c
    vendor/stm/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c
    vendor/stm/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c
    vendor/stm/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_adc.c
    vendor/stm/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c
    vendor/stm/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c
)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/link/flash.ld")
