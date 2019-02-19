#include "key.h"
#include "stm32f10x.h"
//#include "mass_mal.h"
#include "log.h"
//#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//�������� ��������		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/06
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									   
//////////////////////////////////////////////////////////////////////////////////	 

#define KEY_USB  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)//��ȡ����1
//#define KEY_USB  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//��ȡ����1

//������ʼ������ 
//PA15��PC5 ���ó�����
#if 0
static void KEY_Init_Test(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTDʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PA15
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA15
	//GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PA1
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	//GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA1	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOC5	
} 
#endif
static void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//ʹ��PORTDʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PA15
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA15
	//GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PA1
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	//GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA1	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOC5	
} 
static unsigned char KEY_Scan(unsigned char mode)
{	 
	if(KEY_USB==0)return USB_PRES;
	return 0;// �ް�������
}
static unsigned char _USB(void)
{	 
	if(KEY_USB==0)return USB_PRES;
	return 0;// �ް�������
}

static KeyObj key_modele;
KeyObj* NewKey_Obj(void)
{
	key_modele.Init = KEY_Init;
	key_modele.Scan = KEY_Scan;
	key_modele.USB = _USB;
	return &key_modele;
}
