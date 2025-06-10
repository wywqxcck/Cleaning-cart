#include "stm32f10x.h"                  // Device header
#include "car.h" 
#include "Delay.h"  

void Car_Stop(void)//С��ֹͣ
{
	MotorR_SetSpeed(0);
	MotorL_SetSpeed(0);
}

void Car_Up(void)//С��ǰ��
{
	MotorR_SetSpeed(60); //�ٶ����� ���߿��ܻᵼ��ѭ����Ӧ������
	MotorL_SetSpeed(60);	
}

void Car_Down(void)//С������
{
	MotorR_SetSpeed(-60);
	MotorL_SetSpeed(-60);
}

void Car_TurnRight(void)//С�����ת
{
	MotorR_SetSpeed(-60);
	MotorL_SetSpeed(60);
}

void Car_TurnLeft(void)//С���ұ�ת
{
	MotorR_SetSpeed(60);
	MotorL_SetSpeed(-60);
}

void Car_Spin(void)//С����ת
{

       MotorR_SetSpeed(-60);MotorL_SetSpeed(100); 
 }
        

