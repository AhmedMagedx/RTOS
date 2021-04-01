################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/FreeRTOS/Src/PollQ.c \
../FreeRTOS/FreeRTOS/Src/croutine.c \
../FreeRTOS/FreeRTOS/Src/event_groups.c \
../FreeRTOS/FreeRTOS/Src/heap_1.c \
../FreeRTOS/FreeRTOS/Src/integer.c \
../FreeRTOS/FreeRTOS/Src/list.c \
../FreeRTOS/FreeRTOS/Src/port.c \
../FreeRTOS/FreeRTOS/Src/queue.c \
../FreeRTOS/FreeRTOS/Src/tasks.c 

OBJS += \
./FreeRTOS/FreeRTOS/Src/PollQ.o \
./FreeRTOS/FreeRTOS/Src/croutine.o \
./FreeRTOS/FreeRTOS/Src/event_groups.o \
./FreeRTOS/FreeRTOS/Src/heap_1.o \
./FreeRTOS/FreeRTOS/Src/integer.o \
./FreeRTOS/FreeRTOS/Src/list.o \
./FreeRTOS/FreeRTOS/Src/port.o \
./FreeRTOS/FreeRTOS/Src/queue.o \
./FreeRTOS/FreeRTOS/Src/tasks.o 

C_DEPS += \
./FreeRTOS/FreeRTOS/Src/PollQ.d \
./FreeRTOS/FreeRTOS/Src/croutine.d \
./FreeRTOS/FreeRTOS/Src/event_groups.d \
./FreeRTOS/FreeRTOS/Src/heap_1.d \
./FreeRTOS/FreeRTOS/Src/integer.d \
./FreeRTOS/FreeRTOS/Src/list.d \
./FreeRTOS/FreeRTOS/Src/port.d \
./FreeRTOS/FreeRTOS/Src/queue.d \
./FreeRTOS/FreeRTOS/Src/tasks.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/FreeRTOS/Src/%.o: ../FreeRTOS/FreeRTOS/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"E:\IMT\WORKSPACE\NTI\RTOS_APP2\inc" -I"E:\IMT\WORKSPACE\NTI\RTOS_APP2\FreeRTOS\FreeRTOS\Inc" -I"E:\IMT\WORKSPACE\NTI\RTOS_APP2" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


