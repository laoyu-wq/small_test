/* UART5.c file
���ܣ�
UART5ͨ�� ��API
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

	/* ʹ�� UART5 ģ���ʱ��  ʹ�� UART5��Ӧ�����Ŷ˿�PA��ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
  	/* ����UART5 �ķ�������
	  ����PC12 Ϊ�������  ˢ��Ƶ��	50MHz  TX
	  */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);    
  	/* 
	  ����UART5 �Ľ�������
	
	  ����PD2Ϊ��������   RX
	 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	  
	/* 
	  UART5������:
	  1.������Ϊ���ó���ָ�������� baudrate;
	  2. 8λ����			  USART_WordLength_8b;
	  3.һ��ֹͣλ			  USART_StopBits_1;
	  4. ����żЧ��			  USART_Parity_No ;
	  5.��ʹ��Ӳ��������	  USART_HardwareFlowControl_None;
	  6.ʹ�ܷ��ͺͽ��չ���	  USART_Mode_Rx | USART_Mode_Tx;
	 */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//Ӧ�����õ�UART5
	USART_Init(UART5, &USART_InitStructure);
	 
	USART_ITConfig(UART5, USART_IT_TXE, DISABLE); //��ֹ�����ж�        
  USART_ClearFlag(UART5,USART_FLAG_TC);
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);	//ʹ�ܽ����ж�
	//����UART5
  USART_Cmd(UART5, ENABLE);
  U5NVIC_Configuration();	 // UART5 �ж�����
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART5_Put_Char(unsigned char DataToSend)
*��������:		RS232����һ���ֽ�
���������
		unsigned char DataToSend   Ҫ���͵��ֽ�����
���������û��	
*******************************************************************************/
void UART5_Put_Char(unsigned char DataToSend)
{

	USART_SendData(UART5, (unsigned char) DataToSend);
  	while (!(UART5->SR & USART_FLAG_TXE));
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 UART5_Get_Char(void)
*��������:		RS232����һ���ֽ�  һֱ�ȴ���ֱ��UART2���յ�һ���ֽڵ����ݡ�
���������		 û��
���������       UART2���յ�������	
*******************************************************************************/
u8 UART5_Get_Char(void)
{
	while (!(UART5->SR & USART_FLAG_RXNE));
	return(USART_ReceiveData(UART5));
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART5_Put_String(unsigned char *Str)
*��������:		RS232�����ַ���
���������
		unsigned char *Str   Ҫ���͵��ַ���
���������û��	
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
			else if(UART5Buffer[7] ==0xBB)   //��ͨ����
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
