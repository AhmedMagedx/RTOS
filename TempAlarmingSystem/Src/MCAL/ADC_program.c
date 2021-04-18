/***************************************************************************************/
/****************************  IMT School Training Center ******************************/
/***************************************************************************************/
/** This file is developed by IMT School training center, All copyrights are reserved **/
/***************************************************************************************/



#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "ADC_interface.h"
#include "ADC_register.h"

void ADC_voidInit(void)
{

	/* Select the Voltage reference AVCC */
	CLR_BIT(ADMUX,ADMUX_REFS1);
	SET_BIT(ADMUX,ADMUX_REFS0);

	/* Select the right Adjustment */
	CLR_BIT(ADMUX,ADMUX_ADLAR);
	/* Select The Prescaler value => 64 */
	SET_BIT(ADCSRA,ADCSRA_ADPS2);
	SET_BIT(ADCSRA,ADCSRA_ADPS1);
	CLR_BIT(ADCSRA,ADCSRA_ADPS0);

	/* Enable ADC peripheral */
	SET_BIT(ADCSRA,ADCSRA_ADEN);

}

u16  ADC_u16GetDigitalValue (u8 Copy_u8ChannelNb)
{
	/* Clear The Channel Number bits */
		ADMUX &= (ADMUX_CHANNEL_NUM_MASK);
		/* Select The Channel Number */
		ADMUX |= Copy_u8ChannelNb;

		/* Start Conversion */
		SET_BIT(ADCSRA,ADCSRA_ADSC);

		/* Polling on the flag */
		while(GET_BIT(ADCSRA,ADCSRA_ADIF) == 0) { vTaskDelay(10);}

		/* Clear Flag */
		SET_BIT(ADCSRA,ADCSRA_ADIF);

		/* Read the ADC value */
		return ADC;
}

ERROR_t  ADC_u16GetDigitalValue_NonBlocking(u8 Copy_u8ChannelNb, u8* pData)
{
		/* Clear The Channel Number bits */
		ADMUX &= (ADMUX_CHANNEL_NUM_MASK);
		/* Select The Channel Number */
		ADMUX |= Copy_u8ChannelNb;

		/* Start Conversion */ /* Start conv. on start conv. ?!*/
		if( GET_BIT(ADCSRA, ADCSRA_ADSC))
		{
			/* Already Set */
		}
		else
		{
			SET_BIT(ADCSRA, ADCSRA_ADSC);
		}


		/* checking the flag */
		if( GET_BIT(ADCSRA, ADCSRA_ADIF) == 0)	/*if not ready */
		{
			return PENDING;
		}


		/* Clear Flag */
		SET_BIT(ADCSRA,ADCSRA_ADIF);

		*pData = ADC;
		/* Read the ADC value */
		return E_OK;
}
