#include "stm32f10x.h"                  
#include "car.h" 
#include "Delay.h" 
#include "PWM.h" 
#include "run.h"
#include "SG90.h"
#include "OLED.h"

unsigned long S1=0;			//存放前方距离值
unsigned long S2=0;			//存放右边距离
unsigned long S3=0;			//存放左边距离
uint16_t time;		//延时基准变量
 
/* private define -------------------------------------------------*/
// ECHO
#define ECHO_GPIO_PORT    				GPIOB			              /* GPIO端口 */
#define ECHO_GPIO_CLK 	    			RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define ECHO_GPIO_PIN					GPIO_Pin_10			        /* 连接到SCL时钟线的GPIO */
#define ECHO_GPIO_TOGGLE		 		ECHO_GPIO_PORT->BSRR = ((ECHO_GPIO_PORT->ODR & ECHO_GPIO_PIN) << 16) | (~ECHO_GPIO_PORT->ODR & ECHO_GPIO_PIN)
#define ECHO_GPIO_SET			   		ECHO_GPIO_PORT->BSRR = (uint32_t)ECHO_GPIO_PIN // 置1
#define ECHO_GPIO_RESET		   			ECHO_GPIO_PORT->BSRR = (uint32_t)ECHO_GPIO_PIN<<16 // 置0
#define ECHO_GPIO_READ					((ECHO_GPIO_PORT->IDR & ECHO_GPIO_PIN) != 0)
// TRIG
#define TRIG_GPIO_PORT    				GPIOB			              /* GPIO端口 */
#define TRIG_GPIO_CLK 	    			RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define TRIG_GPIO_PIN					GPIO_Pin_11	 		        /* 连接到SCL时钟线的GPIO */
#define TRIG_GPIO_TOGGLE		 		TRIG_GPIO_PORT->BSRR = ((TRIG_GPIO_PORT->ODR & TRIG_GPIO_PIN) << 16) | (~TRIG_GPIO_PORT->ODR & TRIG_GPIO_PIN)
#define TRIG_GPIO_SET			   		TRIG_GPIO_PORT->BSRR = (uint32_t)TRIG_GPIO_PIN // 置1
#define TRIG_GPIO_RESET		   			TRIG_GPIO_PORT->BSRR = (uint32_t)TRIG_GPIO_PIN<<16 // 置0
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
	/*TRIG触发信号*/
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructer.GPIO_Pin=TRIG_GPIO_PIN;
	GPIO_Init(TRIG_GPIO_PORT, &GPIO_InitStructer);
	GPIO_ResetBits(TRIG_GPIO_PORT,TRIG_GPIO_PIN);

	RCC_APB2PeriphClockCmd(ECHO_GPIO_CLK, ENABLE);
	/*ECOH回响信号*/
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructer.GPIO_Pin=ECHO_GPIO_PIN;
	GPIO_Init(ECHO_GPIO_PORT, &GPIO_InitStructer);

}

/**
  * @brief  Read IO口控制
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
    //等待返回信号
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
    //等待返回信号
    while(ECHO_GPIO_READ)
    {
        tim_time++;
;       Delay_us(1);
        if(tim_time >= 10000)
        {
            return 10000;
        }
    }
    //关闭定时器
    length = (tim_time * 0.12)/2;

    return length;
	
}



//舵机转动测距函数  小车避障
void COMM(void)
{
    Car_Up();
    Servo_SetAngle(90);      //舵机归中
    Delay_ms(100);
    S1=Read(); 	     //存储正前方距离
    OLED_ShowString(3,7," C:");
    OLED_ShowNum(4,7,S1,3);
    if(S1<25)
    {
        Car_Stop();
        //测出左中右三个方向的距离  右->左->中
        Servo_SetAngle(0);	     //舵机向右转
        Delay_ms(1000);
        //启动超声波测距计算距离
        S2=Read();         //存储右边距离
        OLED_ShowString(3,13," R:");
        OLED_ShowNum(4,13,S2,3);
        Servo_SetAngle(180);     //舵机向左转
        Delay_ms(1000);
        S3=Read(); 	     //存储左边距离
        OLED_ShowString(3,1," L:");
				OLED_ShowString(4,0,"       ");
        OLED_ShowNum(4,1,S3,3); 
        if((S2<25)||(S3<25))     //左边或者右边的距离小于15cm
        { 
            Car_Down();	     //小车后退
            Delay_ms(1000);   //后退一段时间
        }                   
        if(S2>S3)		 	     //如果右边距离大于左边距离
        {
            Car_TurnLeft();  	     //车的左边比车的右边距离小，右转	
            Delay_ms(500);
        }				      
        else				     //如果右边距离小于左边距离
        {
            Car_TurnRight();		 //车的左边比车的右边距离大，左转
            Delay_ms(500);
        }	
    }
}


