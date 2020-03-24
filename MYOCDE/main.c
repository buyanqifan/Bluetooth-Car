#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "delay.h"
#include "time.h"
#include "pwm.h"
#include "usart.h"
#include "string.h"
#include "dht11.h"
#include <stdio.h>
#include "sr04.h"
#include "infrared.h"
#include "iwdg.h"
#include "rtc.h"
#include "adc.h"
#include "light_sensor.h"
#include "iic.h"
#include "OLED_I2C.h"
#include "control.h"

u8 rx_i, rx_count = 0, rx_flag = 0;
u8 buffer[64] = {0};
u8 rx_buffer[64] = {0};


void USART1_IRQHandler(void)
{

	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)   == SET)
	{
		//从串口1接收数据
		buffer[rx_count++] = USART_ReceiveData(USART1);
		
		if(buffer[rx_count - 1] == ':') //接收字符结束
		{
			for(rx_i=0; rx_i<rx_count-1; rx_i++)
			{
				rx_buffer[rx_i] = buffer[rx_i];  //将有效数据存储到rx_buffer
			}
		
			rx_count = 0 ; //用于下次接受字符串时buffer从buffer[0]开始接受数据
			memset(buffer, 0, sizeof(buffer));
			rx_flag = 1;  //表示收到数据后标志位为1
		}
		

	}
	//清除中断标志位
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);

}



int main(void)
{
	unsigned char i;
	extern const unsigned char BMP1[];
	u16 speed = 250;
	u16 car_distance = 30;
	u8 carflag = 1;
	
	//中断优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Led_Init();
	Delay_Init();
	I2C_Configuration();
	OLED_Init();
	Pwm_Init();		//电机初始化
	Sr04_Init();	//超声波测距初始化
	Usart1_Init();
	
	OLED_Fill(0x00);
	for(i=0;i<3;i++)
	{
		OLED_ShowCN(38+i*16,0,i);//测试显示中文
	}

	
	while(1)
	{
		if(rx_flag == 1)   //表示接受到数据
		{
			
			printf("rx_buffer = %s\n",rx_buffer);
			
			if(strcmp(rx_buffer,"GO") == 0)	
			{
				Car_forward(&speed, &carflag);
			
			}
			if(strcmp(rx_buffer,"BACK") == 0)
			{
				Car_backward(&speed, &carflag);
				
			}
			if(strcmp(rx_buffer,"ADD") == 0)
			{
				Car_speedup(&speed, &carflag);
			
			}
			if(strcmp(rx_buffer,"SUB") == 0)
			{
				Car_speeddown(&speed, &carflag);

			}
			if(strcmp(rx_buffer,"STOP") == 0)
			{
				Car_stop();
			
			}
			if(strcmp(rx_buffer,"TR") == 0)
			{
				Car_turnright(&speed, &carflag);
				
			}
			if(strcmp(rx_buffer,"TL") == 0)
			{
				Car_turnleft(&speed, &carflag);
			}
			if(strcmp(rx_buffer,"ADDSR") == 0)
			{
				car_distance += 5;
			}
			if(strcmp(rx_buffer,"SUBSR") == 0)
			{
				car_distance -= 5;
			}

			
			//清空数组
			memset(rx_buffer, 0, sizeof(rx_buffer));
			
			rx_flag = 0;	
		}
		Car_obstacle(&speed, &carflag, car_distance);


	}

	return 0;
}
