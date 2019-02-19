#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK mini�SSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//static uint32_t RCC_APB2Periph=RCC_APB2Periph_GPIOA;
//static GPIO_TypeDef* GPIOx=GPIOA;
//static uint16_t GPIO_Pin=GPIO_Pin_8;
//LED IO��ʼ��
//void LED_Init(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
void LED_Init(QiFreeIO *io)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(io->RCC_APB2Periph, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = io->Pin;				 //LED0-->PA.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(io->GPIOx, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
	GPIO_SetBits(io->GPIOx, io->Pin);						 //PA.8 �����
	//GPIO_ResetBits(io->GPIOx, io->Pin);
}
void LEDON(QiFreeIO *io, int on)
{
	//GPIO_ResetBits(GPIOE,GPIO_Pin_1);//LED1 = 0;
	//PEout(1) = (on&0x1);	   
#if 1
	on &= 0x1; // 1:LED��
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
//	GPIO_InitStructure.GPIO_Pin = io->Pin;				 //LED0-->PA.8 �˿�����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
//	GPIO_Init(io->GPIOx, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
	//GPIO_ResetBits(GPIOE,GPIO_Pin_1);//LED1 = 0;
	//PEout(1) = (on&0x1);
	//int on = GPIO_ReadInputDataBit(io->GPIOx, io->Pin);
	int on = GPIO_ReadOutputDataBit(io->GPIOx, io->Pin);
	//LED_Init(io);
#if 1
	on &= 0x1; // 1:LED��
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
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
 GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PA.8 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 �˿�����, �������
 GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 ����� 
}
 
