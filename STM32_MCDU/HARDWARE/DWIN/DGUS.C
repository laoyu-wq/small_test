#include "stm32f10x.h"
#include "string.h"
#include "UART5.h"
#include "DGUS.h"
#include "adc.h"
#include "sys.h"

#define FAIL   BIT_ADDR(GPIOC_ODR_Addr,9)
#define OFST   BIT_ADDR(GPIOC_ODR_Addr,8)
#define CALL   BIT_ADDR(GPIOA_ODR_Addr,8)
#define EXEC   BIT_ADDR(GPIOC_ODR_Addr,6)
#define MSG    BIT_ADDR(GPIOC_ODR_Addr,7)

#define bufsize 100
#define uartsendbyte(x) UART5_Put_Char(x)
#define uartsendstring(x,y) UART5_Put_array(x,y)

uint8_t Current_Page=0;
uint8_t Baidiheizi=0;
extern uint8_t status_link;




extern uint8_t status_link;

void head(void)
{
	uint8_t temp[]={0XAA,0XCC,151,0X82};
	uartsendstring(temp,4);
}
void fill_screen(uint8_t *BufferPtr)
{
	uint8_t str[]={0X01,0X00,0X00,0X0C,0X00,12};
	uint8_t temp_cdu[]={0XAA,0X98,0X00,0X00,0X00,0X00,0X00,0XC1,0X02,0XFF,0XFF,0X00,0X00,0X00,0XCC,0X33,0XC3,0X3C};
	uint8_t temp2[]={0XAA,0X56,0X0D,0X82,0X40,0X02,0X00,0X00,0X00,0X00,0X00,0X00,0XCC,0X33,0XC3,0X3C};	//0X02,0X7F,0X01,0XDF,0X7C
	uint8_t temp3[]={0XAA,0X56,0X07,0X82,0X40,0X00,0X00,0X04,0X00,0X01};
	uint8_t temp4[]={0XAA,0X56,0X0D,0X82,0X40,0X02,0X00,0X00,0X00,0X00,0X00,0X00,0XCC,0X33,0XC3,0X3C};	//0X02,0X7F,0X01,0XDF,0X7C
	uint8_t temp5[]={0XAA,0X56,0X07,0X82,0X40,0X00,0X00,0X04,0X00,0X01};
	uint8_t temp6[]={0XAA,0X56,0X07,0X82,0X40,0X00,0X00,0X04,0X00,0X00};
	uint8_t i,j,k=0,l=0;
	uint16_t status=0,status1=0;
	uint32_t temp;
	//指示灯显示
//	OFST=!((BufferPtr[0]-1)&1);
//	CALL=!(((BufferPtr[0]-1)>>1)&1);
//	EXEC=!(((BufferPtr[0]-1)>>2)&1);
//	MSG=!(((BufferPtr[0]-1)>>3)&1);
//	FAIL=!(((BufferPtr[0]-1)>>4)&1);
	////FAIL=(((BufferPtr[0])>>4)&1);
	//哈哈哈哈
//	if(Current_Page==0)
//	{
//		uartsendstring(temp6,10);
//		temp6[4]=0X42;
//		uartsendstring(temp6,10);
//	}
//	else
//	{
//		temp6[4]=0X41;
//		uartsendstring(temp6,10);
//		temp6[4]=0X43;
//		uartsendstring(temp6,10);
//	}
	/******************改变背景灯的亮度***************/
	/*******FAIL可控*******BufferPtr[0]改成BufferPtr[1]*************/
//	 if(((BufferPtr[1]-1))==0)
//	{
//		HT1632C_Writer_CMD(LED_OFF);
//	}
//	else
//	{
//		HT1632C_Writer_CMD(LED_ON);
//		HT1632C_Writer_CMD(((BufferPtr[1]-1))+0XA0-1);		
//	}
	/**************************************************/
	//CDU文字显示
	for(i=0;i<26;i++)
	{
//		head();
//		//USARTSend(2,temp1,4);
//		//首字母坐标
//		if(Current_Page==0)
//		{
//			str[0]=2*i+1;
//		}
//		else
//		{
//			str[0]=2*i+2;
//		}
//		uartsendstring(str,6);
		/***********************************/
		temp=(i%2)*12*BG_WIDTH+X_DIST;
		temp_cdu[2]=temp/256;
		temp_cdu[3]=temp%256;
		temp_cdu[4]=((i/2)*BG_HEIGHT+Y_DIST)/256;
		temp_cdu[5]=((i/2)*BG_HEIGHT+Y_DIST)%256;
		for(j=0;j<12;j++)
		{
			//处理ASCII码
		  /*******FAIL可控*******BufferPtr[i*24+j*2+1]改成BufferPtr[i*24+j*2+2]*************/
			temp_cdu[13]=BufferPtr[i*24+j*2+2];
			//处理字体大小
			/*******FAIL可控*******BufferPtr[i*24+j*2+2]改成BufferPtr[i*24+j*2+3]*************/
			if(((BufferPtr[i*24+j*2+3]-1)&0X1)==0)
			{
				temp_cdu[8]=0X03;	   		//字体为Normal
				temp_cdu[9]=0XFF;
				temp_cdu[6]=0X00;				//字库位置
			}
			else
			{
				temp_cdu[8]=0X02;	   			//字体为Small
				temp_cdu[9]=0XFF;
				temp_cdu[6]=0X00;				//字库位置
			}
			//处理字体颜色
			/*******FAIL可控*******BufferPtr[i*24+j*2+2]改成BufferPtr[i*24+j*2+3]*************/
		switch((BufferPtr[i*24+j*2+3]-1)>>1)
		{
			case 0:	
			{
				temp_cdu[9]=0XFF;
				temp_cdu[10]=0XFF;
				if((status%2)==1)
				{
					status=status+(j<<5);
					status=status-1;
					status=status+0X8000;
				}
				if((status1%2)==1)
				{
					status1=status1+(j<<5);
					status1=status1-1;
					status1=status1+0X8000;
				}
			} break;	//White
			case 1:	
			{
				temp_cdu[9]=0X07;
				temp_cdu[10]=0XE0;
				if((status%2)==1)
				{
					status=status+(j<<5);
					status=status-1;
					status=status+0X8000;
				}
				if((status1%2)==1)
				{
					status1=status1+(j<<5);
					status1=status1-1;
					status1=status1+0X8000;
				}
			} break;	//Green
			case 2:	
			{
				temp_cdu[9]=0X34;
				temp_cdu[10]=0XFF;
				if((status%2)==1)
				{
					status=status+(j<<5);
					status=status-1;
					status=status+0X8000;
				}
				if((status1%2)==1)
				{
					status1=status1+(j<<5);
					status1=status1-1;
					status1=status1+0X8000;
				}
			} break;	//Cyan
			case 3:	
			{
				temp_cdu[9]=0X80;
				temp_cdu[10]=0X1F;
				if((status%2)==1)
				{
					status=status+(j<<5);
					status=status-1;
					status=status+0X8000;
				}
				if((status1%2)==1)
				{
					status1=status1+(j<<5);
					status1=status1-1;
					status1=status1+0X8000;
				}
			} break;	//Magenta
      case 4:	
				{
					temp_cdu[0]=0XFF;
					temp_cdu[1]=0XFF;
					if(status==0)
					{
						status=status+j*2;
						status=status+1;
						Baidiheizi=1;
					}
					if(((status>>15)==1)&&(status1==0))
					{
						status1=status1+j*2;
						status1=status1+1;	
					}	
				} break;	//灰底白字
				default: break;
		}
			//处理文字位置
			temp_cdu[2]=(temp+j*BG_WIDTH)/256;
			temp_cdu[3]=(temp+j*BG_WIDTH)%256;
			uartsendstring(temp_cdu,18);
			//USARTSend(1,&status,1);
			/***********************************/
		}
		
		
//		if((status!=0)&&(status>>15)!=1)
//		{
//			status=status|0X8000;
//			status=status+(12<<5);
//		}
//		if((status1!=0)&&(status1>>15)!=1)
//		{
//			status1=status1|0X8000;
//			status1=status1+(12<<5);
//		}			
//		if((status>>15)==1)
//		{
//			 k++;
//			 temp3[9]=k;
//			 temp2[5]=5*k-3;
//			 temp2[6]=(temp+(status%32)/2*BG_WIDTH)/256;
//			 temp2[7]=(temp+(status%32)/2*BG_WIDTH)%256;
//			 temp2[8]=temp_cdu[4];
//			 temp2[9]=temp_cdu[5];
//			 temp2[10]=(temp+((status>>5)%16)*BG_WIDTH)/256;
//			 temp2[11]=(temp+((status>>5)%16)*BG_WIDTH)%256;
//			 temp2[12]=((i/2)*BG_HEIGHT+Y_DIST+BG_HEIGHT)/256;
//			 temp2[13]=((i/2)*BG_HEIGHT+Y_DIST+BG_HEIGHT)%256;
//			 if(Current_Page==0)
//			 {
//			 	temp2[4]=0X40;
//				temp3[4]=0X40;	
//			 }
//			 else
//			 {
//			 	temp2[4]=0X41;
//				temp3[4]=0X41;	
//			 }
//			 uartsendstring(temp2,16);
//			 uartsendstring(temp3,10);
//			 status=0;
//		}
//		if((status1>>15)==1)
//		{
//			 l++;
//			 temp5[9]=l;
//			 temp4[5]=5*l-3;
//			 temp4[6]=(temp+(status1%32)/2*BG_WIDTH)/256;
//			 temp4[7]=(temp+(status1%32)/2*BG_WIDTH)%256;
//			 temp4[8]=temp_cdu[4];
//			 temp4[9]=temp_cdu[5];
//			 temp4[10]=(temp+((status1>>5)%16)*BG_WIDTH)/256;
//			 temp4[11]=(temp+((status1>>5)%16)*BG_WIDTH)%256;
//			 temp4[12]=((i/2)*BG_HEIGHT+Y_DIST+BG_HEIGHT)/256;
//			 temp4[13]=((i/2)*BG_HEIGHT+Y_DIST+BG_HEIGHT)%256;
//			 if(Current_Page==0)
//			 {
//			 	temp4[4]=0X42;
//				temp5[4]=0X42;	
//			 }
//			 else
//			 {
//			 	temp4[4]=0X43;
//				temp5[4]=0X43;	
//			 }
//			 uartsendstring(temp4,16);
//			 uartsendstring(temp5,10);
//			 status1=0;
//		}
	}	
}

void fill_lable(uint8_t *BufferPtr)
{
	uint8_t str[]={0X01,0X00,0X00,0X0C,0X00,12};
	uint8_t temp_cdu[]={/**/0X3F,0XBD,/**/0X00,0X00,0X00,0X00,/**/25,/**/0X05,/**/24,32,0X00,0X35};
	uint8_t i,j;
	uint32_t temp;
	for(i=0;i<2;i++)
	{
		head();
		//首字母坐标
		str[0]=53+2*i;
		uartsendstring(str,6);
		temp=(i%2)*12*BG_WIDTH+X_DIST;
		temp_cdu[4]=(13*BG_HEIGHT+Y_DIST)/256;
		temp_cdu[5]=(13*BG_HEIGHT+Y_DIST)%256;
		for(j=0;j<12;j++)
		{
			//处理ASCII码
			/*******FAIL可控*******BufferPtr[i*24+j*2+625]改成BufferPtr[i*24+j*2+626]*************/
			temp_cdu[11]=BufferPtr[i*24+j*2+626];
			//处理字体大小
			/*******FAIL可控*******BufferPtr[i*24+j*2+626]改成BufferPtr[i*24+j*2+627]*************/
			if(((BufferPtr[i*24+j*2+627]-1)&0X1)==0)
			{
				temp_cdu[8]=24;	   			//字体为Normal
				temp_cdu[9]=32;
				temp_cdu[6]=25;				//字库位置
			}
			else
			{
				temp_cdu[8]=24;	   			//字体为Small
				temp_cdu[9]=32;
				temp_cdu[6]=24;				//字库位置
			}
			//处理字体颜色
						/*******FAIL可控*******BufferPtr[i*24+j*2+626]改成BufferPtr[i*24+j*2+627]*************/
			switch((BufferPtr[i*24+j*2+627]-1)>>1)
			{
				case 0:	{temp_cdu[0]=0XFF;temp_cdu[1]=0XFF;} break;	//White
				case 1:	{temp_cdu[0]=0X07;temp_cdu[1]=0XE0;} break;	//Green
				case 2:	{temp_cdu[0]=0X07;temp_cdu[1]=0XFF;} break;	//Cyan
				case 3:	{temp_cdu[0]=0XF8;temp_cdu[1]=0X1F;} break;	//Magenta
				case 4:	{temp_cdu[0]=0X7C;temp_cdu[1]=0X0F;} break;	//Gray
				default: break;
			}
			//处理文字位置
			temp_cdu[2]=(temp+j*BG_WIDTH)/256;
			temp_cdu[3]=(temp+j*BG_WIDTH)%256;
			uartsendstring(temp_cdu,12);
		}	
	}	
}

void change_screen(uint8_t page)
{ 
	uint8_t temp[]={0XAA,0XCC,4,0X80,3,0,0};
	temp[6]=page;
	uartsendstring(temp,7);
}


void Brightn_Change( char BrightNum) //0x00-0x40
{

  unsigned char temp[6]={0XAA,0XCC,3,0X80,1,0};
	u16  brightness;
	if(BrightNum==1)
	{
	brightness=Get_Adc_Average(10,4);
  temp[5]=brightness*0X40/4096/4*0.8+1;
	}
	uartsendstring(temp,6);

}


void Clear(void)
{
	
	






}

