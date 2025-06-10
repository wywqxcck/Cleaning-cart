
#include "stm32f10x.h"                  
#include "PWM.h"
//SG90���
void Servo_Init(void)
{
	PWM12_Init();
}
//���ת�� ֱ������Ƕ�
void Servo_SetAngle(float Angle)
{
	//0->500 180->2500  2.5ms
	PWM12_SetCompare4(Angle / 270 * 2000 + 500);
}
