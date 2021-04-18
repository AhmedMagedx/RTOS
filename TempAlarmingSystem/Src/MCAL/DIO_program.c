/***************************************************************************************/
/****************************  IMT School Training Center ******************************/
/***************************************************************************************/
/** This file is developed by IMT School training center, All copyrights are reserved **/
/***************************************************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "DIO_register.h"



/* IO Pins */
void DIO_voidSetPinValue(u8 u8PortIdCopy , u8 u8PinIdCopy, u8 u8PinValCopy)
{
	/* Make sure that the Port ID and Pin ID are in the valid range */
	if ((u8PortIdCopy <= DIO_u8_PORTD) && (u8PinIdCopy <= DIO_u8_PIN7))
	{
		if (u8PinValCopy == DIO_u8_HIGH)
		{
			/* Check on the Required PORT Number */
			switch (u8PortIdCopy)
			{
				case DIO_u8_PORTA: SET_BIT(PORTA_Register,u8PinIdCopy); break;
				case DIO_u8_PORTB: SET_BIT(PORTB_Register,u8PinIdCopy); break;
				case DIO_u8_PORTC: SET_BIT(PORTC_Register,u8PinIdCopy); break;
				case DIO_u8_PORTD: SET_BIT(PORTD_Register,u8PinIdCopy); break;
			}
		
		}
		
		else if (u8PinValCopy == DIO_u8_LOW)
		{
			/* Check on the Required PORT Number */
			switch (u8PortIdCopy)
			{
				case DIO_u8_PORTA: CLR_BIT(PORTA_Register,u8PinIdCopy); break;
				case DIO_u8_PORTB: CLR_BIT(PORTB_Register,u8PinIdCopy); break;
				case DIO_u8_PORTC: CLR_BIT(PORTC_Register,u8PinIdCopy); break;
				case DIO_u8_PORTD: CLR_BIT(PORTD_Register,u8PinIdCopy); break;
			}
		}
	
		else
		{
			/* Do nothing, Wrong value to set */
		}
	}
	
	else
	{
		/* Do nothing, Error in the Pin ID or PORT ID */
	}
}

u8 DIO_u8GetPinValue(u8 u8PortIdCopy, u8 u8PinIdCopy)
{
	/* Define Local Variable to get the BIT Value */
	u8 u8ResultLocal;
	/* Make sure that the Port ID and Pin ID are in the valid range */
	if ((u8PortIdCopy <= DIO_u8_PORTD) && (u8PinIdCopy <= DIO_u8_PIN7))
	{
		/* Check on the Required PORT Number */
		switch (u8PortIdCopy)
		{
			case DIO_u8_PORTA: u8ResultLocal= GET_BIT(PINA_Register,u8PinIdCopy); break;
			case DIO_u8_PORTB: u8ResultLocal= GET_BIT(PINB_Register,u8PinIdCopy); break;
			case DIO_u8_PORTC: u8ResultLocal= GET_BIT(PINC_Register,u8PinIdCopy); break;
			case DIO_u8_PORTD: u8ResultLocal= GET_BIT(PIND_Register,u8PinIdCopy); break;
		}
	}
	
	else
	{
		/* return 0xff in case of error in the Pin ID or PORT ID */
		u8ResultLocal = 0xFF;
	}
	
	return u8ResultLocal;
}

void DIO_voidSetPinDirection  (u8 u8PortIdCopy, u8 u8PinIdCopy, u8 u8PinDirCopy)
{
	/* Make sure that the Port ID and Pin ID are in the valid range */
	if ((u8PortIdCopy <= DIO_u8_PORTD) && (u8PinIdCopy <= DIO_u8_PIN7))
	{
		if ( u8PinDirCopy == DIO_u8_OUTPUT )
		{
			/* Check on the Required PORT Number */
			switch (u8PortIdCopy)
			{
				case DIO_u8_PORTA: SET_BIT(DDRA_Register,u8PinIdCopy); break;
				case DIO_u8_PORTB: SET_BIT(DDRB_Register,u8PinIdCopy); break;
				case DIO_u8_PORTC: SET_BIT(DDRC_Register,u8PinIdCopy); break;
				case DIO_u8_PORTD: SET_BIT(DDRD_Register,u8PinIdCopy); break;
			}
		}
		
		else if ( u8PinDirCopy == DIO_u8_INPUT )
		{
			/* Check on the Required PORT Number */
			switch (u8PortIdCopy)
			{
				case DIO_u8_PORTA: CLR_BIT(DDRA_Register,u8PinIdCopy); break;
				case DIO_u8_PORTB: CLR_BIT(DDRB_Register,u8PinIdCopy); break;
				case DIO_u8_PORTC: CLR_BIT(DDRC_Register,u8PinIdCopy); break;
				case DIO_u8_PORTD: CLR_BIT(DDRD_Register,u8PinIdCopy); break;
			}
		}
		
		else
		{
			/* Do nothing, Wrong Direction Required */
		}
	}
	
	else
	{
		/* Do nothing, Error in the Pin ID or PORT ID */
	}
}


/* IO Ports */
void DIO_voidSetPortDirection (u8 u8PortId, u8 u8PortDir)
{
	/* Check on the Required PORT Number */
	switch (u8PortId)
	{
		case     DIO_u8_PORTA: DDRA_Register = u8PortDir; break;
		case     DIO_u8_PORTB: DDRB_Register = u8PortDir; break;
		case     DIO_u8_PORTC: DDRC_Register = u8PortDir; break;
		case     DIO_u8_PORTD: DDRD_Register = u8PortDir; break;
		default: /* Wrong Port ID */       break;
	}
}

void DIO_voidSetPortValue     (u8 u8PortId, u8 u8PortVal)
{
	/* Check on the Required PORT Number */
	switch (u8PortId)
	{
		case     DIO_u8_PORTA: PORTA_Register = u8PortVal; break;
		case     DIO_u8_PORTB: PORTB_Register = u8PortVal; break;
		case     DIO_u8_PORTC: PORTC_Register = u8PortVal; break;
		case     DIO_u8_PORTD: PORTD_Register = u8PortVal; break;
		default: /* Wrong Port ID */        break;
	}
}
