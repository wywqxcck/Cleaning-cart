#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void MotorAll_Init(void)
{
	//������������ڵ��ĸ�GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	PWM12_Init();//������ʱ��
}

//������·����ٶ� PWM
void MotorR_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)//SpeedֵΪ��
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);//�����ת
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		PWM12_SetCompare1(Speed);//����Speedת��
	}
	else//SpeedֵΪ��
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);//�����ת
		GPIO_SetBits(GPIOB, GPIO_Pin_13);
		PWM12_SetCompare1(-Speed);//��Ϊ-Speedת��
	}
}

//������·���PWM �ٶ�
void MotorL_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_14);
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);
		PWM12_SetCompare2(Speed);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);
		GPIO_SetBits(GPIOB, GPIO_Pin_15);
		PWM12_SetCompare2(-Speed);
	}
}


