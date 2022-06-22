#include "ccontrol.h"
#include "adc.h"
#include "config.h"
#include "arm_math.h"
#include "tim1.h"

u8 DMA_Flag = 0;
u8 DMA_Run = 0,CC_Run = 0;
static arm_pid_instance_f32 PID_Instance;
float Aim_Current = 0.0f;

static float Filter_ADC(u16* buff,u16 len)
{
	#warning Todo:Add filter
	static u16 value = 0;
	LOWPASS_FAST(value,buff[1],LOWPASS_COEFF);
	return ADC_VREF*value/4096;
}

static float Convert_ADC2Current(float adc)
{
	#warning Todo:Add Algorism
	return 10 * adc;
}



void Controller_Init(void)
{
	PID_Instance.Kp = PID_P_DEFAULT;
	PID_Instance.Ki = PID_I_DEFAULT;
	PID_Instance.Kd = PID_D_DEFAULT;
	arm_pid_init_f32(&PID_Instance,1);
}

void Controller_SetPID(float p, float i, float d, u8 reset)
{
	if(p >= 0)
		PID_Instance.Kp = p;
	if(i >= 0)
		PID_Instance.Ki = i;
	if(d >= 0)
		PID_Instance.Kd = d;
	arm_pid_init_f32(&PID_Instance,reset);
}

void Controller_ResetPWM(void)
{
	arm_pid_reset_f32(&PID_Instance);
}

float Control_FeedForward(float current)
{
	#warning Todo:Add Controller
	return 0.1;
}

void Current_Control(void)
{
	if(CC_Run == 0)
		return;
	
	float adc = Filter_ADC(ADC_BUFF,ADC_DMA_LEN);
	float current = Convert_ADC2Current(adc);
	volatile float out = arm_pid_f32(&PID_Instance,Aim_Current - current);
	
	Tim1_SetPWM4(out);
	
}


void DMA2_Stream0_IRQHandler()
{
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		ADC_My_StopDMA();
		DMA_Flag = 1;
		
#if DMA_WAIT_CONTROL==0
		ADC_My_StartDMA();
#endif
		
	}
}





































