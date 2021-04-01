/***************************************************************************************/
/****************************  IMT School Training Center ******************************/
/***************************************************************************************/
/** This file is developed by IMT School training center, All copyrights are reserved **/
/***************************************************************************************/
#ifndef _DIO_INT_H
#define _DIO_INT_H
#include "STD_TYPES.h"

/* Port Defines */
#define DIO_u8_PORTA    0
#define DIO_u8_PORTB    1
#define DIO_u8_PORTC    2
#define DIO_u8_PORTD    3

/* PIN Defines */
#define DIO_u8_PIN0   0
#define DIO_u8_PIN1   1
#define DIO_u8_PIN2   2
#define DIO_u8_PIN3   3
#define DIO_u8_PIN4   4
#define DIO_u8_PIN5   5
#define DIO_u8_PIN6   6
#define DIO_u8_PIN7   7

/* PIN Directions */
#define DIO_u8_INPUT  0
#define DIO_u8_OUTPUT 1

/* PIN Value Options */
#define DIO_u8_HIGH   1
#define DIO_u8_LOW    0

/* IO Pins */
void DIO_voidSetPinValue      (u8 u8PortIdCopy , u8 u8PinIdCopy, u8 u8PinValCopy);

u8   DIO_u8GetPinValue        (u8 u8PortIdCopy, u8 u8PinIdCopy);

void DIO_voidSetPinDirection  (u8 u8PortIdCopy, u8 u8PinIdCopy, u8 u8PinDirCopy);

/* IO Ports */
void DIO_voidSetPortDirection (u8 u8PortId, u8 u8PortDir);

void DIO_voidSetPortValue     (u8 u8PortId, u8 u8PortVal);

#endif
