################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/MCP4661.c \
../Src/UART_Parser.c \
../Src/adc.c \
../Src/dma.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/main.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/sys.c \
../Src/syscalls.c \
../Src/system_stm32f4xx.c \
../Src/tim.c \
../Src/usart.c 

OBJS += \
./Src/MCP4661.o \
./Src/UART_Parser.o \
./Src/adc.o \
./Src/dma.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/sys.o \
./Src/syscalls.o \
./Src/system_stm32f4xx.o \
./Src/tim.o \
./Src/usart.o 

C_DEPS += \
./Src/MCP4661.d \
./Src/UART_Parser.d \
./Src/adc.d \
./Src/dma.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/main.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/sys.d \
./Src/syscalls.d \
./Src/system_stm32f4xx.d \
./Src/tim.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Og -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

