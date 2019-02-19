#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PAout(8)	// PA8
#define LED1 PDout(2)	// PD2	

//void LED_Init(void);//��ʼ��

typedef struct {
	uint32_t RCC_APB2Periph;
	GPIO_TypeDef* GPIOx;
	uint16_t Pin;
}QiFreeIO;

//extern void LED_Init(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
extern void LED_Init(QiFreeIO *io);
extern void LEDON(QiFreeIO *io, int on);

		 				    
#endif
