/* UART5.c file
功能：
UART5通信 的API
------------------------------------
 */
#include "sys.h"
#include "UART5.h"
uint8_t UART5Buffer[24];
uint32_t UART5Count=0;
uint32_t Key_Code;

void U5NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
      /* Enable the UART5 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel =UART5_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void Initial_UART5(u32 baudrate)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 UART5 模块的时钟  使能 UART5对应的引脚端口PA的时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
  	/* 配置UART5 的发送引脚
	  配置PC12 为复用输出  刷新频率	50MHz  TX
	  */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);    
  	/* 
	  配置UART5 的接收引脚
	
	  配置PD2为浮地输入   RX
	 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	  
	/* 
	  UART5的配置:
	  1.波特率为调用程序指定的输入 baudrate;
	  2. 8位数据			  USART_WordLength_8b;
	  3.一个停止位			  USART_StopBits_1;
	  4. 无奇偶效验			  USART_Parity_No ;
	  5.不使用硬件流控制	  USART_HardwareFlowControl_None;
	  6.使能发送和接收功能	  USART_Mode_Rx | USART_Mode_Tx;
	 */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//应用配置到UART5
	USART_Init(UART5, &USART_InitStructure);
	 
	USART_ITConfig(UART5, USART_IT_TXE, DISABLE); //禁止发送中断        
  USART_ClearFlag(UART5,USART_FLAG_TC);
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);	//使能接收中断
	//启动UART5
  USART_Cmd(UART5, ENABLE);
  U5NVIC_Configuration();	 // UART5 中断配置
}

/**************************实现函数********************************************
*函数原型:		void UART5_Put_Char(unsigned char DataToSend)
*功　　能:		RS232发送一个字节
输入参数：
		unsigned char DataToSend   要发送的字节数据
输出参数：没有	
*******************************************************************************/
void UART5_Put_Char(unsigned char DataToSend)
{

	USART_SendData(UART5, (unsigned char) DataToSend);
  	while (!(UART5->SR & USART_FLAG_TXE));
}

/**************************实现函数********************************************
*函数原型:		u8 UART5_Get_Char(void)
*功　　能:		RS232接收一个字节  一直等待，直到UART2接收到一个字节的数据。
输入参数：		 没有
输出参数：       UART2接收到的数据	
*******************************************************************************/
u8 UART5_Get_Char(void)
{
	while (!(UART5->SR & USART_FLAG_RXNE));
	return(USART_ReceiveData(UART5));
}

/**************************实现函数********************************************
*函数原型:		void UART5_Put_String(unsigned char *Str)
*功　　能:		RS232发送字符串
输入参数：
		unsigned char *Str   要发送的字符串
输出参数：没有	
*******************************************************************************/
void UART5_Put_String(unsigned char *Str)
{
	while(*Str)
	{
		UART5_Put_Char(*Str);
		Str++;
	}
}

void UART5_Put_array(unsigned char *Str, uint8_t lens)
{
	while(lens--)
	{
		UART5_Put_Char(*Str);
		Str++;
	}
}



//------------------------------------------------------
void UART5_IRQHandler(void)  
{
//  unsigned char data;
 if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
                        
    USART_ClearITPendingBit(UART5, USART_IT_TXE); 
  
  }

  if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
  {
  	UART5Buffer[UART5Count++]=USART_ReceiveData(UART5);
	  UART5Count%=24;
		
  /* Clear the UART5 transmit interrupt */
    USART_ClearITPendingBit(UART5, USART_IT_RXNE);
  }
	
  if ( UART5Buffer[0] != 0xA5 )
  {
    UART5Count = 0;
  }
	else if ( UART5Count == 2 )
	{
		if ( UART5Buffer[1] != 0x5A )
		{
			UART5Count = 1;
			UART5Buffer[0] = UART5Buffer[1];
		}
	}
  else if( UART5Count > 8 )
  {
    //if( UART5Buffer[1] == 0x5A )
    {
      if(UART5Buffer[5]==0x40)   //+  - 
      {
        Key_Code = UART5Buffer[8]+7;
        UART5Count = 0;
        UART5Buffer[0] = 0;
      }
			else if(UART5Buffer[7] ==0xBB)   //普通按键
      {
        Key_Code = UART5Buffer[8];
        UART5Count = 0;
        UART5Buffer[0] = 0;
      }
    }
//
  }
}


//------------------End of File----------------------------
