#include "stm32f10x.h"                  // Device header
#include "run.h"
#include "OLED.h"

uint16_t Trace_1;
#define Tracking_1		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)//左上
#define Tracking_2		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)//左下
#define Tracking_3		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//右下
#define Tracking_4		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//右上
void TCRT5000_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 |GPIO_Pin_4 | GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
}

void xunji(void)
{
        //循迹  2左1中3右
        if((Tracking_2 == 0 && Tracking_1 == 0 && Tracking_3 == 0 && Tracking_4 == 0)
					||(Tracking_1 == 0 && Tracking_2 == 1 && Tracking_3 == 1 && Tracking_4==0))
        {
            Car_Up();  
						OLED_ShowString(3,1,"     ");
						OLED_ShowString(3,1,"go");
        }    
        else if((Tracking_1 == 1 && Tracking_2 == 0 && Tracking_3 == 1 && Tracking_4==1)

					||(Tracking_1 == 0 && Tracking_2 == 0 && Tracking_3 == 1 && Tracking_4==1)
					||(Tracking_1 == 0 && Tracking_2 == 0 && Tracking_3 == 0 && Tracking_4==1)
					) //前方遇到黑线优先左拐
        {	//先左转再直行//
            Car_TurnRight();
						OLED_ShowString(3,1,"     ");
            OLED_ShowString(3,1,"Left");
            
        }
        else if((Tracking_1 == 1 && Tracking_2 == 1 && Tracking_3 == 0 && Tracking_4==1)	
					||(Tracking_1 == 1 && Tracking_2 == 0 && Tracking_3 == 0 && Tracking_4==1)
					||(Tracking_1 == 1 && Tracking_2 == 1 && Tracking_3 == 0 && Tracking_4==0)
					||(Tracking_1 == 1 && Tracking_2 == 0 && Tracking_3 == 0 && Tracking_4==0))

        {	//先右转再直行//
            Car_TurnLeft();
						OLED_ShowString(3,1,"     ");
            OLED_ShowString(3,1,"Right");
            
          }      
      else if(Tracking_1 == 1 && Tracking_2 == 1 && Tracking_3 == 1 && Tracking_4==1) //停止
      {  //前方黑线 停止
            Car_Stop();
            OLED_ShowString(3,1,"Stop");
            OLED_ShowString(3,1,"     ");          
      }       
}
