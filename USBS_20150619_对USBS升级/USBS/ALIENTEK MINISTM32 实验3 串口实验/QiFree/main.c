#include "QiFreeIO.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include <stdio.h>

// 1K
__attribute__ ((section (".boot"))) const char _boot[0x400] __attribute__((used)) = {
       0x12,0x12,0x12,0x12,
	   0x12,0x12,0x12,0x12
};

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
static uint16_t match_ReadHalfWord(uint32_t faddr)
{
	return *(vu16*)faddr; 
}
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
static void match_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=match_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

 int QiFreeIO_test1(void)
 {	
 	QiFreeIO led;
	QiFree_ioObj Qi;
	u8 t=0;	  
	delay_init();	    	 //延时函数初始化	  
	//LED_Init();		  	 	//初始化与LED连接的硬件接口
	KEY_Init();          	//初始化与按键连接的硬件接口
	//LED_Init(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_8);
	//LED_Init(RCC_APB2Periph_GPIOD, GPIOD, GPIO_Pin_2);
//	led.RCC_APB2Periph = RCC_APB2Periph_GPIOA;
//	led.GPIOx = GPIOA;
//	led.Pin = GPIO_Pin_8;

	// 获取相应的 IO函数集
	newQiFree_ioObj(&Qi, 1, 1); // LED低电平亮, KEY低电平有效
	led.RCC_APB2Periph = RCC_APB2Periph_GPIOD;
	led.GPIOx = GPIOD;
	led.Pin = GPIO_Pin_2;
	Qi.led_init(&led);
	//LED_Init(&led);
	//LED0=0;					//点亮LED
	//LEDON(&led, 1);
	while(1)
	{
		t=KEY_Scan(0);		//得到键值
		switch(t)
		{				 
			case KEY0_PRES:
				//LED0=!LED0;
				//LEDON(&led, 0);
				//LEDNOT(&led);
				Qi.led_on(&led, 0);
				break;
			case KEY1_PRES:
				//LED1=!LED1;
				//LEDON(&led, 1);
				//LEDNOT(&led);
				Qi.reversal(&led);
				break;
			case WKUP_PRES:				
				//LED0=!LED0;
				//LED1=!LED1;
				Qi.led_on(&led, 1);
				break;
			default:
				delay_ms(10);	
		} 
	}		 
}
int QiFreeIO_test2(void)
{	
 	QiFreeIO led;
	QiFreeIO key;
	QiFree_ioObj Qi;
	uint8_t down=0;	  
	delay_init();	    	 //延时函数初始化	  

	// 获取相应的 IO函数集
	newQiFree_ioObj(&Qi, 1, 1); // LED低电平亮, KEY低电平有效
	// LED0(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_8);
	// LED1(RCC_APB2Periph_GPIOD, GPIOD, GPIO_Pin_2);
	led.RCC_APB2Periph = RCC_APB2Periph_GPIOA;
	led.GPIOx = GPIOA;
	led.Pin = GPIO_Pin_8;
	Qi.led_init(&led);
	// KEY0(RCC_APB2Periph_GPIOC, GPIOC, GPIO_Pin_5);
	// KEY1(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_15);
	// WK_UP(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_0);
	key.RCC_APB2Periph = RCC_APB2Periph_GPIOA;
	key.GPIOx = GPIOA;
	key.Pin = GPIO_Pin_15;
	Qi.key_init(&key);
	while(1)
	{
		//得到键值
		down = Qi.key_down(&key);
		// 消抖
		delay_ms(10);
		if(down && (Qi.key_down(&key)))
		{
			Qi.reversal(&led);
			// 等待按键松开
			while(Qi.key_down(&key));
		}
		delay_ms(10);
	}		 
}

// LED0(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_8);
// LED1(RCC_APB2Periph_GPIOD, GPIOD, GPIO_Pin_2);
// KEY0(RCC_APB2Periph_GPIOC, GPIOC, GPIO_Pin_5);
// KEY1(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_15);
uint32_t cfg[] = {
	// KEY
	'A', 15,
	//'C', 5,
	// LED
	'A',  8,
	//'D', 2,
	0, 0
};
int QiFreeIO_test3(void)
{	
 	QiFreeIO led;
	QiFreeIO key;
	QiFree_ioObj Qi;
	uint8_t down=0;	  
	delay_init();	    	 //延时函数初始化	  

	// 获取相应的 IO函数集
	newQiFree_ioObj(&Qi, 1, 1); // LED低电平亮, KEY低电平有效
#if 0
	// LED0(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_8);
	// LED1(RCC_APB2Periph_GPIOD, GPIOD, GPIO_Pin_2);
	// KEY0(RCC_APB2Periph_GPIOC, GPIOC, GPIO_Pin_5);
	// KEY1(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_15);
	led.RCC_APB2Periph = RCC_APB2Periph_GPIOA;
	led.GPIOx = GPIOA;
	led.Pin = GPIO_Pin_8;
	key.RCC_APB2Periph = RCC_APB2Periph_GPIOA;
	key.GPIOx = GPIOA;
	key.Pin = GPIO_Pin_15;
#else
	QiFreeIO_config(&cfg[2], &led);
	QiFreeIO_config(cfg, &key);
#endif
	Qi.led_init(&led);
	//while(1);
	Qi.key_init(&key);
	while(1)
	{
		//得到键值
		down = Qi.key_down(&key);
		// 消抖
		delay_ms(10);
		if(down && (Qi.key_down(&key)))
		{
			Qi.reversal(&led);
			// 等待按键松开
			while(Qi.key_down(&key));
		}
		delay_ms(10);
	}		 
}
int QiFreeIO_test4(void)
{	
 	QiFreeIO led;
	QiFreeIO led2;
	QiFreeIO key;
	QiFree_ioObj Qi;
	uint8_t down=0;	  
	delay_init();	    	 //延时函数初始化	  

	// 获取相应的 IO函数集
	newQiFree_ioObj(&Qi, 1, 1); // LED低电平亮, KEY低电平有效

	//QiFreeIO_config(&cfg[2], &led);
	QiFreeIO_config(cfg+2, &led);
	QiFreeIO_config(&cfg[4], &led2);
	QiFreeIO_config(cfg, &key);
	Qi.led_init(&led);
	Qi.led_init(&led2);
	//while(1);
	Qi.key_init(&key);
	while(1)
	{
		//得到键值
		down = Qi.key_down(&key);
		// 消抖
		delay_ms(10);
		if(down && (Qi.key_down(&key)))
		{
			Qi.reversal(&led);
			Qi.reversal(&led2);
			// 等待按键松开
			while(Qi.key_down(&key));
		}
		delay_ms(10);
	}		 
}

int QiFreeIO_test5(void)
{	
 	QiFreeIO led;
	QiFreeIO led2;
	QiFreeIO key;
	QiFree_ioObj Qi;
	uint8_t down=0;	  
	char buf[1024]={0};
	uint32_t *config = NULL;
	delay_init();	    	 //延时函数初始化	  

	// read flash
	match_Read(0x08000000, (uint16_t*)buf, 1024/2);
	config = (uint32_t*)(buf+34);

	// 获取相应的 IO函数集
	newQiFree_ioObj(&Qi, 1, 1); // LED低电平亮, KEY低电平有效

	//QiFreeIO_config(&cfg[2], &led);
	QiFreeIO_config(config+2, &led);
	QiFreeIO_config(&config[4], &led2);
	QiFreeIO_config(config, &key);
	Qi.led_init(&led);
	Qi.led_init(&led2);
	//while(1);
	Qi.key_init(&key);
	while(1)
	{
		//得到键值
		down = Qi.key_down(&key);
		// 消抖
		delay_ms(10);
		if(down && (Qi.key_down(&key)))
		{
			Qi.reversal(&led);
			Qi.reversal(&led2);
			// 等待按键松开
			while(Qi.key_down(&key));
		}
		delay_ms(10);
	}		 
}

// 40K
#define VectTabOffset  0x00005000
int main(void)
{	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectTabOffset);
 	while(1)QiFreeIO_test5();		 
}

