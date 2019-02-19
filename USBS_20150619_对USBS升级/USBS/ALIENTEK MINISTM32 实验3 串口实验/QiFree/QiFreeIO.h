/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : ����׷��@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 3/31/2015
* Description        : QiFree IO ����.
********************************************************************************
* merafour.blog.163.com
*
*******************************************************************************/
#ifndef __QIFREE_IO_H
#define __QIFREE_IO_H	 

#include <stdint.h>
#include "stm32f10x.h"

//void LED_Init(void);//��ʼ��

typedef struct {
	uint32_t RCC_APB2Periph;
	GPIO_TypeDef* GPIOx;
	uint16_t Pin;
}QiFreeIO;

typedef struct { // ����豸
	//void (*init)(QiFreeIO *io); // ��ʼ��
	void (*led_init)(QiFreeIO *io); // led��ʼ��
	void (*key_init)(QiFreeIO *io); // key��ʼ��
	void (*led_on)(QiFreeIO *io, int on);   // ���,���
	void (*reversal)(QiFreeIO *io); // ��ƽ��ת,���
	uint8_t (*key_down)(QiFreeIO *io); // ��ȡ��ƽ,����
}QiFree_ioObj;
/*
 * ����һ�� IO��������, input��ʾ IO�����뻹�����, LED���� 0�����, KEY���� 1������
 */
extern void newQiFree_ioObj(QiFree_ioObj *Qi, int led_low, int key_low);
extern int QiFreeIO_config(uint32_t cfg[], QiFreeIO *io);

//extern void LED_Init(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
//extern void LED_Init(QiFreeIO *io);
//extern void LEDON(QiFreeIO *io, int on);
//extern void LEDNOT(QiFreeIO *io);

		 				    
#endif // __QIFREE_IO_H
