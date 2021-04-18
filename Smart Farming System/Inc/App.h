/*
 * App.h
 *
 *  Created on: Apr 8, 2021
 *      Author: MaKh
 */

#ifndef INC_APP_APP_H_
#define INC_APP_APP_H_



#include "stdlib.h"
#include "string.h"
#include "usart_driver.h"
#include "lcd.h"
#include "DIO_interface.h"
#include "ADC_interface.h"
#include "sensors.h"




/* Tasks /Functions Prototypes*/
void System_Init(void);
void T_Control(void* pvParam);
void T_SysCheck(void* pvParam);
void T_Terminal(void* pvParam);
void T_Sensing(void* pvParam);
void T_Display(void* pvParam);
void T_Indication(void* pvParam);

/* used to trigger the T_Control task */
#define E_PUMPON		(1<<0)		/* Is set by the T_SysCheck task */
#define E_HEATERON		(1<<1)		/* Is set by the T_SysCheck task */
#define E_COOLERON		(1<<2)		/* Is set by the T_SysCheck task */
#define E_CONTROLMASK 	(0b111)

/* used to trigger the T_Display task */
#define E_MainScreen	(1<<0)
#define E_ConfigScreen	(1<<1)
#define E_HUpdated		(1<<2)
#define E_TUpdated		(1<<3)
#define E_TTUpdated		(1<<4)	/* Changed */
#define E_HTUpdated		(1<<5)
#define E_MotorState	(1<<6) 	/* Changed */
#define E_Next			(1<<7)	/* Changed */
#define E_DISPLAYMASK 	(0b11111111) /* Changed */


#define DISABLED 	0x2
#define ENABLED	 	0xF


/* MOTORs*/
#define WATER_PUMP 		DIO_u8_PIN2
#define HEATER	   		DIO_u8_PIN3
#define COOLER	   		DIO_u8_PIN4
#define CONTROL_PORT	DIO_u8_PORTD



/*************** Display ********************/
#define LCD_MAIN_SCREEN_L1		"T =    C    H =    %"
#define LCD_MAIN_SCREEN_L2		"TT=    C    TH=    %"
#define LCD_TEMP_COL			4
#define LCD_HUMI_COL			16

#define LCD_MAIN_SCREEN_L3		"P:      H:     C:   "
#define LCD_PUMP_COL			2
#define LCD_HEATER_COL			10
#define LCD_COOLER_COL			17

#define LCD_MAIN_SCREEN_L4		"  Configuration: C  "

#define LCD_CONFIG_SCREEN_L1	"TempThreshold:     C"
#define LCD_CONFIG_SCREEN_L2	"HumiThreshold:     %"
#define LCD_CONFIG_COL			14

#define LCD_CONFIG_SCREEN_L4	"OK:O Next:N Cancel:C"

/*Program:   22874 bytes (69.8% Full)
(.text + .data + .bootloader)

Data:       1436 bytes (70.1% Full)
(.data + .bss + .noinit)
 * */



#if	1
EventGroupHandle_t egControl = NULL;
EventGroupHandle_t egDisplay = NULL;
EventBits_t ebControlBits;
EventBits_t ebDisplayBits;
SemaphoreHandle_t bsCheck;
#endif

/*
 * Program:   22874 bytes (69.8% Full)
(.text + .data + .bootloader)

Data:       1436 bytes (70.1% Full)
(.data + .bss + .noinit)
 *
 * */

/* 1 */
typedef enum
{
	OFF,ON
}Motor;

/* 1 */
struct
{
	Motor Water_Pump;
	Motor Heater;
	Motor Cooler;
}Motors_State;

typedef enum  {MainState, ConfigState} SystemState_t ;

/* 3 */
struct
{
	SystemState_t SystemState;
	struct
	{
		u8 TempData;
		u8 HumiData;
	} SensorData;

	struct
	{
		u8 TempT;
		u8 HumiT;
	} SensorThreshold;

} SFS;

/*
Program:   22874 bytes (69.8% Full)
(.text + .data + .bootloader)

Data:       1446 bytes (70.6% Full)
(.data + .bss + .noinit)*/


#endif /* INC_APP_APP_H_ */
