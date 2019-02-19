/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : ����׷��@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 3/31/2015
* Description        : QiFree IO ����.
********************************************************************************
* merafour.blog.163.com
*
*******************************************************************************/
#include "QiFreeIO.h"

// LED��ʼ��
static void QiFree_led_Init(QiFreeIO *io)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	if(0==io->GPIOx) return; // �յ�����io->GPIOx
	RCC_APB2PeriphClockCmd(io->RCC_APB2Periph, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = io->Pin;				 //LED0-->PA.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(io->GPIOx, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
	//GPIO_SetBits(io->GPIOx, io->Pin);						 //PA.8 �����
	GPIO_ResetBits(io->GPIOx, io->Pin);
}
// LED����,�ߵ�ƽ����
static void QiFree_led_on(QiFreeIO *io, int on)
{	
	if(0==io->GPIOx) return; // �յ�����io->GPIOx   
	GPIO_WriteBit(io->GPIOx, io->Pin, on&0x1);
}
// LED����,�͵�ƽ����
static void QiFree_led_on_low(QiFreeIO *io, int on)
{	   
	if(0==io->GPIOx) return; // �յ�����io->GPIOx
	on &= 0x1; // 1:LED��
	on = ~on;
	GPIO_WriteBit(io->GPIOx, io->Pin, on&0x1);
}
// LED��ƽ��ת
static void QiFree_led_reversal(QiFreeIO *io)
{
	int on = 0;
	if(0==io->GPIOx) return; // �յ�����io->GPIOx
	//int on = GPIO_ReadInputDataBit(io->GPIOx, io->Pin);
	on = GPIO_ReadOutputDataBit(io->GPIOx, io->Pin);
	on &= 0x1; // 1:LED��
	on = ~on;
	GPIO_WriteBit(io->GPIOx, io->Pin, on&0x1);
}

// KEY��ʼ��
static void QiFree_key_Init(QiFreeIO *io)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	if(0==io->GPIOx) return; // �յ�����io->GPIOx
	RCC_APB2PeriphClockCmd(io->RCC_APB2Periph, ENABLE);	 //ʹ��PA,PD�˿�ʱ��

 	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTCʱ��
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIO_InitStructure.GPIO_Pin = io->Pin;				 //PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        //���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(io->GPIOx, &GPIO_InitStructure);					 //��ʼ��GPIOA15
}

// KEY��ȡ��ƽ,�ߵ�ƽ����
static uint8_t QiFree_key_down(QiFreeIO *io)
{
	uint8_t down = 0;
	if(0==io->GPIOx) return 0; // �յ�����io->GPIOx
	down = GPIO_ReadInputDataBit(io->GPIOx, io->Pin);
	//int on = GPIO_ReadOutputDataBit(io->GPIOx, io->Pin);
	down &= 0x1; // 
	return down;
}
// KEY��ȡ��ƽ,�͵�ƽ����
static uint8_t QiFree_key_down_low(QiFreeIO *io)
{
	uint8_t down = 0;
	if(0==io->GPIOx) return 0; // �յ�����io->GPIOx
	down = GPIO_ReadInputDataBit(io->GPIOx, io->Pin);
	//int on = GPIO_ReadOutputDataBit(io->GPIOx, io->Pin);
	down = ~down;
	down &= 0x1; // 
	return down;
}

/*
 * ����һ�� IO��������, input��ʾ IO�����뻹�����, LED���� 0�����, KEY���� 1������
 */
void newQiFree_ioObj(QiFree_ioObj *Qi, int led_low, int key_low)
{
	Qi->led_init = QiFree_led_Init;
	Qi->key_init = QiFree_key_Init;
	Qi->led_on = QiFree_led_on;
	Qi->reversal = QiFree_led_reversal;
	Qi->key_down = QiFree_key_down;
	// LED�͵�ƽ����
	if(led_low) Qi->led_on = QiFree_led_on_low;
	// KEY�͵�ƽ��Ч
	if(key_low) Qi->key_down = QiFree_key_down_low;
}
// IO���ö���
static uint32_t QiFree_RCC_GPIOx[7]={
	RCC_APB2Periph_GPIOA,
	RCC_APB2Periph_GPIOB,
	RCC_APB2Periph_GPIOC,
	RCC_APB2Periph_GPIOD,
	RCC_APB2Periph_GPIOE,
	RCC_APB2Periph_GPIOF,
	RCC_APB2Periph_GPIOG
};
static GPIO_TypeDef* QiFree_GPIOx[7] = {
	GPIOA,
	GPIOB,
	GPIOC,
	GPIOD,
	GPIOE,
	GPIOF,
	GPIOG
};
static uint16_t QiFree_Pin[16] = {
	GPIO_Pin_0,
	GPIO_Pin_1,
	GPIO_Pin_2,
	GPIO_Pin_3,
	GPIO_Pin_4,
	GPIO_Pin_5,
	GPIO_Pin_6,
	GPIO_Pin_7,
	GPIO_Pin_8,
	GPIO_Pin_9,
	GPIO_Pin_10,
	GPIO_Pin_11,
	GPIO_Pin_12,
	GPIO_Pin_13,
	GPIO_Pin_14,
	GPIO_Pin_15
};
int QiFreeIO_config(uint32_t cfg[], QiFreeIO *io)
{
	uint32_t gpio=0;
	uint32_t size=0;
	// init
	io->RCC_APB2Periph = 0;
	io->GPIOx = 0;
	io->Pin = 0;
	// RCC_GPIOx
	gpio = cfg[0] - 'A';
	size = sizeof(QiFree_RCC_GPIOx)/sizeof(QiFree_RCC_GPIOx[0])-1;
	//if(cfg[0]>=(sizeof(QiFree_RCC_GPIOx)/sizeof(QiFree_RCC_GPIOx[0]))) return -1;
	if((cfg[0]<'A') || (cfg[0]>('A'+size))) return -1;
	if(cfg[1]>=(sizeof(QiFree_Pin)/sizeof(QiFree_Pin[0]))) return -1;
#if 1
	//io->RCC_APB2Periph = QiFree_RCC_GPIOx[cfg[0]];
	//io->GPIOx = QiFree_GPIOx[cfg[0]];
	io->RCC_APB2Periph = QiFree_RCC_GPIOx[gpio];
	io->GPIOx = QiFree_GPIOx[gpio];
	io->Pin = QiFree_Pin[cfg[1]];
#else
	io->RCC_APB2Periph = QiFree_RCC_GPIOx[0];
	io->GPIOx = QiFree_GPIOx[0];
	io->Pin = QiFree_Pin[8];
//	io->RCC_APB2Periph = RCC_APB2Periph_GPIOA;
//	io->GPIOx = GPIOA;
//	io->Pin = GPIO_Pin_8;
#endif
	return 0;
}
