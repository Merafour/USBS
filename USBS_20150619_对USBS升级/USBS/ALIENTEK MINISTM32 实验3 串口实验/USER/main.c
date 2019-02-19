#include "QiFreeIO.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

/*
 * Flash map
 * bin       |-----------------| start
 * uint32_t 0| __initial_sp    | 000H
 *          1| Reset_Handler   | 004H
 *          2| Flash KB        | 008H
 *           |-----------------|
 *          3|                 | 00CH
 *        ...| mtext           | ...H
 *         10|                 | {8*4}B
 *           |-----------------|
 *         11| empty           |
 *           |-----------------|
 *         12| KEY_GPIO        | 030H
 *         13| KEY_Pin         |
 *         14| LED1_GPIO       |
 *         15| LED1_Pin        |
 *         16| LED2_GPIO       |
 *         17| LED2_Pin        |
 *           |-----------------|
 *         18|                 | 48H
 *        ...| Board name      | ...H
 *           |                 | {4*4}B
 *           |-----------------|
 *         22| signDate        | 58H
 *        ...|                 | ...H
 *           |                 | {8*4}B
 *           |-----------------|
 *         32| empty           | 80H
 *           |                 | ...H
 *           |                 |
 */

// 1K
//__attribute__ ((section (".boot"))) const char _boot[0x400] __attribute__((used)) = {
//       0x12,0x12,0x12,0x12,
//	   0x12,0x12,0x12,0x12
//};
extern uint32_t __Vectors;
extern uint32_t __Vectors_End;
extern uint32_t __Vectors_Size;
extern uint32_t Reset_Handler;

//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
static uint16_t match_ReadHalfWord(uint32_t faddr)
{
	return *(vu16*)faddr; 
}
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
static void match_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=match_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

// 21K
//#define VectTabOffset  0x00005400
// 20K
#define ConfigAddr  0x08005000
int QiFreeIO_test1(void)
{
 	QiFreeIO led;
	QiFreeIO led2;
	QiFreeIO key;
	QiFree_ioObj Qi;
	uint8_t down=0;	  
	char buf[1024]={0};
	uint32_t *config = NULL;
	uint32_t i=0;
		
	u8 t;
	u8 len;	
	u16 times=0; 
 
 	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectTabOffset);
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_Configuration();// �����ж����ȼ�����
	uart_init(115200);	 //���ڳ�ʼ��Ϊ9600

	// read flash
	//match_Read(0x08000000, (uint16_t*)buf, 1024/2);
	match_Read(ConfigAddr, (uint16_t*)buf, 1024/2); // 20K
	config = (uint32_t*)(buf+48);
	/*
	printf("\r\n\r\n");
	for(i=0; i<1024; i+=16)
	{
		//printf("config[%02d]: 0X%08X 0X%08X\r\n", i, config[i], config[i+1]);
		printf("%08XH| %03Xh: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\r\n", ConfigAddr+i, i, \
		 buf[i], buf[i+1], buf[i+2], buf[i+3], buf[i+4], buf[i+5], buf[i+6], buf[i+7], \
		 buf[i+8], buf[i+9], buf[i+10], buf[i+11], buf[i+12], buf[i+13], buf[i+14], buf[i+15]);
	}
	*/
	// ��ȡ��Ӧ�� IO������
	newQiFree_ioObj(&Qi, 1, 1); // LED�͵�ƽ��, KEY�͵�ƽ��Ч
	//QiFreeIO_config(&cfg[2], &led);
	QiFreeIO_config(config+2, &led);
	QiFreeIO_config(&config[4], &led2);
	QiFreeIO_config(config, &key);
	printf("KEY : GPIO%c_Pin%d\r\n", config[0], config[1]);
	printf("LED1: GPIO%c_Pin%d\r\n", config[2], config[3]);
	printf("LED2: GPIO%c_Pin%d\r\n", config[4], config[5]);
	Qi.led_init(&led);
	Qi.led_init(&led2);
	//while(1);
	Qi.key_init(&key);
 
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ:\r\n");
			for(t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUF[t];
				while((USART1->SR&0X40)==0);//�ȴ����ͽ���
			}
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				printf("\r\nALIENTEK MiniSTM32������ ����ʵ��\r\n");
				printf("����ԭ��@ALIENTEK\r\n\r\n\r\n");
			}
			if(times%200==0)printf("����������,�Իس�������\r\n");  
			if(times%30==0)
			{
				//LED0=!LED0;//��˸LED,��ʾϵͳ��������.
				Qi.reversal(&led);
				Qi.reversal(&led2);
			}
			delay_ms(10);   
		}
	}	 
}
int QiFreeIO_test2(void)
{
 	QiFreeIO led;
	QiFreeIO led2;
	QiFreeIO key;
	QiFree_ioObj Qi;
	uint8_t down=0;	  
	uint8_t down_old=0;
	char buf[1024]={0};
	uint32_t *config = NULL;
	uint32_t i=0;
		
	u8 t;
	u8 len;	
	u16 times=0; 
 
 	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectTabOffset);
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_Configuration();// �����ж����ȼ�����
	uart_init(115200);	 //���ڳ�ʼ��Ϊ9600

	// read flash
	//match_Read(0x08000000, (uint16_t*)buf, 1024/2);
	match_Read(ConfigAddr, (uint16_t*)buf, 1024/2); // 20K
	config = (uint32_t*)(buf+48);
	/*
	printf("\r\n\r\n");
	for(i=0; i<1024; i+=16)
	{
		//printf("config[%02d]: 0X%08X 0X%08X\r\n", i, config[i], config[i+1]);
		printf("%08XH| %03Xh: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\r\n", ConfigAddr+i, i, \
		 buf[i], buf[i+1], buf[i+2], buf[i+3], buf[i+4], buf[i+5], buf[i+6], buf[i+7], \
		 buf[i+8], buf[i+9], buf[i+10], buf[i+11], buf[i+12], buf[i+13], buf[i+14], buf[i+15]);
	}
	*/
	// ��ȡ��Ӧ�� IO������
	newQiFree_ioObj(&Qi, 1, 1); // LED�͵�ƽ��, KEY�͵�ƽ��Ч
	//QiFreeIO_config(&cfg[2], &led);
	QiFreeIO_config(config+2, &led);
	QiFreeIO_config(&config[4], &led2);
	QiFreeIO_config(config, &key);
	printf("\r\n\r\n");
	printf("KEY : GPIO%c_Pin%d\r\n", config[0], config[1]);
	printf("LED1: GPIO%c_Pin%d\r\n", config[2], config[3]);
	printf("LED2: GPIO%c_Pin%d\r\n", config[4], config[5]);
	printf("__Vectors: %08X\r\n", __Vectors);
	printf("__Vectors_End: %08X\r\n", __Vectors_End);
	printf("__Vectors_Size: %08X\r\n", __Vectors_Size);
	printf("Reset_Handler: %08X\r\n", Reset_Handler);
	printf("&__Vectors: %08X\r\n", &__Vectors);
	printf("&__Vectors_End: %08X\r\n", &__Vectors_End);
	printf("&__Vectors_Size: %08X\r\n", &__Vectors_Size);
	printf("&Reset_Handler: %08X\r\n", &Reset_Handler);
	printf("Board name: %s\r\n", buf+18*4);
	printf("Date: %s\r\n", buf+22*4);
	Qi.led_init(&led);
	Qi.led_init(&led2);
	//while(1);
	Qi.key_init(&key);
 
	while(1)
	{
		times++; 
		if(times>30)
		{
			//LED0=!LED0;//��˸LED,��ʾϵͳ��������.
			Qi.reversal(&led);
			//Qi.reversal(&led2);
			times = 0;
		}
		//�õ���ֵ
		down = Qi.key_down(&key);
		if((!down_old) && down) // down
		{
			Qi.reversal(&led2);
			printf("KEY down!\r\n");
			down_old = down;
		}
		if(down_old && (!down)) // up
		{
			//Qi.reversal(&led2);
			printf("KEY up!\r\n");
			down_old = down;
		}
		delay_ms(10);
	}	 
}

// 21K
//#define VectTabOffset  0x00005400
int main(void)
{ 
	uint32_t VectorsAddr=0x08000000; // default
	VectorsAddr = (uint32_t)(&__Vectors);
	VectorsAddr = VectorsAddr-0x08000000;
 	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectTabOffset); // &__Vectors
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectorsAddr); // &__Vectors
	QiFreeIO_test2();	 
}


