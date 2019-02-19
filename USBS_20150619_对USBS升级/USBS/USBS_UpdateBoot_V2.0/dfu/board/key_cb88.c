#include "key.h"
#include "stm32f10x.h"
#include "mass_mal.h"
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

#define KEY_USB  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)//��ȡ����1
//#define KEY_USB  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//��ȡ����1
 
static void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//ʹ��PORTDʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//PC5
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
//	// 20K bootloader
//	// 4K �û�����
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
