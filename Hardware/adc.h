#ifndef __ADC_H__
#define __ADC_H__

#include "sys.h"

//#define VREFF_CAL	(*(unsigned short*)0x1FFFF7BA)	//Cannot be used in STM32F4/F1 serise

extern u16 ADC_BUFF[];

void ADC_My_Init(void);
void ADC_My_StartDMA(void);
void ADC_My_StopDMA(void);
u16 ADC_My_ReadVal(void);
u16 ADC_Zero_Cal(void);

#endif
