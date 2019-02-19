#include "key.h"
#include "stm32f10x.h"
#include "mass_mal.h"
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

#define KEY_USB  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)//读取按键1
//#define KEY_USB  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键1
 
static void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//使能PORTD时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//PC5
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
//	// 20K bootloader
//	// 4K 用户数据
//	Mass_Memory_Size[0]=(long long)(256-20-4)*1024;
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
