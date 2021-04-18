################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c 

OBJS += \
./Src/main.o 

C_DEPS += \
./Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"E:\IMT\WORKSPACE\TempAlarmingSystem" -I"E:\IMT\WORKSPACE\TempAlarmingSystem\Inc\MCAL" -I"E:\IMT\WORKSPACE\TempAlarmingSystem\FreeRTOS\Inc" -I"E:\IMT\WORKSPACE\TempAlarmingSystem\Inc" -I"E:\IMT\WORKSPACE\TempAlarmingSystem\Inc\App" -I"E:\IMT\WORKSPACE\TempAlarmingSystem\Inc\common" -I"E:\IMT\WORKSPACE\TempAlarmingSystem\Inc\ECU" -I"E:\IMT\WORKSPACE\TempAlarmingSystem\Inc\MCAL" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


