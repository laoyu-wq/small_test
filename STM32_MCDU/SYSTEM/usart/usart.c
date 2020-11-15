#include "sys.h"
#include "usart.h"	 
#include "UART5.h"
#include "DGUS.h"
#include <stdio.h>
#include "string.h"
char UART1Buffer[BUFSIZE];
uint32_t UART1Count = 0;
uint8_t	Flag_start=1,Flag_start1=1; 
char	USART1_Cmd[]="CDF00";
char	*USART1_Cmd_R="P5F5 TQF5 MIF5 CDF5 P7F5 P5F5 P8F5 PEF5 WHF5 CBF5 ALF5"; 
void uart1_init(u32 bound)
	{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}



void USARTSend( uint32_t portNum, char*BufferPtr, uint32_t Length )
{
	if ( portNum == 1 )
	{
    	while ( Length != 0 )
    	{
      		while((USART1 -> SR & 0X40) == 0);		//�ȴ�������� 
      		//UART0 -> THR = *BufferPtr;
			USART1->DR = ((*BufferPtr) & (uint16_t)0x01FF);              
      		BufferPtr++;
      		Length--;
    	}
	}   
  	else if( portNum == 2 )
  	{
    	while ( Length != 0 )
    	{
      		while((USART2 -> SR & 0X40) == 0);
			USART2->DR = ((*BufferPtr) & (uint16_t)0x01FF);              
      		BufferPtr++;
      		Length--;
    	}
  	}
	else if( portNum == 3 )
  	{
    	while ( Length != 0 )
    	{
      		while((USART3 -> SR & 0X40) == 0);
			    USART3->DR = ((*BufferPtr) & (uint16_t)0x01FF);              
      		BufferPtr++;
      		Length--;
    	}
  	}
}

void USART1_Put_Char(unsigned char DataToSend)
{

	USART_SendData(USART1, (unsigned char) DataToSend);
  	while (!(USART1->SR & USART_FLAG_TXE));
}

void USART1_IRQHandler(void)      //����1�жϷ������   ��P3Dͨ��USB������Ϣ����
{
	u8 Res,Cmd;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Res =USART_ReceiveData(USART1);					   //��ȡ���յ�������
	
		if(Res==0x7E)  //��Ļ��������
		{
			Flag_start=9;
			UART1Count=0;
		}
		else
			if(Res==0x8E) 		//��ɫģʽ
			{
				Flag_start=5;
				/*******FAIL�ɿ�*******��673�ĳ�674*************/
				UART1Count=674;	
			}
			else
				if(Res==0x9E)	//��ǩ������
				{
					Flag_start1=3;
					/*******FAIL�ɿ�*******��625�ĳ�626*************/
				 UART1Count=626;					
				}
			else
			if(Res==0xFE)  //���ݽ�β
			{
				Flag_start=Flag_start&0XFE;   //Flag_start�ֱ��� 8 4 2
				UART1Count=0;
			}
			else
			{
				UART1Buffer[UART1Count]=Res;
				UART1Count++;
			}		  		 
    } 
	   //����ָ��
		 Cmd=strcmp(UART1Buffer,USART1_Cmd_R);
		 if(Cmd==0)
		 {
			 USARTSend(1,USART1_Cmd,5);
			 UART1Count=0;
     }
	}
 


