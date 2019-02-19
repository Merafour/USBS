#include "key.h"
#include "stm32f10x.h"
//#include "mass_mal.h"
#include "log.h"
//#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//按键输入 驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/06
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									   
//////////////////////////////////////////////////////////////////////////////////	 

#define KEY_USB  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)//读取按键1
//#define KEY_USB  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键1

//按键初始化函数 
//PA15和PC5 设置成输入
#if 0
static void KEY_Init_Test(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTD时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PA15
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA15
	//GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PA1
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	//GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA1	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOC5	
} 
#endif
static void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//使能PORTD时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PA15
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA15
	//GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PA1
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	//GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA1	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOC5	
} 
static unsigned char KEY_Scan(unsigned char mode)
{	 
	if(KEY_USB==0)return USB_PRES;
	return 0;// 无按键按下
}
static unsigned char _USB(void)
{	 
	if(KEY_USB==0)return USB_PRES;
	return 0;// 无按键按下
}

static KeyObj key_modele;
KeyObj* NewKey_Obj(void)
{
	key_modele.Init = KEY_Init;
	key_modele.Scan = KEY_Scan;
	key_modele.USB = _USB;
	return &key_modele;
}
