/***************************************************************************************/
/****************************  IMT School Training Center ******************************/
/***************************************************************************************/
/** This file is developed by IMT School training center, All copyrights are reserved **/
/***************************************************************************************/
/* Description ! 16x2 Character LCD for chip LMB161A                                   */
/* Release Date! 28/6/2016                                                             */
/* Version     ! V01                                                                   */
/***************************************************************************************/

/********************************** Header Files INCLUSIONS ****************************/
#define F_CPU 8000000UL
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <util/delay.h>

#include "DIO_interface.h"
#include "LCD_interface.h"
#include "LCD_Extra.h"
#include "LCD_config.h"
/***************************************************************************************/

/*
 -----------                   ----------
 | ATmega32  |                 |   LCD    |
 |           |                 |          |
 |        PD7|---------------->|D7        |
 |        PD6|---------------->|D6        |
 |        PD5|---------------->|D5        |
 |        PD4|---------------->|D4        |
 |        PD3|---------------->|D3        |
 |        PD2|---------------->|D2        |
 |        PD1|---------------->|D1        |
 |        PD0|---------------->|D0        |
 |           |                 |          |
 |        PA2|---------------->|E         |
 |        PA1|---------------->|RW        |
 |        PA0|---------------->|RS        |
 -----------                   ----------
 */

/***************************************************************************************/
/********************************** Function Definition ********************************/
/***************************************************************************************/



/***************************************************************************************/
/********************************** Constant Variables *********************************/
/***************************************************************************************/



/***************************************************************************************/
/* Description! Apply initialization sequence for LCD module                           */
/* Input      ! Nothing                                                                */
/* Output     ! Nothing                                                                */
/***************************************************************************************/
void LCD_vidInit(void)
{
	/* Init the Direction for the Control and the data pins as an output */
	DIO_voidSetPortDirection(LCD_DATA_PORT,0xff);
	DIO_voidSetPinDirection(LCD_CONTROL_PORT,LCD_RS_PIN,DIO_u8_OUTPUT);
	DIO_voidSetPinDirection(LCD_CONTROL_PORT,LCD_RW_PIN,DIO_u8_OUTPUT);
	DIO_voidSetPinDirection(LCD_CONTROL_PORT,LCD_E_PIN,DIO_u8_OUTPUT);


	/* Delay 30ms to ensure the initialization of the LCD driver */
	_delay_ms(30);

	/* Return Home  */
	LCD_vidSendCommand(lcd_Home);
	_delay_ms(15);

	/* Function Set  */
	LCD_vidSendCommand(lcd_FunctionSet8bit);
	_delay_ms(1);



	/* Display ON OFF Control */
	LCD_vidSendCommand(lcd_DisplayOn);
	_delay_ms(1);

	/* Clear Display */
	LCD_vidSendCommand(lcd_Clear);
	_delay_ms(15);

	/* Entry Mode Set  */
	LCD_vidSendCommand(lcd_EntryMode);
	_delay_ms(2);

}


/***************************************************************************************/
/* Description! Interface to send the configuration commands to the LCD Driver         */
/* Input      ! Command number                                                         */
/* Output     ! Nothing                                                                */
/***************************************************************************************/
void LCD_vidSendCommand(u8 u8CmdCpy)
{

	/* Set RS to LOW */
	DIO_voidSetPinValue(LCD_CONTROL_PORT, LCD_RS_PIN, DIO_u8_LOW);

	/* Set R/W to LOW */
	DIO_voidSetPinValue(LCD_CONTROL_PORT, LCD_RW_PIN, DIO_u8_LOW);

	/* Set E to HIGH  */
	DIO_voidSetPinValue(LCD_CONTROL_PORT, LCD_E_PIN, DIO_u8_HIGH);

	/* Load Command on Data bus */
	DIO_voidSetPortValue(LCD_DATA_PORT, u8CmdCpy);

	/* Set E to LOW */
	DIO_voidSetPinValue(LCD_CONTROL_PORT, LCD_E_PIN, DIO_u8_LOW);

	/* Wait for E to settle */
	_delay_ms(1);

	/* Set E to HIGH */
	DIO_voidSetPinValue(LCD_CONTROL_PORT, LCD_E_PIN, DIO_u8_HIGH);

	/* Delay for 1ms to let the LCD execute command */
	_delay_ms(1);

}

/***************************************************************************************/
/* Description! Interface to write character on LCD screen                             */
/* Input      ! Data to send                                                           */
/* Output     ! Nothing                                                                */
/***************************************************************************************/
void LCD_vidWriteCharctr(u8 u8DataCpy)
{
	/* Set RS to High */
	DIO_voidSetPinValue(LCD_CONTROL_PORT, LCD_RS_PIN, DIO_u8_HIGH);

	/* Set R/W to LOW */
	DIO_voidSetPinValue(LCD_CONTROL_PORT, LCD_RW_PIN, DIO_u8_LOW);

	/* Set E to HIGH  */
	DIO_voidSetPinValue(LCD_CONTROL_PORT, LCD_E_PIN, DIO_u8_HIGH);

	/* Load Command on Data bus */
	DIO_voidSetPortValue(LCD_DATA_PORT, u8DataCpy);

	/* Set E to LOW */
	DIO_voidSetPinValue(LCD_CONTROL_PORT, LCD_E_PIN, DIO_u8_LOW);

	/* Wait for E to settle */
	_delay_ms(1);

	/* Set E to HIGH */
	DIO_voidSetPinValue(LCD_CONTROL_PORT, LCD_E_PIN, DIO_u8_HIGH);

	/* Delay to let the LCD Display the character */
	_delay_ms(1);

}

/***************************************************************************************/
/* Description! Interface to write string on LCD screen                                */
/* Input      ! Pointer to the string                                                  */
/* Output     ! Nothing                                                                */
/***************************************************************************************/
void LCD_vidWriteString (u8* pu8StringCpy)
{

	//	/* Local loop index */
	//	u8 u8Index = 0;
	u8 iteration=0;
	//	while (pu8StringCpy[u8Index] != '\0')
	for (iteration=0; pu8StringCpy[iteration]!= '\0' ; iteration++)
	{

		/* Write Character on LCD */
		LCD_vidWriteCharctr(pu8StringCpy[iteration]);

		//		/* Increment local loop index */
		//		u8Index++;

		/* Delay to let the LCD show the character */
		_delay_ms(2);
	}

}

/***************************************************************************************/
/* Description! Interface to start the writing process at certain digit in the LCD     */
/* Input      ! Y = Row position , X = Column position                                 */
/* Output     ! Nothing                                                                */
/***************************************************************************************/
void LCD_vidGotoxy (u8 Y,u8 X)
{
	if (X>0 && X<=16)
	{
		switch(Y)
		{
		case 1:
			LCD_vidSendCommand(0x80+X-1);
			break;
		case 2:
			LCD_vidSendCommand(0xC0+X-1);
			break;
		default:
			break;
		}
	}
}

/***************************************************************************************/
/* Description! Interface to write extra characters saved in the CGRAM                 */
/* Input      ! Y = Row position , X = Column position                                 */
/* Output     ! Nothing                                                                */
/***************************************************************************************/
void LCD_vidWriteExtraChar (u8 Y,u8 X)
{
	u8 iteration1,iteration2;

	/*DDRAM-->CGRAM*/
	LCD_vidSendCommand(64);
	for(iteration1=0 ; iteration1<64 ; iteration1++)
	{
		LCD_vidWriteCharctr(ExtraChar[iteration1]);
	}
	/*CGRAM-->DDRAM*/
	LCD_vidSendCommand(128);
	LCD_vidGotoxy(Y,X);
	/*First eight character which saved at CGRAM*/
	for (iteration2=0; iteration2<=7 ; iteration2++)
	{
		/* Write bytes of DDRAM */
		LCD_vidWriteCharctr(iteration2);

		_delay_ms(5);
	}
}
void LCD_vidWriteNumber(u16 Copy_u16Number)
{
	u8 Counter=0,Digits=0,Current;
	u16 CopyNumber=Copy_u16Number;
	while(CopyNumber)
	{
		CopyNumber/=10;
		Digits++;
	}
	CopyNumber=Copy_u16Number;
	for(Counter=0;Counter<Digits;Counter++)
	{
		Current=CopyNumber/(Private_GetPower(10,Digits-1-Counter));
		LCD_vidWriteCharctr(Current+'0');
		CopyNumber%=(Private_GetPower(10,Digits-1-Counter));
	}

}
static u16 Private_GetPower(u8 Num1, u8 Num2)
{
	u16 Result=1;
	u8 Counter=0;
	for(Counter=0;Counter<Num2;Counter++)
	{
		Result*=Num1;
	}
	return Result;
}
