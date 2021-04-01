################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/DIO_program.c \
../src/common.c \
../src/usart_driver.c 

OBJS += \
./src/DIO_program.o \
./src/common.o \
./src/usart_driver.o 

C_DEPS += \
./src/DIO_program.d \
./src/common.d \
./src/usart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"E:\IMT\WORKSPACE\NTI\RTOS_APP2\inc" -I"E:\IMT\WORKSPACE\NTI\RTOS_APP2\FreeRTOS\FreeRTOS\Inc" -I"E:\IMT\WORKSPACE\NTI\RTOS_APP2" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


