#include "stm32f10x.h"                  // Device header
#include "car.h" 
#include "Delay.h"  

void Car_Stop(void)//小车停止
{
	MotorR_SetSpeed(0);
	MotorL_SetSpeed(0);
}

void Car_Up(void)//小车前进
{
	MotorR_SetSpeed(60); //速度设置 过高可能会导致循迹反应不过来
	MotorL_SetSpeed(60);	
}

void Car_Down(void)//小车后退
{
	MotorR_SetSpeed(-60);
	MotorL_SetSpeed(-60);
}

void Car_TurnRight(void)//小车左边转
{
	MotorR_SetSpeed(-60);
	MotorL_SetSpeed(60);
}

void Car_TurnLeft(void)//小车右边转
{
	MotorR_SetSpeed(60);
	MotorL_SetSpeed(-60);
}

void Car_Spin(void)//小车旋转
{

       MotorR_SetSpeed(-60);MotorL_SetSpeed(100); 
 }
        

