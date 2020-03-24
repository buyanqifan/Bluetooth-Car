#ifndef __CONTROL_H
#define __CONTROL_H

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"



void Car_forward(u16 * speed, u8 * carflag);
void Car_backward(u16 * speed, u8 * carflag);
void Car_speedup(u16 * speed, u8 * carflag);
void Car_speeddown(u16 * speed, u8 * carflag);
void Car_stop(void);
void Car_turnleft(u16 * speed, u8 * carflag);  
void Car_turnright(u16 * speed, u8 * carflag);  
void Car_obstacle(u16 * speed, u8 * carflag, u16 car_distance);  





#endif
