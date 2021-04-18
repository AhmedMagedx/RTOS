#ifndef HAL_SENSORS_SENSORS_H_
#define HAL_SENSORS_SENSORS_H_

//#include "ADC_interface.h"
#include "STD_TYPES.h"


#define TEMP_SENSOR_CH 0

ERROR_t TEMP_u16_Read(u16 * pTemp);
ERROR_t Humit_u16_Read(u8 * pHumi);

#define ADC0 0
#define ADC1 1
#define ADC2 2
#define ADC3 3
#define ADC4 4
#define ADC5 5
#define ADC6 6
#define ADC7 7

#endif /* HAL_SENSORS_SENSORS_H_ */
