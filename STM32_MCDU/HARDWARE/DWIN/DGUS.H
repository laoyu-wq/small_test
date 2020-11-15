#ifndef __DGUS_H__
#define __DGUS_H__

#include <stdio.h>
#include "stm32f10x.h"
//用于文本框显示
/*
#define chart0 0x0100
#define chart1 0x0200
#define chart2 0x0300
#define chart3 0x0400
#define chart4 0x0500
*/

/************发送帧头AACC********************/

#define BG_HEIGHT	32 
#define BG_WIDTH	23

#define X_DIST	48
#define Y_DIST	23

//void head(void);

//void Display_Num(int Address,int Num);
//void Display_String(int Address,char *String);
//void Read_PW(int Address);
//void Read_Word(int Address);
//void uartsendbyte(char bytedata);
//void uartsendstr(unsigned char *str);
//void DWIN_SDRAMToFlash( char FontLibNum, uint32_t FLStartAddr, 
//                        uint16_t SDRAM_StartAddr, uint16_t OP_Length );
//void Read_Reg( int addr ); //读取.dat文件
//void Switch_Screen(int Number);
//void Flash_Icon(int Address ,uint8_t state ) ;
//void Select_Icon(int Address ,uint8_t num ) ;
//void Pic_sectionInit( void );
//void Picture_line( char lineNum, uint16_t* nPos, uint16_t addr, uint16_t nColor );
//void Picture_rectangle( char rectNum, uint16_t* nPosCol, uint16_t addr );
//void Test_copyPaste( void );
//void Send_msgPacket(int Address,char *String, char len);
//void Write_PW( void );
//void Write_Beeper( uint8_t ntenms ); //蜂鸣器
//void Write_RTC( char *nDateTime );   //修改时钟
//void Read_RTC( void );
//void DWIN_OpenBuz( void );
//void Display_RTC(int Address,u32 Time);
//void Read_ID( void );   //test 10 inch screen
//void Set_LED_Luminance( char num );
//void Dis_Time_Str( int Address, int timecntSec );
//void Brightn_Change( char BrightNum);

void head(void);
void fill_screen(uint8_t *BufferPtr);
void fill_lable(uint8_t *BufferPtr);
void change_screen(uint8_t page);
void Brightn_Change( char BrightNum);
#endif


