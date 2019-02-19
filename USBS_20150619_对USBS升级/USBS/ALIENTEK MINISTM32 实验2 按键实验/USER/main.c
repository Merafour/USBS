#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
//ALIENTEK Mini STM32开发板范例代码2
//按键输入实验		   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
 int main(void)
 {	
 	QiFreeIO led;
	u8 t=0;	  
	delay_init();	    	 //延时函数初始化	  
	//LED_Init();		  	 	//初始化与LED连接的硬件接口
	KEY_Init();          	//初始化与按键连接的硬件接口
	//LED_Init(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_8);
	//LED_Init(RCC_APB2Periph_GPIOD, GPIOD, GPIO_Pin_2);
//	led.RCC_APB2Periph = RCC_APB2Periph_GPIOA;
//	led.GPIOx = GPIOA;
//	led.Pin = GPIO_Pin_8;
	led.RCC_APB2Periph = RCC_APB2Periph_GPIOD;
	led.GPIOx = GPIOD;
	led.Pin = GPIO_Pin_2;
	LED_Init(&led);
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
				LEDNOT(&led);
				break;
			case KEY1_PRES:
				//LED1=!LED1;
				LEDON(&led, 1);
				//LEDNOT(&led);
				break;
//			case WKUP_PRES:				
//				LED0=!LED0;
//				LED1=!LED1;
//				break;
			default:
				delay_ms(10);	
		} 
	}		 
}
