#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
//ALIENTEK Mini STM32�����巶������2
//��������ʵ��		   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
 int main(void)
 {	
 	QiFreeIO led;
	u8 t=0;	  
	delay_init();	    	 //��ʱ������ʼ��	  
	//LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
	//LED_Init(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_8);
	//LED_Init(RCC_APB2Periph_GPIOD, GPIOD, GPIO_Pin_2);
//	led.RCC_APB2Periph = RCC_APB2Periph_GPIOA;
//	led.GPIOx = GPIOA;
//	led.Pin = GPIO_Pin_8;
	led.RCC_APB2Periph = RCC_APB2Periph_GPIOD;
	led.GPIOx = GPIOD;
	led.Pin = GPIO_Pin_2;
	LED_Init(&led);
	//LED0=0;					//����LED
	//LEDON(&led, 1);
	while(1)
	{
		t=KEY_Scan(0);		//�õ���ֵ
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
