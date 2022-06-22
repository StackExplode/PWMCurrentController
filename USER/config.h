#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "sys.h"

#define LOWPASS_FAST(value, sample, filter_constant)	(value -= (filter_constant) * ((value) - (sample)))

#define VERSION				"1.01a"

/*PWM Config*/
#define TIM1_FREQ			168000000u	//if 0 divide then tim1 freq=system freq=168MHz

#define PWM_COUNTER			1000
#define PWM_PRESCALER		4
//PWM frequency = TIM1_FREQ/PWM_PRESCALER/PWM_COUNTER
#define PWM_MIN_PERCENT		0.0f	//0.1=10% etc.
#define PWM_MAX_PERCENT		0.2f	//0.1=10% etc.
/*End of PWM Config*/

/*ADC Config*/
#define ADC_VREF			3.3	//Voltage of Vref+
#define ADC_SAMPLE_POS		0.9f	//When more than 0:Percentage of PWM, Less than 0:Fix n counts before PWM, 0:Sync with PWM
#define ADC_DMA_LEN			3
#define DMA_WAIT_CONTROL	0	//1=Start next DMA after control program, 0=Start next DMA immediately after last DMA TC without waiting control program
/*End of ADC Config*/

/*Control Config*/
#define PID_P_DEFAULT		0.01f
#define PID_I_DEFAULT		0.001f
#define PID_D_DEFAULT		0.001f
#define LOWPASS_COEFF		0.1	//Low pass filter coeffectient
#define DEFAULT_AIM			10.0	//Default aim current, unit is A
/*End of Control Config*/

#endif
