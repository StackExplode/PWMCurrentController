#ifndef __CCONTORL_H__
#define __CCONTORL_H__

#include "sys.h"

extern u8 DMA_Flag,CC_Run,DMA_Run;
extern float Aim_Current;

void Current_Control(void);
void Controller_Init(void);
float Control_FeedForward(float current);
void Controller_SetPID(float p, float i, float d, u8 reset);
void Controller_ResetPWM(void);

#endif
