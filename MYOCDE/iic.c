#include "iic.h"


void Iic_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	//打开GPIO B组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_8|GPIO_Pin_9;  		//引脚9
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;	//输出
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//推挽
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;		//上拉
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz; //速度	 
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	

}

void Iic_Mode(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_9;		//引脚9
	GPIO_InitStruct.GPIO_Mode	= Mode;	//输出模式
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;	//输出推挽
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;	//50MHZ
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void Iic_Start(void)
{
	//模式输出
	Iic_Mode(GPIO_Mode_OUT);
	
	SCL		 = 1;
	SDA_OUT  = 1;
	delay_us(5);
	
	SDA_OUT = 0;
	delay_us(5);
	
	//SDA与SCL为低钳住总线
	SCL		= 0;

}

void Iic_Stop(void)
{
	//模式输出
	Iic_Mode(GPIO_Mode_OUT);	
	
	SCL		 = 0;
	SDA_OUT  = 0;
	delay_us(5);
	
	SCL		 = 1;
	delay_us(5);
	
	//SDA与SCL为高电平，总线空闲
	SDA_OUT  = 1;

}

//发送一位数据
void Iic_Send_Ack(u8 ack)
{
	//模式输出
	Iic_Mode(GPIO_Mode_OUT);
	
	SCL		 = 0;
	
	//数据准备
	if(ack == 1)
	{
		SDA_OUT = 1;	
	}
	else
	{
		SDA_OUT = 0;	
	}
	//时钟形成一个脉冲周期，发送一位数据
	delay_us(5);
	SCL = 1;
	delay_us(5);
	SCL = 0;
	

}
//发送一个字节数据 先传高位，再传低位
void Iic_Send_Byte(u8 data)
{
	u8 i;
	//模式输出
	Iic_Mode(GPIO_Mode_OUT);	
	
	
	SCL		 = 0;
	
	for(i=0; i<8; i++)
	{
		//数据准备
		if(	data   &  1<<(7-i) )
		{
			SDA_OUT = 1;	
		}
		else
		{
			SDA_OUT = 0;	
		}
		//时钟形成一个脉冲周期，发送一位数据
		delay_us(5);
		SCL = 1;
		delay_us(5);
		SCL = 0;	
	
	}
	
	
}

//接收一位数据
u8 Iic_Recv_Ack(void)
{
	u8 ack;
	
	//模式输出
	Iic_Mode(GPIO_Mode_IN);		
	
	SCL = 0;
	delay_us(5);
	SCL = 1;
	
	if(SDA_IN == 1)
	{
		ack = 1;
	}
	else
	{
		ack = 0;
	}
	delay_us(5);
	SCL = 0;
	
	return ack;
}

//接收一个字节数据 先接高位，再接低位
u8 Iic_Recv_Byte(void)
{
	u8 i, data = 0;  //0100 1010
	
	//模式输出
	Iic_Mode(GPIO_Mode_IN);		
	
	SCL = 0;
	
	for(i=0; i<8; i++)
	{
		delay_us(5);
		SCL = 1;
		
		if(SDA_IN == 1)
		{
			data |= 1<<(7-i);
		}

		delay_us(5);
		SCL = 0;
	}
	
	return data;



}

//addr:写数据的起始地址; len <= 8;
void At24c02_Write_Page(u8 addr, u8 *write_buff, u8 len)
{
	u8 ack;
	
	//开启信号
	Iic_Start();
	
	//发送设备地址，并执行写操作
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		return ;
	}
	
	Iic_Send_Byte(addr);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		return ;
	}	
	
	
	
	//发送数据
	while(len--)
	{
		Iic_Send_Byte(*write_buff);
		ack = Iic_Recv_Ack();
		if(ack == 1)
		{
			printf("ack failure\n");
			return ;
		}	
		write_buff++;
	
	}
	
	
	Iic_Stop();
	
	printf("write finish\n");
}


void At24c02_Read_Page(u8 addr, u8 *read_buff, u8 len)
{
	u8 ack;
	
	//开启信号
	Iic_Start();
	
	//发送设备地址，并执行写操作
	Iic_Send_Byte(0xA0);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		return ;
	}
	
	Iic_Send_Byte(addr);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		return ;
	}	
	
	//开启信号
	Iic_Start();	
	
	//发送设备地址，并执行读操作
	Iic_Send_Byte(0xA1);
	ack = Iic_Recv_Ack();
	if(ack == 1)
	{
		printf("ack failure\n");
		return ;
	}	
	
		
	//发送数据
	while(len--)
	{
		
		*read_buff = Iic_Recv_Byte();
		
		if(len > 0)
			Iic_Send_Ack(0); //发送有效应答
	
		read_buff++;
	
	}
	
	Iic_Send_Ack(1); //发送无效应答
	Iic_Stop();
	
	printf("read finish\n");
}




























