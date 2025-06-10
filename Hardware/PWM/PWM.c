#include "stm32f10x.h"//ͷ�ļ�

//��ʼ����ʱ��TIM3 ������PWM��ʼ������
void PWM12_Init(void)
{	//����TIM3��GPIOA,GPIOBʱ��

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   
	
    //��ʼ��GPIO�� PB6 PB7 ���ڲ���PWM�ź�
    GPIO_InitTypeDef GPIO_InitStructure;
    //�����������ģʽ��Ϊ������TIM4����
    //�綯��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO B6 B7 
    
    //���      
   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //���PWM��� PA6
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


//	//ʹ���ڲ�ʱ��	
	TIM_InternalClockConfig(TIM4);
	TIM_InternalClockConfig(TIM3);

	//����ʱ����Ԫ����
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//��ʱ������Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;//ARR�Զ���װֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;//PSCԤ��Ƶֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//����ARR����һ���ж� ֹͣ����
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);//��ʼ����Ԫ
    
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//��ʱ������Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = 2000 - 1;//ARR�Զ���װֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;//PSCԤ��Ƶֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//����ARR����һ���ж� ֹͣ����
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);//��ʼ����Ԫ    
    
	//����ȽϽṹ������
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//��ȫ�ṹ����δ���ò���
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//ѡ��PWMģʽ1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//����Ƚϼ���ѡ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//���ʹ��

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);//��ʼ�� TIM4 OC1
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);//ʹ��CCR1�Զ���װ
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);//��ʼ�� TIM4 OC2	
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);//ʹ��CCR2�Զ���װ
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);//��ʼ�� TIM3 OC1	
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);//ʹ��CCR1�Զ���װ
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);//����Ԥװ��
	TIM_Cmd(TIM4, ENABLE);//������ʱ��3	
	TIM_ARRPreloadConfig(TIM3,ENABLE);//����Ԥװ��
	TIM_Cmd(TIM3, ENABLE);//������ʱ��3
    
	TIM4->CCR1 = 0;//��������Ƚ�ֵ
	TIM4->CCR2 = 0;
  TIM3->CCR1 = 0;
}

//����PWM1�Ƚ�ֵ ΪCompare ������Ƚ�ֵ
void PWM12_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM4, Compare);
}

//����PWM2�Ƚ�ֵ ΪCompare
void PWM12_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM4 , Compare);
}
//���
void PWM12_SetCompare4(uint16_t Compare)
{                                                                        
	TIM_SetCompare1(TIM3, Compare);
}

