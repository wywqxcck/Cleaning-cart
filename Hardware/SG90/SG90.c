
#include "stm32f10x.h"                  
#include "PWM.h"
//SG90舵机
void Servo_Init(void)
{
	PWM12_Init();
}
//舵机转动 直接输入角度
void Servo_SetAngle(float Angle)
{
	//0->500 180->2500  2.5ms
	PWM12_SetCompare4(Angle / 270 * 2000 + 500);
}
