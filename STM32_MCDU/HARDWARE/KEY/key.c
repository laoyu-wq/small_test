#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"


u8 KeyCounter_up = 0;
u8 KeyState_down = KEY_INIT_STATE_DOWN;
u8 KeyCounter_down = 0;

//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	u8 i;
	//PB11 PB12 PB13 PB14 PB15单独引脚驱动
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTE时钟

	//输出引脚  PA0 PA1 PA2 PA3 PA4 PC1 PC2 PC3
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 					//推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 					//IO口速度为50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
		//PA8 PC6 PC7 PC8 PC9 PC10灯引脚
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	
	//输入引脚  PA5 PA6 PA7 PC4 PC5 PB0 PB1 PB10
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  								//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_10);//背景灯亮
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);//CALL 呼叫灯  白色
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);//执行灯
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);//MSG  白色 
	GPIO_ResetBits(GPIOC,GPIO_Pin_8);//OFST 偏航灯 白色
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);//FAIL  故障灯 琥珀色
	
	for(i = 0; i < 8; i++)
  {
		GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
  }
}

u8 GetKeyValue ( u8 upordown )
{
	u8 i,j,key_value=NOKEYDOWN;
	if(upordown==KEY_DOWN)
	{
		for(i=0;i<8;i++)
		{
			GPIO_ResetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
			for(j=0;j<8;j++)
			{
				if(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0)
				{
					//key_value=i+8*j+51;  //按键值处理
					 if(i==0&&j<6)
					 key_value=i+8*j+51;
					 else if(i==0&&j==6)
					key_value=0x35;
					 else if(i==0&&j==7)
					key_value=0x39;
					 else if(i==1&&j<6)
					key_value=i+8*j+51;
					else if(i==1&&j==6)
					key_value=0x36;
					else if(i==1&&j==7)
					key_value=0x38;
					else if(i==2&&j==0)
					key_value=0x45;
					else if(i==2&&j>0&&j<7)
					key_value=0x3c+j;
					else if(i==2&&j==7)
					key_value=0x37;
					else if(i==3&&j<2)
					key_value=0x4d+j;
					else if(i==3&&j==2)
					key_value=0x46;
					else if(i==3&&j>2)
					key_value=1+j;
					else if(i==4&&j<3)
					key_value=0x11+j;
					else if(i==4&&j>2)
					key_value=0x09+j;
					else if(i==5&&j<3)
					key_value=0x19+j; 
					else if(i==5&&j>2)
					key_value=0x11+j;  
					else if(i==6&&j<3)
					key_value=0x21+j; 
					else if(i==6&&j>2)
					key_value=0x19+j;
					else if(i==7&&j<3)
					key_value=0x29+j; 
					else if(i==7&&j>2)
					key_value=0x21+j; 
						
				}
			
					
			}
			GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
		}
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)  
			key_value=0x2c;
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==0)
			key_value=0x2d;
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==0)
			key_value=0x2e;
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0)
			key_value=0x2f;
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
			key_value=0x30;
	}
	return key_value;
}



u8 IsKeyDown(u8 upordown)
{
	u8 i;
	if(upordown==KEY_DOWN)
	{
		for(i=0;i<8;i++)
		{
			GPIO_ResetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);
		}
		for(i=0;i<8;i++)
		{
			if(GPIO_ReadInputDataBit(key_input[i].GPIO_x, key_input[i].GPIO_pin) == 0)
			{
					return KEYDOWN;
			}
			
		}
		
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0||GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==0||GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==0||
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0||GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
	{
	
	    return KEYDOWN;  //单独5个按键按下
	
	}
	}
	
	return NOKEYDOWN;
}



u8 KEY_Scan(void)
{
	u8 KeyValue = KEY_NULL_VALUE;
	if( IsKeyDown(KEY_DOWN)!= NOKEYDOWN)  //有按键按下
	{
		//GPIO_ResetBits(GPIOC,GPIO_Pin_10);
		switch(KeyState_down)     //状态法
		{
			case KEY_INIT_STATE_DOWN:
			{
				KeyState_down = KEY_WOBBLE_STATE_DOWN;
				return KEY_NULL_VALUE;
			}
			case KEY_WOBBLE_STATE_DOWN:
			{
				delay_ms(20);
				if( IsKeyDown(KEY_DOWN)!= NOKEYDOWN)
				{
						KeyState_down = KEY_PRESS_STATE_DOWN;
				}
				else
				{
						KeyState_down = KEY_INIT_STATE_DOWN;
				}					
				return KEY_NULL_VALUE;
			}
			case KEY_PRESS_STATE_DOWN:														/* 有按键按下，返回按键值 */
			{
				//GPIO_ResetBits(GPIOC,GPIO_Pin_10);
				KeyValue = GetKeyValue(KEY_DOWN);
				KeyState_down = KEY_LONG_STATE_DOWN;
				return KeyValue;
			}
			case KEY_LONG_STATE_DOWN:                        /* 长按键 */
			{
        KeyCounter_down ++;
				delay_ms(20);
				KeyValue=GetKeyValue(KEY_DOWN);
        if((KeyCounter_down == KEY_LONG_PERIOD)&&(KeyValue==48))
        {
					KeyCounter_down = 0;
					KeyValue = 58;   //清除键长按效果
					return KeyValue;
        }
				return KEY_NULL_VALUE;				
			}
			default: return KEY_NULL_VALUE;
		}
	}
	else
	{
		KeyState_down = KEY_INIT_STATE_DOWN;
		KeyCounter_down=0;
		return KEY_NULL_VALUE;
	}
}

