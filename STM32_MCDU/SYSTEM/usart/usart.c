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
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口 

}



void USARTSend( uint32_t portNum, char*BufferPtr, uint32_t Length )
{
	if ( portNum == 1 )
	{
    	while ( Length != 0 )
    	{
      		while((USART1 -> SR & 0X40) == 0);		//等待发送完毕 
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

void USART1_IRQHandler(void)      //串口1中断服务程序   与P3D通过USB进行信息交互
{
	u8 Res,Cmd;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res =USART_ReceiveData(USART1);					   //读取接收到的数据
	
		if(Res==0x7E)  //屏幕驱动数据
		{
			Flag_start=9;
			UART1Count=0;
		}
		else
			if(Res==0x8E) 		//纯色模式
			{
				Flag_start=5;
				/*******FAIL可控*******由673改成674*************/
				UART1Count=674;	
			}
			else
				if(Res==0x9E)	//便签输入行
				{
					Flag_start1=3;
					/*******FAIL可控*******由625改成626*************/
				 UART1Count=626;					
				}
			else
			if(Res==0xFE)  //数据结尾
			{
				Flag_start=Flag_start&0XFE;   //Flag_start分别变成 8 4 2
				UART1Count=0;
			}
			else
			{
				UART1Buffer[UART1Count]=Res;
				UART1Count++;
			}		  		 
    } 
	   //握手指令
		 Cmd=strcmp(UART1Buffer,USART1_Cmd_R);
		 if(Cmd==0)
		 {
			 USARTSend(1,USART1_Cmd,5);
			 UART1Count=0;
     }
	}
 


