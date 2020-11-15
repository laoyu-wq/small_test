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
	Initial_UART5(115200);//屏幕
	KEY_Init();
	Adc_Init();
	while(1)
  {
		
    /*****************按键处理部分***************************/
		Key=KEY_Scan();
		if(Key!=KEY_NULL_VALUE)//按键检测
		{
			data[1]=Key;
			USARTSend(1,data,3);  //向P3D发送按键键值
	   // GPIO_ResetBits(GPIOC,GPIO_Pin_10);			
		}
		
		Brightn_Change(1);
	
	 /*******************屏幕刷新部分***************************/
			if((Flag_start%2)==0)
		{
			Flag_start=Flag_start|1;	//复位标志位
			//status_screen=0;
			//status_link=1;
			if((Flag_start>>3)==1)//填充屏幕   8
			{
				
				fill_screen(UART1Buffer); //像对应控件里写数据
				
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
			
			 /*******FAIL可控*******UART1Buffer[673]改成UART1Buffer[674]*************/
			if((Flag_start>>2)==1) 						//颜色测试模式   4
			{
				if((UART1Buffer[674]>>4)==1)
				{
					status_screen=1;
					change_screen(UART1Buffer[674]+25);
				}
			}
			/************************************************************/
			if((Flag_start1>>1)==1)						//填充便签行   2
			{
				fill_lable(UART1Buffer);
				status_screen=1;
			}
		}
		
  }
}

