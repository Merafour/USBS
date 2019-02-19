#include "led.h"
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  

#define OFF_SWD 1 // 关闭在线调试
//#undef OFF_SWD

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
static void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	    		 //LED1-->PC.12 端口配置, 推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOC,GPIO_Pin_12); 						 //PC.12 输出高 

#ifdef OFF_SWD // 关闭在线调试
	// LED 使用了SWDIO(PA13)
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 关闭jtag需要使能该时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); // 关闭jtag，关闭SWD，只能使用串口烧录

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA13端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	    		 //LED1-->PA.13 端口配置, 推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOA,GPIO_Pin_13); 						 //PA.13 输出高 
#endif
}

static void LED1ON(int on)
{
	//GPIO_ResetBits(GPIOE,GPIO_Pin_1);//LED1 = 0;
	on &= 0x1; // 1:LED亮
	on = ~on;
	PCout(12) = (on&0x1);
}
static void LED2ON(int on)
{
	on &= 0x1; // 1:LED亮
	on = ~on;
#ifdef OFF_SWD
	PAout(13) = (on&0x1);
#else
	PCout(12) = (on&0x1);
#endif
}

static LEDObj led_module;
LEDObj* NewLED_Obj(void)
{
	led_module.Init = LED_Init;
	led_module.LED1ON = LED1ON;
	led_module.LED2ON = LED2ON;
	return &led_module;
}

