/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : ����׷��@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 3/31/2015
* Description        : QiFree IO .
********************************************************************************
* merafour.blog.163.com
*
*******************************************************************************/
#ifndef __QIFREE_IO_H
#define __QIFREE_IO_H	 

#include <stdint.h>
//#include "stm32f10x.h"
#include "stm32f4xx.h" 

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
extern int QiFreeIO_Default(uint32_t cfg[]);

/******************************** Examples **************************************/
extern const uint32_t ConfigExamples[];
extern QiFreeIO led1;
extern QiFreeIO led2;
extern QiFreeIO key;
extern QiFree_ioObj Qi;
extern uint32_t UserData; // KB
extern void QiFreeConfigIO(uint32_t map[]);
extern void QiFreeConfigLog(uint32_t map[]);

		 				    
#endif // __QIFREE_IO_H
