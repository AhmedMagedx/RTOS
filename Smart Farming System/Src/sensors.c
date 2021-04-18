#include "sensors.h"
#include "STD_TYPES.h"
#include "ADC_interface.h"
ERROR_t TEMP_u16_Read(u16 * pTemp)
{
	u16 adc_read;
	//u8 str[5];
#if 0
	if(E_OK == ADC_u16GetDigitalValue_NonBlocking(ADC0, &adc_read))
	{
		//itoa(adc_read, str, 10);
		//usart_puts("ADC: ");
		//usart_puts(str);
		//usart_puts("\r\n");

		*pTemp = (adc_read*150*5)/(1023*1.5);
		return E_OK;
	}
#else
	/* ILLEGAL NON Blocking */
	adc_read = ADC_u16GetDigitalValue(0);

	*pTemp = (u16)(adc_read * 48.80) / 100;
	//*pTemp = (adc_read*150*50)/(1023*15);


	//itoa(*pTemp, str, 10);
	//usart_puts("Tmp: ");
	///usart_puts(str);
	//usart_puts("\r\n");

#endif

	return E_OK;

}

ERROR_t Humit_u16_Read(u8 * pHumi)
{

	u16 adc_read;
	//u8 str[5];


#if 0
	if(E_OK == ADC_u16GetDigitalValue_NonBlocking(ADC1, &adc_read))
	{

		*pHumi = adc_read;
		return E_OK;
	}
#else
	/* ILLEGAL NON Blocking */
	adc_read = ADC_u16GetDigitalValue(1);

	/* H = 48(V - 0.5) from datasheet
	 * V = (adcRead * 5) / 1023 from adc resolution
	 * H = 48( ((adcRead*5)/1023)  - 0.5)
	 * 	 = 0.234 * adcRead - 24*/

	/*
	itoa(adc_read, str, 10);
	usart_puts("adc: ");
	usart_puts(str);
	usart_puts("\r\n");
    */

	*pHumi = (u16)(adc_read * 48.80) / 100;
	//*pHumi = (u16)( (0.2346 * adc_read) - 69);
	//*pTemp = (adc_read*150*50)/(1023*15);


	//itoa(*pHumi, str, 10);
	//usart_puts("humi: ");
	//usart_puts(str);
	//usart_puts("\r\n");



#endif
	return E_OK;

}
