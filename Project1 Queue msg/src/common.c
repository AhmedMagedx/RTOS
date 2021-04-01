/*
 * common.c
 *
 *  Created on: Mar 28, 2021
 *      Author: MaKh
 */
#include "common.h"


ERROR_t delay_eNonBlocking_ms(const char Time_ms)
{
	static int locTime_uS = 0;
	static int locTime_mS = 0;
	/* 1 cycle time for 8MHz processor = 0.125 uS*/
	for(char locTemp = 0; locTemp<4; locTemp++)
	{
	/* for 8MHz processor, 4 cycle time = 1 uS*/
		/*	1 uS block */
	}

	locTime_uS++;		/* increament the microSecond var by 1 uS */

	if (locTime_uS > 999)
	{
		locTime_mS++;
		locTime_uS = 0;
	}

	//printf("%s %d %s %d\n", "MilliSecond is:",locTime_mS,"  MicroSecond is:",locTime_uS);
	if(locTime_mS >= Time_ms)
	{
		/* clearing the variables for the next call */
		locTime_mS = 0;
		locTime_uS = 0;
		return E_OK;
	}
	else
	{
		return PENDING;
	}
}
