/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 3/31/2015
* Description        : QiFree IO 程序.
********************************************************************************
* merafour.blog.163.com
*
*******************************************************************************/
#ifndef __QIFREE_IO_H
#define __QIFREE_IO_H	 

#include <stdint.h>
#include "stm32f10x.h"

//void LED_Init(void);//初始化

typedef struct {
	uint32_t RCC_APB2Periph;
	GPIO_TypeDef* GPIOx;
	uint16_t Pin;
}QiFreeIO;

typedef struct { // 输出设备
	//void (*init)(QiFreeIO *io); // 初始化
	void (*led_init)(QiFreeIO *io); // led初始化
	void (*key_init)(QiFreeIO *io); // key初始化
	void (*led_on)(QiFreeIO *io, int on);   // 点灯,输出
	void (*reversal)(QiFreeIO *io); // 电平翻转,输出
	uint8_t (*key_down)(QiFreeIO *io); // 读取电平,输入
}QiFree_ioObj;
/*
 * 创建一个 IO操作对象, input表示 IO是输入还是输出, LED传递 0即输出, KEY传递 1即输入
 */
extern void newQiFree_ioObj(QiFree_ioObj *Qi, int led_low, int key_low);
extern int QiFreeIO_config(uint32_t cfg[], QiFreeIO *io);

//extern void LED_Init(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
//extern void LED_Init(QiFreeIO *io);
//extern void LEDON(QiFreeIO *io, int on);
//extern void LEDNOT(QiFreeIO *io);

		 				    
#endif // __QIFREE_IO_H
