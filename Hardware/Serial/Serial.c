 #include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxFlag;

char Serial_RxPacket[100];


void Serial_Init(void)
{
	//开启GPIOA和USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//配置PA9
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;//连RXD
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;//TXD
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//初始化USART1
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;//配置波特率（一般选择9600/115200等常用值）
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//选择是否启用硬件流控制（）此处选择的是不开启硬件流
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//选择工作模式（此处选择的是发送和接收模式）
	USART_InitStructure.USART_Parity = USART_Parity_No;//选择奇偶校验模式（此处选择的是无校验）
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//选择停止位数（此处选择的是一位停止位）
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//选择数据位数（此处选择的是8位，因为没有校验位（不包含停止位和起始位））
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//使能中断（选择的是接收数据寄存器不为空中断）
	
	//配置优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//初始化NVIC
	NVIC_InitTypeDef NVIC_Initstructure;
	NVIC_Initstructure.NVIC_IRQChannel = USART1_IRQn;//选择USART1通道
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;//开启选择的IRQn通道
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 1;//设置抢占优先级
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1;//响应优先级
	NVIC_Init(&NVIC_Initstructure);
	//开启USART
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	/*发送数据寄存器空标志位；若为RESET则发送数据寄存器非空，
	此时不能继续向该寄存器继续写数据，如果发送数据寄存器空继续写数据，就会覆盖之前的数据
	导致发送的数据丢失一部分；例如：Serial_SendArray函数的测试数据若删去判断标志位语句就会
	导致每次接收到的数据总是最后一个数据，而前面三个数据则全部丢失
	*/
}
//发送一个uint8_t数组
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Array[i]);
	}
}
//发送字符串
void Serial_SendString(char *String)
{
	uint16_t i;
	for(i=0;String[i]!='\0';i++)//字符串结尾有一个结束标志位0，可以根据结束标志位来判断字符串是否结束
	{
		Serial_SendByte(String[i]);
	}
}
//返回X^Y次方
uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Temp;
	Temp=1;
	while(Y--)
	{
		Temp*=X;
	}
	return Temp;
}
//12345/1000
//以字符形式输出十进制数字
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte((Number/Serial_Pow(10,Length - i - 1))%10+'0');
		//此处+‘0’是为了让算出来的数字和对应的字符对应起来，从而保证输;.............出正确的字符
	}
}

//int fputc(int ch, FILE *f)
//{
//	Serial_SendByte(ch);
//	return ch;
//}
//重定向，让转函数可以使用sprintf函数使用方法类似printf
void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

//在中断实现接收一个HEX数据包
void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;//用来记录状态数据
	static uint8_t pRxPacket = 0;//用来记录接收了几个数据
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)//再次确认标志位是否正确置位
	{
		uint8_t RxData = USART_ReceiveData(USART1);

		//读取接收数据寄存器中的数据，存储到uint8_t RxData中
		if(RxState == 0)//状态零（接收包头）
		{
			if(RxData == '@' && Serial_RxFlag==0)//增加判断条件，防止对前一次接收数据未完成就再次接受数据的问题
			{
				RxState = 1;
				pRxPacket=0;
			}
		}else if(RxState == 1)//状态1（接收数据）
		{	//将接收到的数据存到Serial_RxPacket数组的相应位置
			if(RxData != '\r')
			{
				Serial_RxPacket[pRxPacket]=RxData;
				pRxPacket++;
			}else
			{
				RxState = 2;
			}
			
		}else if(RxState == 2)//状态2（接收包尾）
		{
			if(RxData == '\n')
			{
				RxState = 0;//包尾接收完成，进入下一个状态
				Serial_RxFlag = 1;//将接收信号位置1，代表数据接收完成，可以读取
				Serial_RxPacket[pRxPacket]='\0';//在字符串的最后加上结束标志位，方便主函数使用时判断字符串什么时候结束
			}
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除接收数据寄存器非空标志位
	}
}



