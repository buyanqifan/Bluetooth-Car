#include "control.h"
#include "sr04.h"
#include "OLED_I2C.h"

u16 car_ = 250, j = 250;
u8 flag = 1;
u8 turnflag = 1;
u16 i;

void Car_forward(u16 * speed, u8 * carflag)
{
	OLED_Fill(0x00);
	for(i=0;i<3;i++)
	{
		OLED_ShowCN(38+i*16,0,i);//法拉利
	}
	OLED_ShowCN(55,2,5);
	OLED_ShowStr(30,5,"forward",2);
	
	
	car_ = *speed;
	flag = *carflag;
	if(flag == 0)
	{
		j = car_;
		//如果是反向的，先减速到零，再加速到相反速度，防止卡壳
		for(;car_>50;car_-=2)
		{
			TIM_SetCompare1(TIM9, 0);
			TIM_SetCompare2(TIM9, car_);
			TIM_SetCompare3(TIM2, 0);
			TIM_SetCompare4(TIM2, car_);
		}
		for(;car_<j;car_+=2)
		{
			TIM_SetCompare1(TIM9, car_);
			TIM_SetCompare2(TIM9, 0);
			TIM_SetCompare3(TIM2, car_);
			TIM_SetCompare4(TIM2, 0);
		}	
		car_ = j;
	}

	TIM_SetCompare1(TIM9, car_);
	TIM_SetCompare2(TIM9, 0);
	TIM_SetCompare3(TIM2, car_);
	TIM_SetCompare4(TIM2, 0);
	*carflag = 1;
	
}

void Car_backward(u16 * speed, u8 * carflag)
{

	OLED_Fill(0x00);
	for(i=0;i<3;i++)
	{
		OLED_ShowCN(38+i*16,0,i);//法拉利
	}
	OLED_ShowCN(55,2,6);
	OLED_ShowStr(30,5,"backward",2);
	
	car_ = *speed;
	flag = *carflag;
	if(flag == 1)
	{
		j = car_;
		//如果是反向的，先减速到零，再加速到相反速度，防止卡壳
		for(;car_>50;car_-=2)
		{
			TIM_SetCompare1(TIM9, car_);
			TIM_SetCompare2(TIM9, 0);
			TIM_SetCompare3(TIM2, car_);
			TIM_SetCompare4(TIM2, 0);
		}
		for(;car_<j;car_+=2)
		{
			TIM_SetCompare1(TIM9, 0);
			TIM_SetCompare2(TIM9, car_);
			TIM_SetCompare3(TIM2, 0);
			TIM_SetCompare4(TIM2, car_);
		}	
		car_ = j;
	}

	TIM_SetCompare1(TIM9, 0);
	TIM_SetCompare2(TIM9, car_);
	TIM_SetCompare3(TIM2, 0);
	TIM_SetCompare4(TIM2, car_);
	*carflag = 0;

}


void Car_speedup(u16 * speed, u8 * carflag)
{
	
	OLED_Fill(0x00);
	for(i=0;i<3;i++)
	{
		OLED_ShowCN(38+i*16,0,i);//法拉利
	}
	OLED_ShowStr(30,5,"speedup",2);
	car_ = *speed;
	flag = *carflag;
	if(car_ < 470)
		car_ += 30;
	if(flag == 1)
	{
		TIM_SetCompare1(TIM9, car_);
		TIM_SetCompare2(TIM9, 0);
		TIM_SetCompare3(TIM2, car_);
		TIM_SetCompare4(TIM2, 0);
	}
	else if(flag == 0)
	{
		TIM_SetCompare1(TIM9, 0);
		TIM_SetCompare2(TIM9, car_);
		TIM_SetCompare3(TIM2, 0);
		TIM_SetCompare4(TIM2, car_);
	}
	
	*speed = car_;

}


void Car_speeddown(u16 * speed, u8 * carflag)
{
	OLED_Fill(0x00);
	for(i=0;i<3;i++)
	{
		OLED_ShowCN(38+i*16,0,i);//法拉利
	}
	OLED_ShowStr(30,5,"speeddown",2);
	car_ = *speed;
	flag = *carflag;
	if(car_ > 30)
		car_ -= 30;
	if(flag == 1)
	{
		TIM_SetCompare1(TIM9, car_);
		TIM_SetCompare2(TIM9, 0);
		TIM_SetCompare3(TIM2, car_);
		TIM_SetCompare4(TIM2, 0);
	}
	else if(flag == 0)
	{
		TIM_SetCompare1(TIM9, 0);
		TIM_SetCompare2(TIM9, car_);
		TIM_SetCompare3(TIM2, 0);
		TIM_SetCompare4(TIM2, car_);
	}
	
	*speed = car_;

}

void Car_stop(void)
{
	OLED_Fill(0x00);
	for(i=0;i<3;i++)
	{
		OLED_ShowCN(38+i*16,0,i);//法拉利
	}
	OLED_ShowStr(30,5,"stop",2);
	
	TIM_SetCompare1(TIM9, 0);
	TIM_SetCompare2(TIM9, 0);
	TIM_SetCompare3(TIM2, 0);
	TIM_SetCompare4(TIM2, 0);

}

//左转
void Car_turnleft(u16 * speed, u8 * carflag)  
{
	OLED_Fill(0x00);
	for(i=0;i<3;i++)
	{
		OLED_ShowCN(38+i*16,0,i);//法拉利
	}
	OLED_ShowCN(55,2,4);
	OLED_ShowStr(30,5,"turnleft",2);
	
	car_ = *speed;
	flag = *carflag;
	j = car_;
	for(;car_>50;car_-=2)
	{
		TIM_SetCompare1(TIM9, j);
		TIM_SetCompare2(TIM9, 0);	
		TIM_SetCompare3(TIM2, car_);
		TIM_SetCompare4(TIM2, 0);
	}
	for(;car_<j;car_+=2)
	{
		TIM_SetCompare1(TIM9, j);
		TIM_SetCompare2(TIM9, 0);
		TIM_SetCompare3(TIM2, 0);
		TIM_SetCompare4(TIM2, car_);
	}	
	delay_ms(700);
	for(;car_>50;car_-=2)
	{
		TIM_SetCompare1(TIM9, j);
		TIM_SetCompare2(TIM9, 0);	
		TIM_SetCompare3(TIM2, 0);
		TIM_SetCompare4(TIM2, car_);
	}
	for(;car_<j;car_+=2)
	{
		TIM_SetCompare1(TIM9, j);
		TIM_SetCompare2(TIM9, 0);
		TIM_SetCompare3(TIM2, car_);
		TIM_SetCompare4(TIM2, 0);
	}	
	car_ = j;
	Car_forward(speed, carflag);

}


//右转
void Car_turnright(u16 * speed, u8 * carflag)  
{
	OLED_Fill(0x00);
	for(i=0;i<3;i++)
	{
		OLED_ShowCN(38+i*16,0,i);//法拉利
	}
	OLED_ShowCN(55,2,3);
	OLED_ShowStr(0,5,"turnright",2);
	car_ = *speed;
	flag = *carflag;
	j = car_;
	for(;car_>50;car_-=2)
	{
		TIM_SetCompare1(TIM9, car_);
		TIM_SetCompare2(TIM9, 0);	
		TIM_SetCompare3(TIM2, j);
		TIM_SetCompare4(TIM2, 0);
	}
	for(;car_<j;car_+=2)
	{
		TIM_SetCompare1(TIM9, 0);
		TIM_SetCompare2(TIM9, car_);	
		TIM_SetCompare3(TIM2, j);
		TIM_SetCompare4(TIM2, 0);
	}
	
	delay_ms(700);
	for(;car_>50;car_-=2)
	{
		TIM_SetCompare1(TIM9, 0);
		TIM_SetCompare2(TIM9, car_);	
		TIM_SetCompare3(TIM2, j);
		TIM_SetCompare4(TIM2, 0);
	}
	for(;car_<j;car_+=2)
	{
		TIM_SetCompare1(TIM9, car_);
		TIM_SetCompare2(TIM9, 0);	
		TIM_SetCompare3(TIM2, j);
		TIM_SetCompare4(TIM2, 0);
	}	

	car_ = j;
	Car_forward(speed, carflag);
}

void Car_obstacle(u16 * speed, u8 * carflag, u16 car_distance)  
{
	
	u16 distance;
	distance = Get_Sr04_Distance();
	if(distance < car_distance)
	{
		if(distance < 10)
		{
			Car_backward(speed, carflag);
			delay_s(1);
			Car_forward(speed, carflag);
		}
		if(turnflag)
		{
			Car_turnleft(speed, carflag);
			turnflag = 0;
		}
		else
		{
			Car_turnright(speed, carflag);
			turnflag = 1;
		}
	}
}

