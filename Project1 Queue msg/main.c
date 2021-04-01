/*
 * main.c
 *
 *  Created on: Mar 30, 2021
 *      Author: MaKh
 */
#define F_CPU (8000000UL)


/* OS Header files */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "util/delay.h"


/* program headers*/
#include "DIO_interface.h"
#include "usart_driver.h"
#include "STD_TYPES.h"
#include "string.h"
#include "stdlib.h"

/* Tasks Proto. or functions */
void T_UserInput(void* pvParam);
void T_UserOutput(void* pvParam);
void T_SystemIndication(void* pvParam);

void system_init(void);

/* OS Object Decl. */
SemaphoreHandle_t			bsUartRes = NULL;
QueueHandle_t qStudentStruct;


typedef struct
{
	u8 Name[15];
	u8 Mark;
} student_t;



int main(void)
{

	/* Init. Pr. */
	system_init();

	/* OS Object Creat. */
	qStudentStruct = xQueueCreate(3, 20); /* msg size Should be 16bytes or more */

	/* Tasks Creation */
	xTaskCreate(T_SystemIndication,NULL,100,NULL,1,NULL);
	xTaskCreate(T_UserInput,NULL,100,NULL,2,NULL);
	xTaskCreate(T_UserOutput,NULL,250,NULL,3,NULL); /* Higher priority */

	/* OS Start or sched */
	vTaskStartScheduler();

}


void T_UserInput(void* pvParam)
{
	student_t Student;

	u8 ucData = 0;
	u8 strMark[2];
	while(1)
	{
		if( E_OK == usart_getc_NonBlocking(&ucData) )
		{
			u8 i = 0;
			memset(Student.Name, 0, 14);
			while(ucData != '\r') /* while Enter key not pressed yet */
			{
				/* Waiting to receive the full name of buffer full */
				Student.Name[i] = ucData;
				i++;

				if(i>14)
				{
					break;
				}

				while( PENDING == usart_getc_NonBlocking(&ucData))
				{
					vTaskDelay(5);
				}
				//usart_puts("Task Input While\r\n");
			} 	/* end While the enter not pressed or buffer full */

			while( PENDING == usart_getc_NonBlocking(&ucData))
			{
				vTaskDelay(5);
			}

			Student.Mark = 0;
			memset(strMark, 0, 2);
			i = 0;

			while(ucData != '\r')
			{
				/* Waiting to receive the mark of buffer full */
				strMark[i] = ucData;
				i++;

				if(i>2)
				{
					break;
				}

				while( PENDING == usart_getc_NonBlocking(&ucData))
				{
					vTaskDelay(5);
				}
			}

#if 0
			usart_puts("Student Name :");
			usart_puts(Student.Name);
			usart_puts("\r\n");

			usart_puts("Student Mark :");
			usart_puts(strMark);
			usart_puts("\r\n");
#endif


			Student.Mark = atoi(strMark);		/* Convert from ascii to integer */


			xQueueSend(qStudentStruct, &Student, portMAX_DELAY);	/* Send the student data to the receiving task */


		}
		else
		{

		}

		vTaskDelay(10);

	}
}



void T_UserOutput(void* pvParam)
{
	student_t StudentList[10];
	u8 i = 0;
	u8 strMark[4];

	/* initializing the student list with Zeros */
	for(i = 0; i < 10; i++)
	{
		memset( StudentList[i].Name, 0, 15);
		StudentList[i].Mark = 0;
	}


	i=0;


	while(1)
	{


		if(xQueueReceive(qStudentStruct, &StudentList[i], portMAX_DELAY))
		{

			usart_puts("New Student data: \r\nStudent ");
			usart_putc(	i  + 1 + 48 );
			usart_puts(":\r\n");
			usart_puts(StudentList[i].Name );
			usart_puts("\r\n");
			itoa(StudentList[i].Mark, strMark, 10);
			usart_puts(strMark);
			usart_puts("\r\n");


			i++;

		}
		else
		{

		}

	}
}



void T_SystemIndication(void* pvParam)
{
	while(1)
	{
		PORTC ^= 0xFF;
		vTaskDelay(1000);
	}
}






void system_init(void)
{



	/* leds  */
	DIO_voidSetPortDirection(DIO_u8_PORTC ,DIO_u8_OUTPUT);


	DIO_voidSetPortValue(DIO_u8_PORTC, DIO_u8_LOW);


	/* UART */
	usart_init(9600);
	usart_puts("System Started...\r\n");

}
