################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Src/MCAL/subdir.mk
-include Src/ECU/subdir.mk
-include Src/subdir.mk
-include FreeRTOS/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
TempAlarmingSystem.lss \

FLASH_IMAGE += \
TempAlarmingSystem.hex \

EEPROM_IMAGE += \
TempAlarmingSystem.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: TempAlarmingSystem.elf secondary-outputs

# Tool invocations
TempAlarmingSystem.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,TempAlarmingSystem.map -mmcu=atmega32 -o "TempAlarmingSystem.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

TempAlarmingSystem.lss: TempAlarmingSystem.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S TempAlarmingSystem.elf  >"TempAlarmingSystem.lss"
	@echo 'Finished building: $@'
	@echo ' '

TempAlarmingSystem.hex: TempAlarmingSystem.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex TempAlarmingSystem.elf  "TempAlarmingSystem.hex"
	@echo 'Finished building: $@'
	@echo ' '

TempAlarmingSystem.eep: TempAlarmingSystem.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex TempAlarmingSystem.elf  "TempAlarmingSystem.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: TempAlarmingSystem.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 TempAlarmingSystem.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) TempAlarmingSystem.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
