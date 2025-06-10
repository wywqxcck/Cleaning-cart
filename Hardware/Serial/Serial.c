 #include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxFlag;

char Serial_RxPacket[100];


void Serial_Init(void)
{
	//����GPIOA��USART1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//����PA9
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;//��RXD
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;//TXD
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��ʼ��USART1
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;//���ò����ʣ�һ��ѡ��9600/115200�ȳ���ֵ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ѡ���Ƿ�����Ӳ�������ƣ����˴�ѡ����ǲ�����Ӳ����
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//ѡ����ģʽ���˴�ѡ����Ƿ��ͺͽ���ģʽ��
	USART_InitStructure.USART_Parity = USART_Parity_No;//ѡ����żУ��ģʽ���˴�ѡ�������У�飩
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//ѡ��ֹͣλ�����˴�ѡ�����һλֹͣλ��
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//ѡ������λ�����˴�ѡ�����8λ����Ϊû��У��λ��������ֹͣλ����ʼλ����
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//ʹ���жϣ�ѡ����ǽ������ݼĴ�����Ϊ���жϣ�
	
	//�������ȼ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//��ʼ��NVIC
	NVIC_InitTypeDef NVIC_Initstructure;
	NVIC_Initstructure.NVIC_IRQChannel = USART1_IRQn;//ѡ��USART1ͨ��
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;//����ѡ���IRQnͨ��
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 1;//������ռ���ȼ�
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1;//��Ӧ���ȼ�
	NVIC_Init(&NVIC_Initstructure);
	//����USART
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	/*�������ݼĴ����ձ�־λ����ΪRESET�������ݼĴ����ǿգ�
	��ʱ���ܼ�����üĴ�������д���ݣ�����������ݼĴ����ռ���д���ݣ��ͻḲ��֮ǰ������
	���·��͵����ݶ�ʧһ���֣����磺Serial_SendArray�����Ĳ���������ɾȥ�жϱ�־λ���ͻ�
	����ÿ�ν��յ��������������һ�����ݣ���ǰ������������ȫ����ʧ
	*/
}
//����һ��uint8_t����
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Array[i]);
	}
}
//�����ַ���
void Serial_SendString(char *String)
{
	uint16_t i;
	for(i=0;String[i]!='\0';i++)//�ַ�����β��һ��������־λ0�����Ը��ݽ�����־λ���ж��ַ����Ƿ����
	{
		Serial_SendByte(String[i]);
	}
}
//����X^Y�η�
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
//���ַ���ʽ���ʮ��������
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte((Number/Serial_Pow(10,Length - i - 1))%10+'0');
		//�˴�+��0����Ϊ��������������ֺͶ�Ӧ���ַ���Ӧ�������Ӷ���֤��;.............����ȷ���ַ�
	}
}

//int fputc(int ch, FILE *f)
//{
//	Serial_SendByte(ch);
//	return ch;
//}
//�ض�����ת��������ʹ��sprintf����ʹ�÷�������printf
void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

//���ж�ʵ�ֽ���һ��HEX���ݰ�
void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;//������¼״̬����
	static uint8_t pRxPacket = 0;//������¼�����˼�������
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)//�ٴ�ȷ�ϱ�־λ�Ƿ���ȷ��λ
	{
		uint8_t RxData = USART_ReceiveData(USART1);

		//��ȡ�������ݼĴ����е����ݣ��洢��uint8_t RxData��
		if(RxState == 0)//״̬�㣨���հ�ͷ��
		{
			if(RxData == '@' && Serial_RxFlag==0)//�����ж���������ֹ��ǰһ�ν�������δ��ɾ��ٴν������ݵ�����
			{
				RxState = 1;
				pRxPacket=0;
			}
		}else if(RxState == 1)//״̬1���������ݣ�
		{	//�����յ������ݴ浽Serial_RxPacket�������Ӧλ��
			if(RxData != '\r')
			{
				Serial_RxPacket[pRxPacket]=RxData;
				pRxPacket++;
			}else
			{
				RxState = 2;
			}
			
		}else if(RxState == 2)//״̬2�����հ�β��
		{
			if(RxData == '\n')
			{
				RxState = 0;//��β������ɣ�������һ��״̬
				Serial_RxFlag = 1;//�������ź�λ��1���������ݽ�����ɣ����Զ�ȡ
				Serial_RxPacket[pRxPacket]='\0';//���ַ����������Ͻ�����־λ������������ʹ��ʱ�ж��ַ���ʲôʱ�����
			}
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//����������ݼĴ����ǿձ�־λ
	}
}



