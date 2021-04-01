################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LAbs/MX.c 

OBJS += \
./LAbs/MX.o 

C_DEPS += \
./LAbs/MX.d 


# Each subdirectory must supply rules for building sources it contributes
LAbs/%.o: ../LAbs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"E:\IMT\WORKSPACE\RTOS_APP2\inc" -I"E:\IMT\WORKSPACE\RTOS_APP2\FreeRTOS\FreeRTOS\Inc" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


