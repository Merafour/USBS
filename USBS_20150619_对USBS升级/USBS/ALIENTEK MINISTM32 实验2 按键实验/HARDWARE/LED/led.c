#include "led.h"

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
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//static uint32_t RCC_APB2Periph=RCC_APB2Periph_GPIOA;
//static GPIO_TypeDef* GPIOx=GPIOA;
//static uint16_t GPIO_Pin=GPIO_Pin_8;
//LED IO初始化
//void LED_Init(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
void LED_Init(QiFreeIO *io)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(io->RCC_APB2Periph, ENABLE);	 //使能PA,PD端口时钟
	
	GPIO_InitStructure.GPIO_Pin = io->Pin;				 //LED0-->PA.8 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(io->GPIOx, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
	GPIO_SetBits(io->GPIOx, io->Pin);						 //PA.8 输出高
	//GPIO_ResetBits(io->GPIOx, io->Pin);
}
void LEDON(QiFreeIO *io, int on)
{
	//GPIO_ResetBits(GPIOE,GPIO_Pin_1);//LED1 = 0;
	//PEout(1) = (on&0x1);	   
#if 1
	on &= 0x1; // 1:LED亮
	on = ~on;
	GPIO_WriteBit(io->GPIOx, io->Pin, on&0x1);
#else
	if(on&0x1) GPIO_ResetBits(io->GPIOx, io->Pin);
	else GPIO_SetBits(io->GPIOx, io->Pin);
#endif
}
void LEDNOT(QiFreeIO *io)
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	
//	GPIO_InitStructure.GPIO_Pin = io->Pin;				 //LED0-->PA.8 端口配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
//	GPIO_Init(io->GPIOx, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
	//GPIO_ResetBits(GPIOE,GPIO_Pin_1);//LED1 = 0;
	//PEout(1) = (on&0x1);
	//int on = GPIO_ReadInputDataBit(io->GPIOx, io->Pin);
	int on = GPIO_ReadOutputDataBit(io->GPIOx, io->Pin);
	//LED_Init(io);
#if 1
	on &= 0x1; // 1:LED亮
	on = ~on;
	GPIO_WriteBit(io->GPIOx, io->Pin, on&0x1);
#else
	if(on&0x1) GPIO_ResetBits(io->GPIOx, io->Pin);
	else GPIO_SetBits(io->GPIOx, io->Pin);
#endif
}

void LED_Init_old()
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //使能PA,PD端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
 GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PA.8 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 端口配置, 推挽输出
 GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 输出高 
}
 
