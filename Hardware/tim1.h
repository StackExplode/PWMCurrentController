#ifndef __TIM1_H__
#define __TIM1_H__

#include "sys.h"

void Tim1_Init(void);
void Tim1_SetPWM1(float percent);
void Tim1_SetPWM4(float percent);
void Tim1_SetPWM_ADC_PULSE(float percent);

#define TIM1_PWM1_ON()		(TIM1->CCER |= 1)
#define TIM1_PWM1_OFF()		(TIM1->CCER &= ~1)

#define TIM1_PWM4_ON()		(TIM1->CCER |= (1 << 12))
#define TIM1_PWM4_OFF()		(TIM1->CCER &= ~(1 << 12))

#endif
