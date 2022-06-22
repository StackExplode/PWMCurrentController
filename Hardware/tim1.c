#include "tim1.h"
#include "config.h"


static void _Tim1_GPIO_Init(void)	//PA8->TIM1_CH1 PA11->TIM1_CH4
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11; //GPIOA8 11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA8 11
}


static void _Tim1_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //��ʱ�� TIM1 ʹ��
	
	TIM_TimeBaseInitTypeDef tm;
	tm.TIM_Prescaler = PWM_PRESCALER - 1;         
	tm.TIM_CounterMode = TIM_CounterMode_Up;  
	tm.TIM_Period = PWM_COUNTER - 1;  
	tm.TIM_ClockDivision = 0; //����Ƶ��	
	tm.TIM_RepetitionCounter=0;
	//tm.TIM_ClockDivision = TIM_CKD_DIV1;  
	TIM_TimeBaseInit(TIM1, &tm);  
	
	//TIM_DMAConfig(TIM1,Tim_dmabase_, TIM_DMABurstLength_1Transfer);
	TIM_DMACmd(TIM1, TIM_DMA_Update | TIM_DMA_Trigger | TIM_DMA_COM, ENABLE);
	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
	
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM1Ϊ����ռ�ձ�ģʽ��PWM2Ϊ������ģʽ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�,��Ч��ƽΪ�͵�ƽ
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//�ڿ���ʱ���     ��,��������ÿ��Ըı�TIM_OCPolarity ���û��䣬��1ͨ��������
	TIM_OCInitStructure.TIM_Pulse = 0; //����ͨ��1 CCR1��ռ�ձ���ֵ��
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); //Ch1��ʼ��
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); //Ch4��ʼ��
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���,CCR�Զ�װ��Ĭ��Ҳ�Ǵ򿪵�
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ�� 
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1,ENABLE);

}


void Tim1_Init(void)
{
	_Tim1_GPIO_Init();
	_Tim1_Init();
}


void Tim1_SetPWM1(float percent)
{
	if(percent < PWM_MIN_PERCENT)
		percent = PWM_MIN_PERCENT;
	if(percent > PWM_MAX_PERCENT)
		percent = PWM_MAX_PERCENT;
	u32 pwm = (u16)(PWM_COUNTER * percent);
	TIM1->CCR1 = pwm;
}

void Tim1_SetPWM4(float percent)
{
	if(percent < PWM_MIN_PERCENT)
		percent = PWM_MIN_PERCENT;
	if(percent > PWM_MAX_PERCENT)
		percent = PWM_MAX_PERCENT;
	u32 pwm = (u32)(PWM_COUNTER * percent);
	TIM1->CCR4 = pwm;
}

void Tim1_SetPWM_ADC_PULSE(float percent)
{
	if(percent < PWM_MIN_PERCENT)
		percent = PWM_MIN_PERCENT;
	if(percent > PWM_MAX_PERCENT)
		percent = PWM_MAX_PERCENT;
	u32 pwm = (u32)(PWM_COUNTER * percent);
	
	TIM1->CCR4 = pwm;
	
	if(ADC_SAMPLE_POS > 0)
		pwm =  (u32)(pwm * ADC_SAMPLE_POS);
	else if(ADC_SAMPLE_POS < 0)
	{
		pwm += ADC_SAMPLE_POS;
		if(pwm <= 0)
			pwm -= ADC_SAMPLE_POS;
	}
	
	TIM1->CCR1 = pwm;

}
































