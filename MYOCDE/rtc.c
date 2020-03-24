#include "rtc.h"
#include "delay.h"

void Rtc_Init(void)
{
	RTC_InitTypeDef	 	RTC_InitStruct;
	RTC_TimeTypeDef 	RTC_TimeStruc;
	RTC_DateTypeDef 	RTC_DateStruct;
	//1、使能PWR时钟：
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	//2、使能后备寄存器访问:   
	PWR_BackupAccessCmd(ENABLE);
	
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x1916)
	{
	
		//3、配置RTC时钟源，使能RTC时钟：
		 RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		 RCC_RTCCLKCmd(ENABLE);
		
		//如果使用LSE，要打开LSE：
		RCC_LSEConfig(RCC_LSE_ON);
		delay_ms(50);		//延时等待时钟稳定
		
		RTC_InitStruct.RTC_HourFormat	= RTC_HourFormat_24;	//24小时制
		RTC_InitStruct.RTC_AsynchPrediv	= 0x7F;					//异步分频器
		RTC_InitStruct.RTC_SynchPrediv	= 0xFF;					//同步分频器
		//4、 初始化RTC(同步/异步分频系数和时钟格式)：
		RTC_Init(&RTC_InitStruct);
		
		RTC_TimeStruc.RTC_H12		= RTC_H12_PM;    //24小时制不需要写
		RTC_TimeStruc.RTC_Hours		= 16;
		RTC_TimeStruc.RTC_Minutes	= 10;
		RTC_TimeStruc.RTC_Seconds	= 15;
		//5、 设置时间：	
		RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruc);
		
		
		RTC_DateStruct.RTC_Year		= 19;
		RTC_DateStruct.RTC_Month	= 9;
		RTC_DateStruct.RTC_Date		= 19;
		RTC_DateStruct.RTC_WeekDay	= RTC_Weekday_Thursday;
		//6、设置日期：
		RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct);
		
		//往后备寄存写值
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x1916);
	}
}