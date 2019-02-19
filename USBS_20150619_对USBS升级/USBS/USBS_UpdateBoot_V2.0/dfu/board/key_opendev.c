#include "key.h"
#include "stm32f10x.h"
//#include "mass_mal.h"
#include "log.h"
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

#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 
//#define KEY_USB  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)//读取按键1
#define KEY_USB  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键1

//按键初始化函数 
//PA15和PC5 设置成输入
void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTC时钟	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
} 
static unsigned char KEY_Scan(unsigned char mode)
{	 
#if 0
	if(KEY_USB==0)return USB_PRES;
	return 0;// 无按键按下
#else
	if(KEY_USB==0)return 0;
	return USB_PRES;// 无按键按下
#endif
}
static unsigned char _USB(void)
{	 
	if(KEY_USB==0)return USB_PRES;
	return 0;// 无按键按下
}

static KeyObj key_modele;
KeyObj* NewKey_Obj(void)
{
//	Mass_Memory_Size[0]=(long long)232*1024;
//	Mass_Block_Size[0] =512;
//	Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
//	lprintf("Mass_Memory_Size[%d]:%d byte\r\n", 0, Mass_Memory_Size[0]);
//	lprintf("Mass_Block_Size[%d]:%d byte\r\n", 0, Mass_Block_Size[0]);
//	lprintf("Mass_Block_Count[%d]:%d\r\n", 0, Mass_Block_Count[0]);
	key_modele.Init = KEY_Init;
	key_modele.Scan = KEY_Scan;
	key_modele.USB = _USB;
	return &key_modele;
}
