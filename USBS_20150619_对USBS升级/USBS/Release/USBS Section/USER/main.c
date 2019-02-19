#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

__attribute__((section("zoo"))) static void Loop(void)
{
	while(1)
	{
		printf("Section test ...\r\n");
		delay_ms(500);   
	}
}
typedef void (*func_t)(void);
//static const func_t func __attribute__((section("zoo"))) = Loop;
static func_t func __attribute__((section("zoo"))) = Loop;
int main(void)
{	
	const func_t* loop=NULL;
	delay_init();	    	 
	NVIC_Configuration();
	uart_init(115200);
	loop = &func;
	//printf("Start ...0x%08X\r\n", loop);
	printf("Start ...\r\n");
	delay_ms(1000);
	(*loop)();	 
	while(1)
	{
		printf("ERROR!\r\n");
		delay_ms(300);   
	}	 
}


