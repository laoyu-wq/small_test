#include "stm32f10x.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include "uart5.h"
#include "adc.h"
#include "DGUS.h"

extern uint8_t Flag_start,Flag_start1;
extern uint8_t UART1Buffer[680];
extern uint8_t Current_Page;
extern uint8_t Baidiheizi;
uint8_t status_screen=0,status_link=1;

int main(void)
{	
	u8 Key=0;
	char data[]={0X7E,0X62,0XFE};
	delay_init();					 
	uart1_init(115200);
	Initial_UART5(115200);//��Ļ
	KEY_Init();
	Adc_Init();
	while(1)
  {
		
    /*****************����������***************************/
		Key=KEY_Scan();
		if(Key!=KEY_NULL_VALUE)//�������
		{
			data[1]=Key;
			USARTSend(1,data,3);  //��P3D���Ͱ�����ֵ
	   // GPIO_ResetBits(GPIOC,GPIO_Pin_10);			
		}
		
		Brightn_Change(1);
	
	 /*******************��Ļˢ�²���***************************/
			if((Flag_start%2)==0)
		{
			Flag_start=Flag_start|1;	//��λ��־λ
			//status_screen=0;
			//status_link=1;
			if((Flag_start>>3)==1)//�����Ļ   8
			{
				
				fill_screen(UART1Buffer); //���Ӧ�ؼ���д����
				
				if(Baidiheizi==0)
				{
					if(Current_Page==0)
					{
						change_screen(12);
						Current_Page=1;
					}
					else
					{
						change_screen(13);
						Current_Page=0;
					}
				}
				else
				{
					if(Current_Page==0)
					{
						change_screen(1);
						Current_Page=1;
					}
					else
					{
						change_screen(2);
						Current_Page=0;
					}
					Baidiheizi=0;	
				}
				
			}
			
			 /*******FAIL�ɿ�*******UART1Buffer[673]�ĳ�UART1Buffer[674]*************/
			if((Flag_start>>2)==1) 						//��ɫ����ģʽ   4
			{
				if((UART1Buffer[674]>>4)==1)
				{
					status_screen=1;
					change_screen(UART1Buffer[674]+25);
				}
			}
			/************************************************************/
			if((Flag_start1>>1)==1)						//����ǩ��   2
			{
				fill_lable(UART1Buffer);
				status_screen=1;
			}
		}
		
  }
}

