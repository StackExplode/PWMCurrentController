#include "adc.h"
#include "config.h"
#include "delay.h"

static void _ADC_Port_Init(void)	//PC1
{
	GPIO_InitTypeDef  GPIO_InitStructure;  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  //ʹ��GPIOCʱ��  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������  
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
}

static void _ADC1_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	// ��ʱ��������ʹ������
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Falling;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;    //ADC_DataAlign_Right  ADC_DataAlign_Left�������������Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_3Cycles);
}

static void _ADC1_Comon_Config(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;        //ADC_DMAAccessMode_Disabled    ADC_DMAAccessMode_1
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);    //��ͨ��ʹ��
	//ADC_Cmd(ADC1, ENABLE);
	//ADC_DMACmd(ADC1, ENABLE);
}

//ADC1-DMA is channel0~stream0
static void _ADC1_DMA2_Init(const void* buff_ptr,u32 buff_len)
{
	DMA_Cmd(DMA2_Stream0, DISABLE); //ʹ��DMAͨ��0
	DMA_InitTypeDef            DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//����DMA2ʱ��
	
	DMA_DeInit(DMA2_Stream0);
    DMA_StructInit( &DMA_InitStructure);        //DMA�ṹ���ʼ��
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//�ӼĴ���������
    DMA_InitStructure.DMA_BufferSize = buff_len;//�Ĵ�����С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���Ϊ����
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���Ϊ����
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//���ȼ��ǳ���
    //DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//�ر��ڴ浽�ڴ�ģʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//ѭ������ģʽ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//�����ַΪDACͨ��1�ĵ�ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buff_ptr;//�������ݱ��ڴ��ַ
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// ѡ�� DMA ͨ����ͨ������������   
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);//��ʼ��
	//DMA_Cmd(DMA2_Stream0, ENABLE); //ʹ��DMAͨ��3 
}

static void _ADC1_DMA2_Interrupt_Init(void)
{
	                                    
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;       
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TC); //����жϱ�־   
    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE); //��������ж�   
}


u16 ADC_BUFF[ADC_DMA_LEN];	

__forceinline void ADC_My_StartDMA(void)
{
	DMA_Cmd(DMA2_Stream0, ENABLE); //ʹ��DMAͨ��3 
	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	//TIM_Cmd(TIM1,ENABLE);
}

__forceinline void ADC_My_StopDMA(void)
{
	DMA_Cmd(DMA2_Stream0, DISABLE); //ʧ��DMAͨ��3 
	ADC_Cmd(ADC1, DISABLE);
	ADC_DMACmd(ADC1, DISABLE);
	//TIM_Cmd(TIM1,DISABLE);
}

void ADC_My_Init(void)
{
	_ADC_Port_Init();
	_ADC1_Init();
	_ADC1_Comon_Config();
	ADC_My_StopDMA();
	_ADC1_DMA2_Init(ADC_BUFF,ADC_DMA_LEN);	
	_ADC1_DMA2_Interrupt_Init();
}

u16 ADC_My_ReadVal(void)
{
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_15Cycles);
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	u16 rt = ADC_GetConversionValue(ADC1); //�������һ��ADC1�������ת�����

	
	return rt;
}

u16 ADC_Zero_Cal(void)
{
	u32 sum = 0;
	static const u16 CALC_TIME = 30;
	for(u8 i=0;i<CALC_TIME;i++)
	{
		sum += ADC_My_ReadVal();
		delay_ms(3);
	}
	return sum/CALC_TIME;
}





































