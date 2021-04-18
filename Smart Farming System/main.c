/* include OS header file */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"


#include "App.h"



int main(void)
{
	System_Init();


	/* OS Object Creation */
	egControl = xEventGroupCreate();
	egDisplay = xEventGroupCreate();
	bsCheck = xSemaphoreCreateBinary();


	//xTaskCreate(T_Indication, )
	xTaskCreate(T_Display, 	 NULL, 170, NULL, 2, NULL);
	xTaskCreate(T_Sensing, 	 NULL, 100,  NULL, 3, NULL);
	xTaskCreate(T_Terminal,  NULL, 70, NULL, 4, NULL);
	xTaskCreate(T_SysCheck,  NULL, 70,  NULL, 5, NULL);
	xTaskCreate(T_Control,	 NULL, 150, NULL, 6, NULL);


	vTaskStartScheduler();

}



void T_Control(void* pvParam)
{
	//usart_puts("T_Control started...\r\n");
	u8 str[5] = {0};


	while(1)
	{

		ebControlBits = xEventGroupWaitBits(egControl, E_CONTROLMASK , 0, 0, portMAX_DELAY);
		//usart_puts("Run Control\r\n");

#if 0
		usart_puts("Running T_Control\r\n heater is ");  /**** DEBUG  ****/
		itoa( (ebControlBits & E_HEATERON) >> 1, str, 10);
		usart_puts(str);
		usart_puts("\r\n");  /**** DEBUG  ****/
		usart_puts("cooler is ");  /**** DEBUG  ****/
		itoa( (ebControlBits & E_COOLERON) >> 2, str, 10);
		usart_puts(str);
		usart_puts("\r\n");  /**** DEBUG  ****/
		usart_puts("water is ");  /**** DEBUG  ****/
		itoa( ebControlBits & E_PUMPON , str, 10);
		usart_puts(str);
		usart_puts("\r\n");  /**** DEBUG  ****/
#endif

		if ( (ebControlBits & E_HEATERON) == E_HEATERON )
		{
			//usart_puts("Run Control\r\n");
			if(Motors_State.Heater == ON)
			{
				DIO_voidSetPinValue(CONTROL_PORT ,  HEATER, DIO_u8_HIGH);
			}
			else
			{
				DIO_voidSetPinValue(CONTROL_PORT ,  HEATER, DIO_u8_LOW);
			}
		}

		if( (ebControlBits & E_COOLERON) == E_COOLERON )
		{
			if(Motors_State.Cooler == ON)
			{
				DIO_voidSetPinValue(CONTROL_PORT ,  COOLER, DIO_u8_HIGH);
			}
			else
			{
				DIO_voidSetPinValue(CONTROL_PORT ,  COOLER, DIO_u8_LOW);
			}
		}


		if( (ebControlBits & E_PUMPON) == E_PUMPON )
		{
			//usart_puts("E_PUMPON EVENT RECEIVED\r\n");

			if(Motors_State.Water_Pump == ON)
			{
				//usart_puts("PUMP_ON\r\n");
				DIO_voidSetPinValue(CONTROL_PORT ,  WATER_PUMP, DIO_u8_HIGH);
			}
			else
			{
				//usart_puts("PUMP_OFF\r\n");
				DIO_voidSetPinValue(CONTROL_PORT ,  WATER_PUMP, DIO_u8_LOW);
			}
		}

		vTaskDelay(5);
	}
}


void T_SysCheck(void* pvParam)
{
	//usart_puts("T_SysCheck started...\r\n");

	/* initial defaults */
	xEventGroupSetBits(egDisplay, E_MainScreen); /* for T_Display Task */

	xEventGroupClearBits(egControl, E_CONTROLMASK);


	while(1)
	{
		if(xSemaphoreTake(bsCheck, portMAX_DELAY))
		{
			//usart_puts("Running T_SysCheck\r\n");  /**** DEBUG  ****/

			if(SFS.SensorData.TempData > SFS.SensorThreshold.TempT)
			{
				Motors_State.Cooler = ON;
				Motors_State.Heater = OFF;

				//usart_puts("E_COOLERON \r\n");  /**** DEBUG  ****/


			}
			else if(SFS.SensorData.TempData < SFS.SensorThreshold.TempT)
			{
				//usart_puts("E_HEATERON \r\n");  /**** DEBUG  ****/

				Motors_State.Cooler = OFF;
				Motors_State.Heater = ON;

			}
			else  /* when equal */
			{
				Motors_State.Cooler = OFF;
				Motors_State.Heater = OFF;

			}

			xEventGroupSetBits(egControl, E_COOLERON | E_HEATERON);		/* TODO */
			vTaskDelay(10);
			xEventGroupClearBits(egControl, E_COOLERON | E_HEATERON);		/* TODO */


			if(SFS.SensorData.HumiData > SFS.SensorThreshold.HumiT)
			{
				//usart_puts("E_PUMPON \r\n");  /**** DEBUG  ****/
				Motors_State.Water_Pump = OFF;

			}
			else
			{

				Motors_State.Water_Pump = ON;
				//usart_puts("E_PUMPON \r\n");  /**** DEBUG  ****/

			}

			xEventGroupSetBits(egControl, E_PUMPON);
			vTaskDelay(10);
			xEventGroupClearBits(egControl, E_PUMPON);



			xEventGroupSetBits(egDisplay, E_MotorState); /* for T_Display Task */

		}

		//usart_puts("Running T_SysCheck...\r\n");

	}
}


void T_Terminal(void* pvParam)
{
	//usart_puts("T_Terminal started ...\r\n");
	u8 data;
	u8 strTTemp[4];
	u8 strTHumi[4];
	memset(strTTemp, 0, 3);
	memset(strTHumi, 0, 3);

	static enum {TempReceiving = 13, HumiReceiving} ReceivingState;

	ReceivingState = TempReceiving;  /* Default entry point */

	u8 i = 0;

	/* 	*/
	xEventGroupSetBits(egDisplay, E_MainScreen);

	while(1)
	{
		//usart_puts("Running T_Terminal\r\n");

		switch (ReceivingState)
		{
			case TempReceiving:
			{
				if(E_OK == usart_getc_NonBlocking(&data)) /* if new data exist on terminal */
				{
					//usart_putc(data);  /**** DEBUG  ****/
					if(MainState == SFS.SystemState )
					{
						if('C' == data)	/* the data is 'C' */
						{
							SFS.SystemState = ConfigState;
							i = 0; 	/* clearing index to start saving from zero in next config */
							memset(strTTemp, 0, 3);  /* clear temporary data for next config */
							xEventGroupSetBits(egDisplay, E_ConfigScreen); /* Display main */
						}
					}
					else if (ConfigState == SFS.SystemState)
					{
						if('C' == data)	/* the data is 'C' */
						{
							SFS.SystemState = MainState;
							xEventGroupSetBits(egDisplay, E_MainScreen); /* Display main */
						}
						else if( '9' >= data && '0' <= data)	/* the data is digit */
						{
							if(3 > i)	/* if we still did`t receive the third digit */
							{
								strTTemp[i] = data;
								i++;
							}
						}
						else if( 'O' == data)	/* the data is 'O' */
						{
							if( 0 == atoi(strTTemp) )	/* FIXME *//* if u forget to clear the buffer in some conditions, an error may appear here */
							{
								/* Do not update the global struct if no data exist */
							}
							else
							{
								SFS.SensorThreshold.TempT = atoi(strTTemp);	/* update global threshold*/
								xSemaphoreGive(bsCheck);
								xEventGroupSetBits(egDisplay, E_TTUpdated);
							}

							i = 0;
							ReceivingState = HumiReceiving; 	/* Go to Humidity receiving state */
							xEventGroupSetBits(egDisplay, E_Next); /* in both situation, move the cursor to humidity*/

						}
						else if( 'N' == data)	/* the data is 'N' */
						{
							i = 0; /* to start from zero in Humidity receiving */
							ReceivingState = HumiReceiving; 	/* Go to Humidity receiving state */
							xEventGroupSetBits(egDisplay, E_Next); /* tell the display to move the cursor */
						}
						else
						{
							/* Ignore, Unpredicted data entered*/
						}
					
					} /* end IF MainState | ConfigState */
					
				} /* end if Data exist in uart */

				break;
			}	/* end of case TempReceiving */
			case HumiReceiving:
			{
				if(E_OK == usart_getc_NonBlocking(&data)) /* if new data exist on terminal */
				{
					//usart_puts("Running T_Terminal\r\n");  /**** DEBUG  ****/

					if('C' == data)	/* the data is 'C' */
					{
						ReceivingState = TempReceiving;
						i = 0; 	/* clearing index to start saving from zero in next config */
						memset(strTHumi, 0, 3);  /* clear temporary data for next config */
						SFS.SystemState = MainState;
						xEventGroupSetBits(egDisplay, E_MainScreen); /* Display main */

					}
					else if( '9' >= data && '0' <= data)	/* the data is digit */
					{
						if(3 > i)	/* if we still did`t receive the third digit */
						{
							strTHumi[i] = data;
							i++;
						}
						
					}
					else if('O' == data)	/* the data is 'O' */
					{
						if( 0 == atoi(strTHumi) )  /* if u forget to clear the buffer in some conditions, an error may appear here */
						{
							/* Do not update the global struct if no data exist */

						}
						else
						{
							usart_puts(strTHumi);
							SFS.SensorThreshold.HumiT = atoi(strTHumi);	/* update global threshold*/
							LCD_goToRowColumn(1, 0);
							LCD_intgerToString(SFS.SensorThreshold.HumiT);
							u8 str[6];
							memset(str, 0 ,5);
							itoa(SFS.SensorThreshold.HumiT, str, 10);
							usart_puts(str);
							xSemaphoreGive(bsCheck);
							xEventGroupSetBits(egDisplay, E_HTUpdated);
						}

						ReceivingState = TempReceiving; /* next state */
						SFS.SystemState = MainState;
						xEventGroupSetBits(egDisplay, E_MainScreen); /* in both cases go to main screen */

					}
					else if('N' == data)	/* the data is 'N' */
					{
						ReceivingState = TempReceiving; 	/* Go to temp receiving state */
						SFS.SystemState = MainState;
						xEventGroupSetBits(egDisplay, E_MainScreen);
					}
					else
					{
						/* Ignore, Unpredicted data entered*/
					}
				}
				else
				{	
					/* if no data exist */
				}

				break;
			}
			default:
				break;
		}

		vTaskDelay(50);  /* Task periodicity is 50ms */
	}
}



void T_Sensing(void* pvParam)
{
	u8 tempValue = 0;
	u8 humiValue = 0;

	//usart_puts("T_Sensing started ...\r\n");

	while(1)
	{

		//usart_puts("Run Sensing\r\n");

		if(E_OK == TEMP_u16_Read(&tempValue))
		{

			//usart_puts("Temp rdy\r\n");

			if(  SFS.SensorData.TempData != tempValue )
			{
				SFS.SensorData.TempData= tempValue;
				xSemaphoreGive(bsCheck); /* syscheck */
				xEventGroupSetBits(egDisplay,E_TUpdated);

			}

		}
		else
		{
			//usart_puts("TempNOT rdy\r\n");
		}



		if(E_OK == Humit_u16_Read(&humiValue))
		{
		//	usart_puts("Humi data ready\r\n");

			if ( SFS.SensorData.HumiData!= humiValue )
			{
				SFS.SensorData.HumiData = humiValue;
				xSemaphoreGive(bsCheck);
				xEventGroupSetBits(egDisplay,E_HUpdated);

				//usart_puts("Running T_Sensing\r\n");
			}

		}
		else
		{
			//usart_puts("Humi data NOT ready\r\n");
		}

		vTaskDelay(500);  	/* Task periodicity is 1 sec */

	}
}


void T_Display(void* pvParam)
{

	//usart_puts("T_Display started\r\n");

	while(1)
	{
		ebDisplayBits = xEventGroupWaitBits(egDisplay,E_DISPLAYMASK,1,0,portMAX_DELAY);
		usart_puts("Running T_Display\r\n");
		if( (ebDisplayBits & E_MainScreen) == E_MainScreen)
		{
			vTaskDelay(2000);

			if( MainState == SFS.SystemState )
			{
				LCD_clearScreen();
				LCD_displayString(LCD_MAIN_SCREEN_L1);

				LCD_goToRowColumn(0,LCD_TEMP_COL);
				LCD_displayString("   ");
				LCD_goToRowColumn(0,LCD_TEMP_COL);
				LCD_intgerToString(SFS.SensorData.TempData);

				LCD_goToRowColumn(0,LCD_HUMI_COL);
				LCD_displayString("   ");
				LCD_goToRowColumn(0,LCD_HUMI_COL);
				LCD_intgerToString(SFS.SensorData.HumiData);

				LCD_goToRowColumn(1,0);
				LCD_displayString(LCD_MAIN_SCREEN_L2);

				LCD_goToRowColumn(1,LCD_TEMP_COL);
				LCD_displayString("   ");
				LCD_goToRowColumn(1,LCD_TEMP_COL);
				LCD_intgerToString(SFS.SensorThreshold.TempT);
				//LCD_goToRowColumn(1,LCD_TEMP_COL);

				LCD_goToRowColumn(1,LCD_HUMI_COL);
				LCD_displayString("   ");
				LCD_goToRowColumn(1,LCD_HUMI_COL);
				LCD_intgerToString(SFS.SensorThreshold.HumiT);

				LCD_goToRowColumn(2,0);
				LCD_displayString(LCD_MAIN_SCREEN_L3);
				
				if(Motors_State.Water_Pump == ON)
				{
					LCD_goToRowColumn(2,LCD_PUMP_COL);
					LCD_displayString("   ");
					LCD_goToRowColumn(2,LCD_PUMP_COL);
					LCD_displayString("ON");
				}
				else
				{
					LCD_goToRowColumn(2,LCD_PUMP_COL);
					LCD_displayString("   ");
					LCD_goToRowColumn(2,LCD_PUMP_COL);
					LCD_displayString("OFF");
				}

				if(Motors_State.Heater == ON)
				{
					LCD_goToRowColumn(2,LCD_HEATER_COL);
					LCD_displayString("   ");
					LCD_goToRowColumn(2,LCD_HEATER_COL);
					LCD_displayString("ON");
				}
				else
				{
					LCD_goToRowColumn(2,LCD_HEATER_COL);
					LCD_displayString("   ");
					LCD_goToRowColumn(2,LCD_HEATER_COL);
					LCD_displayString("OFF");
				}

				if(Motors_State.Cooler == ON)
				{
					LCD_goToRowColumn(2,LCD_COOLER_COL);
					LCD_displayString("   ");
					LCD_goToRowColumn(2,LCD_COOLER_COL);
					LCD_displayString("ON");
				}
				else
				{
					LCD_goToRowColumn(2,LCD_COOLER_COL);
					LCD_displayString("   ");
					LCD_goToRowColumn(2,LCD_COOLER_COL);
					LCD_displayString("OFF");
				}
				
				
				
				LCD_goToRowColumn(3,0);
				LCD_displayString(LCD_MAIN_SCREEN_L4);
				
			} /*end of if MainState*/
			

		}

		if( (ebDisplayBits & E_ConfigScreen) == E_ConfigScreen)
		{
			if(ConfigState == SFS.SystemState)
			{			
				LCD_clearScreen();
				LCD_displayString(LCD_CONFIG_SCREEN_L1);
				LCD_displayString(LCD_CONFIG_SCREEN_L2);
				LCD_displayString(LCD_CONFIG_SCREEN_L4);

				LCD_goToRowColumn(0,LCD_CONFIG_COL);
				LCD_sendCommand(CURSOR_BLINK);
			}

		}
		
		if( (ebDisplayBits & E_TTUpdated) == E_TTUpdated)
		{
			LCD_goToRowColumn(0,LCD_CONFIG_COL);
			LCD_sendCommand(CURSOR_OFF);
			LCD_intgerToString(SFS.SensorThreshold.TempT);
		}

		if( (ebDisplayBits & E_HTUpdated) == E_HTUpdated)
		{
			usart_puts("E_HTUpdated");
			LCD_goToRowColumn(2,LCD_CONFIG_COL);
			LCD_sendCommand(CURSOR_OFF);
			LCD_intgerToString(SFS.SensorThreshold.HumiT);
		}

		if( (ebDisplayBits & E_Next) == E_Next)
		{
			LCD_goToRowColumn(2,LCD_CONFIG_COL);
			LCD_sendCommand(CURSOR_BLINK);
		}

		if( (ebDisplayBits & E_TUpdated) == E_TUpdated)
		{
			LCD_goToRowColumn(0,LCD_TEMP_COL);
			LCD_displayString("   ");
			LCD_goToRowColumn(0,LCD_TEMP_COL);
			LCD_intgerToString(SFS.SensorData.TempData);
		}

		if( (ebDisplayBits & E_HUpdated) == E_HUpdated)
		{
			LCD_goToRowColumn(0,LCD_HUMI_COL);
			LCD_displayString("   ");
			LCD_goToRowColumn(0,LCD_HUMI_COL);
			LCD_intgerToString(SFS.SensorData.HumiData);
		}


if(MainState == SFS.SystemState)
{
	if( (ebDisplayBits & E_MotorState) == E_MotorState)
	{
		if(Motors_State.Water_Pump == ON)
		{
			LCD_goToRowColumn(2,LCD_PUMP_COL);
			LCD_displayString("   ");
			LCD_goToRowColumn(2,LCD_PUMP_COL);
			LCD_displayString("ON");
		}
		else
		{
			LCD_goToRowColumn(2,LCD_PUMP_COL);
			LCD_displayString("   ");
			LCD_goToRowColumn(2,LCD_PUMP_COL);
			LCD_displayString("OFF");
		}

		if(Motors_State.Heater == ON)
		{
			LCD_goToRowColumn(2,LCD_HEATER_COL);
			LCD_displayString("   ");
			LCD_goToRowColumn(2,LCD_HEATER_COL);
			LCD_displayString("ON");
		}
		else
		{
			LCD_goToRowColumn(2,LCD_HEATER_COL);
			LCD_displayString("   ");
			LCD_goToRowColumn(2,LCD_HEATER_COL);
			LCD_displayString("OFF");
		}

		if(Motors_State.Cooler == ON)
		{
			LCD_goToRowColumn(2,LCD_COOLER_COL);
			LCD_displayString("   ");
			LCD_goToRowColumn(2,LCD_COOLER_COL);
			LCD_displayString("ON");
		}
		else
		{
			LCD_goToRowColumn(2,LCD_COOLER_COL);
			LCD_displayString("   ");
			LCD_goToRowColumn(2,LCD_COOLER_COL);
			LCD_displayString("OFF");
		}
	} /* if motorState*/
	
}

		
		
	}
}




void T_Indication(void* pvParam)
{
	EventBits_t ebControlBits;
	//usart_puts("T_Indication started ...\r\n");

		while (1)
		{
			//usart_puts("Running T_Indication\r\n");
			ebControlBits = xEventGroupWaitBits(egControl, E_HEATERON | E_COOLERON |E_PUMPON ,0,0,portMAX_DELAY);
			if( ( (ebControlBits & E_HEATERON) == E_HEATERON) || ( (ebControlBits & E_COOLERON) == E_COOLERON ) || ( (ebControlBits & E_PUMPON) == E_PUMPON))
			{

				DIO_voidSetPinValue(DIO_u8_PORTA ,  DIO_u8_PIN0, DIO_u8_HIGH);
				vTaskDelay(500);
				DIO_voidSetPinValue(DIO_u8_PORTA ,  DIO_u8_PIN0, DIO_u8_LOW);
				vTaskDelay(500);

			}
		}
}














void System_Init(void)
{

	/*initial system parameters*/


	/* lcd init */
	LCD_init();

	/* ADC init */
	ADC_voidInit();


	/* MOTORS */
	DIO_voidSetPinDirection(CONTROL_PORT, WATER_PUMP, DIO_u8_OUTPUT);
	DIO_voidSetPinDirection(CONTROL_PORT, HEATER, DIO_u8_OUTPUT);
	DIO_voidSetPinDirection(CONTROL_PORT, COOLER, DIO_u8_OUTPUT);

	/* initialize to defaults Zero*/
	DIO_voidSetPinValue(CONTROL_PORT, WATER_PUMP, DIO_u8_LOW);
	DIO_voidSetPinValue(CONTROL_PORT, HEATER, DIO_u8_LOW);
	DIO_voidSetPinValue(CONTROL_PORT, COOLER, DIO_u8_LOW);

	/* indication led */
	DIO_voidSetPinDirection(DIO_u8_PORTB, DIO_u8_PIN0, DIO_u8_OUTPUT);
	DIO_voidSetPinValue(DIO_u8_PORTB, DIO_u8_PIN0, DIO_u8_LOW);


	/*Defaults initialize */
	SFS.SensorThreshold.TempT = 20;
	SFS.SensorThreshold.HumiT = 30;




	/* uart init*/
	usart_init(9600);
	usart_puts("System started\r\n");


}

