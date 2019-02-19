#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"


//#define VectTabOffset  0x00005000	
//#define VectTabOffset  0x0000A000
#define VectTabOffset  0x00007000
int main(void)
{	 
 	NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectTabOffset); // &__Vectors
	delay_init();	    	 
	NVIC_Configuration();
	uart_init(115200);	 
	while(1)
	{
		printf("QiFree USBS test...\r\n");  
		delay_ms(500);   
	}	 
}


