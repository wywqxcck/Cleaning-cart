#include "stm32f10x.h"                  
#include "car.h" 
#include "Delay.h" 
#include "PWM.h" 
#include "run.h"
#include "SG90.h"
#include "OLED.h"

unsigned long S1=0;			//���ǰ������ֵ
unsigned long S2=0;			//����ұ߾���
unsigned long S3=0;			//�����߾���
uint16_t time;		//��ʱ��׼����
 
/* private define -------------------------------------------------*/
// ECHO
#define ECHO_GPIO_PORT    				GPIOB			              /* GPIO�˿� */
#define ECHO_GPIO_CLK 	    			RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define ECHO_GPIO_PIN					GPIO_Pin_10			        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define ECHO_GPIO_TOGGLE		 		ECHO_GPIO_PORT->BSRR = ((ECHO_GPIO_PORT->ODR & ECHO_GPIO_PIN) << 16) | (~ECHO_GPIO_PORT->ODR & ECHO_GPIO_PIN)
#define ECHO_GPIO_SET			   		ECHO_GPIO_PORT->BSRR = (uint32_t)ECHO_GPIO_PIN // ��1
#define ECHO_GPIO_RESET		   			ECHO_GPIO_PORT->BSRR = (uint32_t)ECHO_GPIO_PIN<<16 // ��0
#define ECHO_GPIO_READ					((ECHO_GPIO_PORT->IDR & ECHO_GPIO_PIN) != 0)
// TRIG
#define TRIG_GPIO_PORT    				GPIOB			              /* GPIO�˿� */
#define TRIG_GPIO_CLK 	    			RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define TRIG_GPIO_PIN					GPIO_Pin_11	 		        /* ���ӵ�SCLʱ���ߵ�GPIO */
#define TRIG_GPIO_TOGGLE		 		TRIG_GPIO_PORT->BSRR = ((TRIG_GPIO_PORT->ODR & TRIG_GPIO_PIN) << 16) | (~TRIG_GPIO_PORT->ODR & TRIG_GPIO_PIN)
#define TRIG_GPIO_SET			   		TRIG_GPIO_PORT->BSRR = (uint32_t)TRIG_GPIO_PIN // ��1
#define TRIG_GPIO_RESET		   			TRIG_GPIO_PORT->BSRR = (uint32_t)TRIG_GPIO_PIN<<16 // ��0
#define TRIG_GPIO_READ					((TRIG_GPIO_PORT->IDR & TRIG_GPIO_PIN) != 0)

/* private variables ----------------------------------------------*/

/* private function prototypes ------------------------------------*/
 void bizhang_Init(void);
static uint16_t Read(void);

/**
  * @brief  SR04 Init
  * @param  None
  * @retval None
  * @note   None
*/
 void bizhang_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructer;

	RCC_APB2PeriphClockCmd(TRIG_GPIO_CLK, ENABLE);
	/*TRIG�����ź�*/
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructer.GPIO_Pin=TRIG_GPIO_PIN;
	GPIO_Init(TRIG_GPIO_PORT, &GPIO_InitStructer);
	GPIO_ResetBits(TRIG_GPIO_PORT,TRIG_GPIO_PIN);

	RCC_APB2PeriphClockCmd(ECHO_GPIO_CLK, ENABLE);
	/*ECOH�����ź�*/
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructer.GPIO_Pin=ECHO_GPIO_PIN;
	GPIO_Init(ECHO_GPIO_PORT, &GPIO_InitStructer);

}

/**
  * @brief  Read IO�ڿ���
  * @param  None
  * @retval cm
  * @note   None
*/
static uint16_t Read(void)
{
	uint32_t tim_time = 0;
	uint16_t length = 0;

    TRIG_GPIO_SET;
Delay_us(20);
    TRIG_GPIO_RESET;
    //�ȴ������ź�
    while(ECHO_GPIO_READ == RESET)
    {
        tim_time++;
        Delay_us(1);
        if(tim_time >= 10000)
        {
            return 10000;
        }
    }
    tim_time = 0;
    //�ȴ������ź�
    while(ECHO_GPIO_READ)
    {
        tim_time++;
;       Delay_us(1);
        if(tim_time >= 10000)
        {
            return 10000;
        }
    }
    //�رն�ʱ��
    length = (tim_time * 0.12)/2;

    return length;
	
}



//���ת����ຯ��  С������
void COMM(void)
{
    Car_Up();
    Servo_SetAngle(90);      //�������
    Delay_ms(100);
    S1=Read(); 	     //�洢��ǰ������
    OLED_ShowString(3,7," C:");
    OLED_ShowNum(4,7,S1,3);
    if(S1<25)
    {
        Car_Stop();
        //�����������������ľ���  ��->��->��
        Servo_SetAngle(0);	     //�������ת
        Delay_ms(1000);
        //�������������������
        S2=Read();         //�洢�ұ߾���
        OLED_ShowString(3,13," R:");
        OLED_ShowNum(4,13,S2,3);
        Servo_SetAngle(180);     //�������ת
        Delay_ms(1000);
        S3=Read(); 	     //�洢��߾���
        OLED_ShowString(3,1," L:");
				OLED_ShowString(4,0,"       ");
        OLED_ShowNum(4,1,S3,3); 
        if((S2<25)||(S3<25))     //��߻����ұߵľ���С��15cm
        { 
            Car_Down();	     //С������
            Delay_ms(1000);   //����һ��ʱ��
        }                   
        if(S2>S3)		 	     //����ұ߾��������߾���
        {
            Car_TurnLeft();  	     //������߱ȳ����ұ߾���С����ת	
            Delay_ms(500);
        }				      
        else				     //����ұ߾���С����߾���
        {
            Car_TurnRight();		 //������߱ȳ����ұ߾������ת
            Delay_ms(500);
        }	
    }
}


