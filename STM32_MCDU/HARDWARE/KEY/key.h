#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
   	 
struct io_port {
GPIO_TypeDef *GPIO_x;
unsigned short GPIO_pin;
};

static struct io_port key_input_down[8] = {
{GPIOB, GPIO_Pin_10}, {GPIOB, GPIO_Pin_13},
{GPIOB, GPIO_Pin_15}, {GPIOB, GPIO_Pin_14},
{GPIOB, GPIO_Pin_11}, {GPIOB, GPIO_Pin_12},
{GPIOC, GPIO_Pin_6}, {GPIOC, GPIO_Pin_8},
};

static struct io_port key_output_down[6] = {
{GPIOB, GPIO_Pin_0}, {GPIOB, GPIO_Pin_1},
{GPIOC, GPIO_Pin_7}, {GPIOC, GPIO_Pin_9},
{GPIOC, GPIO_Pin_10}, {GPIOC, GPIO_Pin_11}
};

static struct io_port key_input_up[8] = {
{GPIOA, GPIO_Pin_2}, {GPIOA, GPIO_Pin_3},
{GPIOA, GPIO_Pin_4}, {GPIOA, GPIO_Pin_5},
{GPIOA, GPIO_Pin_6}, {GPIOA, GPIO_Pin_7},
{GPIOC, GPIO_Pin_4}, {GPIOC, GPIO_Pin_5},
};

static struct io_port key_output_up[6] = {
{GPIOC, GPIO_Pin_0}, {GPIOC, GPIO_Pin_1},
{GPIOC, GPIO_Pin_2}, {GPIOC, GPIO_Pin_3},
{GPIOA, GPIO_Pin_0}, {GPIOA, GPIO_Pin_1}
};

//输出引脚   PC1 PC2 PC3 PA0 PA1 PA2 PA3 PA4
static struct io_port key_output[8] = {
{GPIOC, GPIO_Pin_1}, {GPIOC, GPIO_Pin_2},
{GPIOC, GPIO_Pin_3}, {GPIOA, GPIO_Pin_0},
{GPIOA, GPIO_Pin_1}, {GPIOA, GPIO_Pin_2},
{GPIOA, GPIO_Pin_3}, {GPIOA, GPIO_Pin_4}
};

//输入引脚  PA5 PA6 PA7 PC4 PC5 PB0 PB1 PB10
static struct io_port key_input[8] = {
{GPIOA, GPIO_Pin_5}, {GPIOA, GPIO_Pin_6},
{GPIOA, GPIO_Pin_7}, {GPIOC, GPIO_Pin_4},
{GPIOC, GPIO_Pin_5}, {GPIOB, GPIO_Pin_0},
{GPIOB, GPIO_Pin_1}, {GPIOB, GPIO_Pin_10}
};

#define KEY_NULL_VALUE 0

/* 定义按键处理状态 */
#define        KEY_INIT_STATE_DOWN              0
#define        KEY_WOBBLE_STATE_DOWN            1
#define        KEY_PRESS_STATE_DOWN             2
#define        KEY_LONG_STATE_DOWN              3
/* 长按时间 */
#define         KEY_LONG_PERIOD                40       /* 2S */
#define 			KEYDOWN							1
#define 			NOKEYDOWN						0

#define 			KEY_UP							0
#define 			KEY_DOWN						1

void KEY_Init(void);//IO初始化
u8 GetKeyValue ( u8 upordown );
u8 IsKeyDown(u8 upordown);
u8 KEY_Scan(void);
//按键扫描函数					    
#endif
