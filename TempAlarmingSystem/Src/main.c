/*
 * main.c
 *
 *  Created on: Apr 4, 2021
 *      Author: MaKh
 */

#if 1
/* include OS header file */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#endif

#include "stdlib.h"
#include "string.h"
#include "usart_driver.h"
#include "LCD_interface.h"
#include "DIO_interface.h"
#include "ADC_interface.h"

/* Tasks /Functions Prototypes*/
void System_Init(void);
void T_Alarm(void* pvParam);
void T_SysCheck(void* pvParam);
void T_Terminal(void* pvParam);
void T_CTemp(void* pvParam);
void T_Display(void* pvParam);

/* used to trigger the T_Alarm task */
#define E_Alarm	(1<<0)		/* Is set by the T_SysCheck task */


/* used to trigger the T_Display task */
#define E_MainScreen		(1<<1)
#define E_ConfigScreen		(1<<2)
#define E_AlarmScreen		(1<<3)
#define E_CTempUpdated		(1<<4)
#define E_TTempUpdated		(1<<5)
#define E_ASUpdated			(1<<6)
#define E_TConfigUpdated	(1<<7)


#define DISABLED 0
#define ENABLED	 1




EventGroupHandle_t egEvents = NULL;
EventBits_t ebBits;
SemaphoreHandle_t bsCheck;



struct
{
	enum  {MainState, ConfigState, AlarmState} SystemState;
	u8 CTemp;
	u8 TTemp;
	u8 AS;
} TAS;


int main(void)
{

	System_Init();

	/* OS Object Creation */
	egEvents = xEventGroupCreate();
	bsCheck = xSemaphoreCreateBinary();


#if 1
	LCD_vidWriteString("System Started");
	/* Creating the tasks*/
	xTaskCreate(T_Display, 	NULL, 150, NULL, 1, NULL);
	xTaskCreate(T_CTemp, 	NULL, 150, NULL, 2, NULL);
	xTaskCreate(T_Terminal, NULL, 150, NULL, 3, NULL);
	xTaskCreate(T_SysCheck, NULL, 150, NULL, 4, NULL);
	xTaskCreate(T_Alarm,	NULL, 100, NULL, 5, NULL);

	vTaskStartScheduler();

#endif

}


void T_Alarm(void* pvParam)
{
	usart_puts("T_Alarm started ...\r\n");

	while(1)
	{
		//usart_puts("Running T_Alarm\r\n");

		/* Waiting the event to Start the alarm */
		ebBits = xEventGroupWaitBits(egEvents, E_Alarm, 0, 0, portMAX_DELAY);
		if( (ebBits&E_Alarm) == E_Alarm )	/* if exit waiting because of event */
		{
			//usart_puts("E_Alarm received...\r\n");
			DIO_voidSetPinValue(DIO_u8_PORTD, DIO_u8_PIN2, DIO_u8_HIGH );
			vTaskDelay(500);
			DIO_voidSetPinValue(DIO_u8_PORTD, DIO_u8_PIN2, DIO_u8_LOW );
			vTaskDelay(500);
		}
	}
}

void T_SysCheck(void* pvParam)
{
	usart_puts("T_SysCheck started...\r\n");

	/* initial trigger to T_Display Task */
	xEventGroupSetBits(egEvents, E_MainScreen); /* for T_Display Task */

	while(1)
	{

		//usart_puts("Running T_SysCheck \r\n");

		/* Waiting Sem. from T_CTemp or T_terminal */
		if(xSemaphoreTake(bsCheck, portMAX_DELAY))
		{
			//usart_puts("xSemaphoreTake \r\n");
			if( TAS.SystemState == MainState && TAS.CTemp >= TAS.TTemp && TAS.AS == ENABLED)
			{
			/* if we are in main screen AND if temp is bigger than threshold AND Alarming enabled*/
				//usart_puts("condition1 \r\n");
				TAS.SystemState = AlarmState;	/* to display the alarm screen when T_Display is called*/
				xEventGroupSetBits(egEvents, E_Alarm); /* trigger the T_Alarm Task */
				xEventGroupSetBits(egEvents, E_AlarmScreen); /* for T_Display Task */
			}
			else if( TAS.SystemState == AlarmState &&
					 (TAS.CTemp < TAS.TTemp || TAS.AS == DISABLED) )
			{			/* if we are in alarm screen AND ( temp is below  threshold or Alarming disabled*/
				//usart_puts("condition2 \r\n");
				TAS.SystemState = MainState;	/* to display the main screen when T_Display is called*/
				xEventGroupClearBits(egEvents, E_Alarm); /* disable the Alarm */

				/* for T_Display Task */
				xEventGroupSetBits(egEvents, E_MainScreen 	| E_CTempUpdated);
				xEventGroupSetBits(egEvents, E_TTempUpdated | E_ASUpdated);

			}
			else
			{	/* the cpu is  not supposed to reach this condition, but it just to make sure */
				//usart_puts("condition else \r\n");
				xEventGroupClearBits(egEvents, E_Alarm); /* disable the Alarm */
			}
		}
	}
}

/* Global */
struct
{
	u8 strTTemp[4];
	u8 i;
}g_temp_TTemp;

void T_Terminal(void* pvParam)
{
	usart_puts("T_Terminal started ...\r\n");
	u8 data;
	u8 strTTemp[4];
	memset(strTTemp, 0, 3);
	u8 i = 0;
	while(1)
	{
		//usart_puts("Running T_Terminal\r\n");

		if(E_OK == usart_getc_NonBlocking(&data))	/* New data exist */
		{
			if(TAS.SystemState == MainState)
			{
				/* if( MainState is the current)*/
				if(data == 'C')
				{
					TAS.SystemState = ConfigState;
					xEventGroupSetBits(egEvents, E_ConfigScreen);
				}
				else if(data == 'T')
				{
					/*************************************/
					/* Toggle the AlarmState (between on Enabled and disabled)*/
					TAS.AS ^= 1;
					//usart_puts("\r\nTAS.AS = "); /* DEBUG */
					//usart_putc(TAS.AS + 48); /* DEBUG */
					//usart_puts("\r\n"); /* DEBUG */
					vTaskDelay(200); /* DEBUG */
					xEventGroupSetBits(egEvents, E_ASUpdated);	/* to trigger display task and  update the Alarm state*/
					xSemaphoreGive(bsCheck);	/* to trigger T_SysCheck task to update the T_Alarm task state */
				}
			}	/*end if( MainState is the current)*/

			else if(TAS.SystemState == ConfigState)
			{/* if( ConfigState is the current)*/
				if( data >= '0' && data <= '9')
				{
					/* if the data is a digit */
					strTTemp[i] = data;
					i++;

					/* make data ready for display task, just a copy */
					memset( g_temp_TTemp.strTTemp, strTTemp, 3);
					g_temp_TTemp.i = i;

					xEventGroupSetBits(egEvents, E_TConfigUpdated);	/* to trigger the Display task*/

					if(i > 2)
					{
						/* if the last stored data is the third digit*/
						i = 0;
						xEventGroupSetBits(egEvents, E_TTempUpdated);	/* to trigger the Display task*/
					}
				}/* end if the data is a digit */
				else if(data == 'O')
				{/* if data is O (OK) */
					TAS.TTemp = atoi(strTTemp);	/* Set the global threshold with local from terminal*/
					TAS.SystemState = MainState; /* we done configure hence done with congigure screen*/
					xSemaphoreGive(bsCheck); /* to trigger T_SysCheck to update the alarm state based on new threshold from terminal*/

					/* For display task */
					xEventGroupSetBits(egEvents, E_MainScreen 	| E_CTempUpdated);
					xEventGroupSetBits(egEvents, E_TTempUpdated | E_ASUpdated);

					i = 0;	/* when OK, we finished storing digit hence clear index for next time */
				}/* end if data is O (OK) */
				else if(data == 'C')
				{/* if data is C (Cancel) */
					TAS.SystemState = MainState; /* we done configure hence done with congigure screen*/
					/* For display task */
					xEventGroupSetBits(egEvents, E_MainScreen 	| E_CTempUpdated);
					xEventGroupSetBits(egEvents, E_TTempUpdated | E_ASUpdated);
				}/* end if data is C (Cancel) */

			}/*end if( ConfigState is the current)*/
			else if(TAS.SystemState == AlarmState)
			{/* if( AlarmState is the current)*/
				if(data == 'S')
				{
					TAS.AS = DISABLED;	/* disable AlarmState to disable the Alarm */
					xSemaphoreGive(bsCheck); 	/* to trigger T_SysCheck to update T_Alarm */
					xEventGroupSetBits(egEvents, E_MainScreen);	/* to trigger the Display task*/
				}
			}/*end if( AlarmState is the current)*/
			else
			{
				/* if the current state is not MainState nor ConfigState nor AlarmState , Wrong Condition*/
			}

		}/* end if new data exist */
		else
		{
			/* if No new data exist */
			/* exit */
		}
		vTaskDelay(50);  /* Task periodicity is 50ms */
	}
}


#if 0
ERROR_t ADC_u16GetDigitalValue_NonBlockingFAKE(u8 channel, u8* pData)
{
	static u8 locData = 0;
	static u8 locTimer = 0;
	locTimer++;
	//usart_puts("HERE\r\n");
	if(locTimer < 20)
	{
		return PENDING;
	}
	locTimer = 0;
	locData++;
	locData = locData % 5;
	*pData = locData;
	return E_OK;
}
#endif

void T_CTemp(void* pvParam)
{
	usart_puts("T_CTemp started ...\r\n");
	u16 ADCData = 0; /* 0V >> 5V*/	/* 0C >> 50C */
	u8 NewTemp = ADCData * 10 ;	/* Fake Temp */
	while(1)
	{

		//if(E_OK == ADC_u16GetDigitalValue_NonBlocking(0, &ADCData))	/*if reaady, read from channel zero to ADCData */
		//if(E_OK == ADC_u16GetDigitalValue_NonBlockingFAKE(0, &ADCData))	/* FAKE */
		ADCData = ADC_u16GetDigitalValue(0);
		ADCData = (ADCData*50) / 1024;
		{

#if 1

			//usart_puts("Running T_CTemp\r\n");

			NewTemp = ADCData  ;	/* (Fake Temp) */

			//usart_puts("ADC is\r\n");
			//usart_putc(ADCData + 48);
			//usart_puts("\r\n");



			if( NewTemp != TAS.CTemp )	/* if the new readed temp is not the already stored in the system*/
			{
				TAS.CTemp = NewTemp;
				xSemaphoreGive(bsCheck); /* to trigger T_SysCheck to update T_Alarm based on new Temp*/
				if(TAS.SystemState == MainState)
				{/* if the current state is Main*/
					xEventGroupSetBits(egEvents, E_TTempUpdated);	/* to update display */
				}
			}
#endif

		}

		vTaskDelay(200);  	/* Task periodicity is 100ms */

	}
}


/* private to T_Display task */
static void	Private_DisplayMainScreen(void);
static void	Private_DisplayConfigScreen(void);
static void	Private_DisplayAlarmScreen(void);
static void	Private_UpdateCTemp(void);
static void	Private_UpdateTTemp(void);
static void	Private_UpdateAS(void);
static void	Private_UpdateTTempConfig(void);


void T_Display(void* pvParam)
{
	usart_puts("T_Display started ...\r\n");

	while(1)
	{
		//usart_puts("Running T_Display\r\n");
		ebBits = xEventGroupWaitBits(egEvents, E_MainScreen | E_ConfigScreen |
									  E_AlarmScreen| E_CTempUpdated |
									  E_TTempUpdated|E_ASUpdated | E_TConfigUpdated ,
									  1,
									  0,
									  portMAX_DELAY);

		//usart_puts("\r\n exit Waiting event \r\n");

		if( (ebBits&E_MainScreen) == E_MainScreen )
		{
			Private_DisplayMainScreen();
		}
		else if( (ebBits&E_ConfigScreen) == E_ConfigScreen )
		{
			Private_DisplayConfigScreen();
		}
		else if( (ebBits&E_AlarmScreen) == E_AlarmScreen )
		{
			Private_DisplayAlarmScreen();
		}
		else if( (ebBits&E_CTempUpdated) == E_CTempUpdated )
		{
			Private_UpdateCTemp();
		}
		else if( (ebBits&E_TTempUpdated) == E_TTempUpdated )
		{
			Private_UpdateTTemp();
		}
		else if( (ebBits&E_ASUpdated) == E_ASUpdated )
		{
			Private_UpdateAS();
		}
		else if( (ebBits&E_ASUpdated) == E_TConfigUpdated )
		{
			//usart_puts("\r\nE_TConfigUpdated event \r\n");
			Private_UpdateTTempConfig();
		}




	}
}

void System_Init(void)
{

	/*initial system parameters*/
	TAS.SystemState = MainState;
	TAS.AS = ENABLED;
	TAS.CTemp = 0;
	TAS.TTemp = 30;

	/* lcd init */
	LCD_vidInit();

	/* ADC init */
	ADC_voidInit();

	/**/
	DIO_voidSetPinDirection(DIO_u8_PORTD, DIO_u8_PIN2, DIO_u8_OUTPUT);


	/* uart init*/
	usart_init(9600);
	usart_puts("System started\r\n");

}





/* private to T_Display task */
static void	Private_DisplayMainScreen(void)
{
	LCD_vidSendCommand(lcd_Clear);
	LCD_vidWriteString("C:");
	LCD_vidWriteNumber(TAS.CTemp);
	LCD_vidWriteString("  ");
	LCD_vidWriteString("T:");
	LCD_vidWriteNumber(TAS.TTemp);
	LCD_vidWriteString("  ");
	LCD_vidWriteString("AS:");
	if(TAS.AS) /* 1 >> enabled */
	{
		LCD_vidWriteCharctr('E');
	}
	else
	{
		LCD_vidWriteCharctr('D');
	}
	LCD_vidGotoxy(2,4);
	LCD_vidWriteString("TC:C   AST:T");

}
static void	Private_DisplayConfigScreen(void)
{
	LCD_vidSendCommand(lcd_Clear);
	LCD_vidWriteString("    T:");
	LCD_vidGotoxy(2,2);
	LCD_vidWriteString("OK:O    Canc:C");

}
static void	Private_DisplayAlarmScreen(void)
{
	LCD_vidSendCommand(lcd_Clear);
	LCD_vidWriteString("ALARMING!!");
	LCD_vidGotoxy(2,4);
	LCD_vidWriteString("STOP:S");
}
static void	Private_UpdateCTemp(void)
{
	LCD_vidGotoxy(1,1);
	LCD_vidWriteString("C:");
	LCD_vidWriteNumber(TAS.CTemp);
	LCD_vidWriteString("xx");
	LCD_vidWriteString("T:");
	LCD_vidWriteNumber(TAS.TTemp);
	LCD_vidWriteString("xx");
}
static void	Private_UpdateTTemp(void)
{
	LCD_vidGotoxy(1,1);
	LCD_vidWriteString("C:");
	LCD_vidWriteNumber(TAS.CTemp);
	LCD_vidWriteString("xx");
	LCD_vidWriteString("T:");
	LCD_vidWriteNumber(TAS.TTemp);
	LCD_vidWriteString("xx");
}
static void	Private_UpdateAS(void)
{
	LCD_vidGotoxy(1,16);
	if(TAS.AS) /* 1 >> enabled */
	{
		LCD_vidWriteCharctr('E');
	}
	else
	{
		LCD_vidWriteCharctr('D');
	}
}


static void	Private_UpdateTTempConfig(void)
{
	LCD_vidGotoxy(1, 8 + g_temp_TTemp.i);

	//usart_puts("\r\ng_temp_TTemp.strTTemp = ");
	//usart_putc(g_temp_TTemp.strTTemp[g_temp_TTemp.i] + 48);
	//usart_puts("\r\n");

	LCD_vidWriteCharctr( g_temp_TTemp.strTTemp[g_temp_TTemp.i] + 48 );
}

