#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void MotorAll_Init(void)
{
	//开启电机驱动口的四个GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	PWM12_Init();//开启定时器
}

//设置右路电机速度 PWM
void MotorR_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)//Speed值为正
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);//电机正转
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		PWM12_SetCompare1(Speed);//设置Speed转速
	}
	else//Speed值为负
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);//电机反转
		GPIO_SetBits(GPIOB, GPIO_Pin_13);
		PWM12_SetCompare1(-Speed);//设为-Speed转速
	}
}

//设置左路电机PWM 速度
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


