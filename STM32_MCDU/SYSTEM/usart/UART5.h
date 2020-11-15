#ifndef __UART5_H
#define __UART5_H

#include <stdio.h>
#include "stm32f10x.h"

void U5NVIC_Configuration(void);
void Initial_UART5(u32 baudrate);
void UART5_Put_Char(unsigned char DataToSend);
void UART5_Put_String(unsigned char *Str);
void UART5_Put_array(unsigned char *Str, uint8_t lens);
#endif
