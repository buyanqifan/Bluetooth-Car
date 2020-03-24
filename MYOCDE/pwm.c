#include "pwm.h"

/*
PA2 PA3----左轮串口2 TIM9 OC1、2
PB11 PB10---右轮串口1 TIM2 OC3、4
*/
void Pwm_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//	1.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 

	//	2.
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; //复用
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; //复用
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//	3.
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM9);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_TIM2);
	
	//	4.
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_Prescaler	= 84-1;  		//84MHZ/84 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_Period		= 500-1;		//在时钟频率为1MHZ下，计数500个数，用时500us

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler	= 168-1;  		//84MHZ/84 = 1MHZ
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseInitStruct);


	//	5.
	TIM_OCInitStruct.TIM_OCMode 		= TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState 	= TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity 	= TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM9, &TIM_OCInitStruct); 
	TIM_OC2Init(TIM9, &TIM_OCInitStruct); 
	TIM_OC3Init(TIM2, &TIM_OCInitStruct); 
	TIM_OC4Init(TIM2, &TIM_OCInitStruct); 

	
	//	6.
	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	//	7.
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_ARRPreloadConfig(TIM9,ENABLE);
	//	8.
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM9, ENABLE);
}



