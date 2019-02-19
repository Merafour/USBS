#include "led.h"
#include "sys.h"

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

#define OFF_SWD 1 // �ر����ߵ���
//#undef OFF_SWD

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
static void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	    		 //LED1-->PC.12 �˿�����, �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOC,GPIO_Pin_12); 						 //PC.12 ����� 

#ifdef OFF_SWD // �ر����ߵ���
	// LED ʹ����SWDIO(PA13)
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // �ر�jtag��Ҫʹ�ܸ�ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); // �ر�jtag���ر�SWD��ֻ��ʹ�ô�����¼

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA13�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	    		 //LED1-->PA.13 �˿�����, �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOA,GPIO_Pin_13); 						 //PA.13 ����� 
#endif
}

static void LED1ON(int on)
{
	//GPIO_ResetBits(GPIOE,GPIO_Pin_1);//LED1 = 0;
	on &= 0x1; // 1:LED��
	on = ~on;
	PCout(12) = (on&0x1);
}
static void LED2ON(int on)
{
	on &= 0x1; // 1:LED��
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

