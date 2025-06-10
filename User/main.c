#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "TCRT5000.h"
#include "Serial.h"
#include <string.h>
#include "run.h" 
#include "PWM.h"
#include "car.h"
#include "SG90.h"
#include "SR04.h"
#include "DHT11.h"
#include "LED.h"


DHT11_Data_TypeDef DHT11_Data;
int8_t  x=0;			//存放前方距离值
int main()
{  
    PWM12_Init();
    MotorAll_Init();
    OLED_Init();
    Serial_Init(); 
		Servo_SetAngle(0);
    Servo_Init(); 
		TCRT5000_Init();
    bizhang_Init();
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE );	
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		
		OLED_ShowString(1, 1, "temp:");//温度
		OLED_ShowString(2, 1, "humidity:");//湿度
		LED_Init();

	
while(1)
{	 	
	COMM();
////温湿度板块
//			if(Read_DHT11(&DHT11_Data) == SUCCESS)
//			{
//				OLED_ShowNum(2, 10, DHT11_Data.humi_int, 2); //湿度
//				OLED_ShowString(2, 12, ".");
//				OLED_ShowNum(2, 13, DHT11_Data.humi_deci, 2);
//				OLED_ShowNum(1, 6, DHT11_Data.temp_int, 2);  //温度
//				OLED_ShowString(1, 8, ".");
//				OLED_ShowNum(1, 9, DHT11_Data.temp_deci, 2);
//				
//			}
////蓝牙遥控循迹小车
//        if(Serial_RxFlag == 1)
//        { 

//            OLED_ShowString(4,1,"                ");//清除显示，防止后来数据与先前数据同时显示
//            OLED_ShowString(4,1,Serial_RxPacket);
//            if(strcmp(Serial_RxPacket, "Up") == 0) //小车前进
//            {
//                Serial_SendString("car is up");
//                OLED_ShowString(3,1,"                ");
//                OLED_ShowString(3,1," v: 80 ");
//                Car_Up();
//            }
//            else if(strcmp(Serial_RxPacket, "Down") == 0)//小车后退
//            {
//                Serial_SendString("car is down");
//                OLED_ShowString(3,1,"                ");
//                OLED_ShowString(3,1," v: 80");
//                Car_Down();
//            }
//            else if(strcmp(Serial_RxPacket, "LEFT") == 0) //左转
//            {
//                Serial_SendString("car is left");
//                OLED_ShowString(3,1,"                ");
//                OLED_ShowString(3,1," L:-100 R:100 ");
//                Car_TurnRight();
//            }
//            else if(strcmp(Serial_RxPacket, "RIGHT") == 0)//右转
//            {
//                Serial_SendString("car is right");
//                OLED_ShowString(3,1,"                ");
//                OLED_ShowString(3,1," R:-100 L:100 ");
//                Car_TurnLeft();
//            }
//            else if(strcmp(Serial_RxPacket, "STOP") == 0)//停下
//            {
//                Serial_SendString("car is stop");
//                OLED_ShowString(3,1,"                ");
//                OLED_ShowString(3,1," stop!!!! ");
//                Car_Stop();
//            }
//            else if(strcmp(Serial_RxPacket, "SHOW TIME!!!") == 0)//风扇
//            {
//                Serial_SendString("show tim");
//                OLED_ShowString(3,1,"                ");
//                OLED_ShowString(3,1,"show time!!!!! ");
//								if(x==0){LED1_ON();x=1;}					
//								else {LED1_OFF();x=0;}
//            }
//            else if(strcmp(Serial_RxPacket, "bizhang") == 0)//避障/
//            {
//                Serial_RxFlag =0;
//								OLED_ShowString(3,1,"                ");
//                while(Serial_RxFlag == 0)
//                {
//                    COMM(); 
//                }
//            }
//            else if(strcmp(Serial_RxPacket, "xunji") == 0)//循迹/
//            {
//                OLED_ShowString(3,1,"                ");
//                Serial_SendString("xunji");
//                Serial_RxFlag =0;
//                while(Serial_RxFlag == 0)
//                {
//                   xunji();
//                }
//            }
//            Serial_RxFlag =0;
//        }  
    }
}




