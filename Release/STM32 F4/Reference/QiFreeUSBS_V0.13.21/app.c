/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 6/12/2015
* Description        : main.
********************************************************************************
* merafour.blog.163.com
* 深圳市乐迪电子有限公司:www.radiolink.com.cn
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"
#include "log.h"
#include "config.h"
#include "Board.h" 
#include "QiFreeIO.h"

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

__ALIGN_BEGIN USB_OTG_CORE_HANDLE     USB_OTG_dev __ALIGN_END ;

#if 1
static const uint32_t LinkAddress = 0x08000000; // 链接地址,default 0K
#else
static const uint32_t LinkAddress = (uint32_t)ConfigExamples; // 链接地址,default 0K
#endif
extern uint32_t __Vectors;

void delay_ms(int ms)
{
	int ms1=0;
	while(ms>0)
	{
		ms--;
		for(ms1=0; ms1<12000; ms1++);
		for(ms1=0; ms1<12000; ms1++);
		for(ms1=0; ms1<12000; ms1++);
		for(ms1=0; ms1<6000; ms1++);
	}
}

typedef  void (*pFunction)(void);
static pFunction Jump_To_Application;
static uint32_t JumpAddress;
#define APP_DEFAULT_ADD                 0x08008000 /* The first 2 sectores (32 KB) are reserved for DFU code */
static void boot_app(void)
{
    if (((*(__IO uint32_t*)APP_DEFAULT_ADD) & 0x2FFE0000 ) == 0x20000000)
    { /* Jump to user application */
		JumpAddress = *(__IO uint32_t*) (APP_DEFAULT_ADD + 4);
		Jump_To_Application = (pFunction) JumpAddress;
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) APP_DEFAULT_ADD);
		Jump_To_Application();
		while(1)NVIC_SystemReset();	// Program Reset
    }
}
void boot_reset(void)
{
	/* Test if user code is programmed starting from address 0x8003000 */
	if (((*(__IO uint32_t*)APP_DEFAULT_ADD) & 0x2FFE0000 ) == 0x20000000)
	{
		NVIC_SystemReset(); // Program Reset	
	}
} 
static void code_entry(void)
{
	uint8_t down=0;
	unsigned int led=0;
	unsigned int count=0;
	unsigned int clock=0;

	/************************ Boot App *****************************/
	clock = 0;
	count = clock+200; // 0.2s
	led = clock+50; // 0.05s

	for(clock=0; clock<400; clock += 10)
	{
		delay_ms(10);
		//down = Board_PBGetState();
		down = Qi.key_down(&key);
		//down = (~down)&0x1;
		if(down) // down, update
		{
			count = clock+200; // 0.2s
		}
		else if(clock>=count) // Boot app
		{
			boot_app();
			//break;
			count = clock+200; // 0.2s
		}
		if(clock > led)
		{
			led = clock+50; // 0.05s
//			Board_LEDToggle(LED1);
//			Board_LEDToggle(LED2);
			Qi.reversal(&led1);
			Qi.reversal(&led2);

		}
	}
}
static int DownLoadFlag=0;
void DownLoadStatus(void)
{
	DownLoadFlag = 1;
//	Board_LEDOn(LED1);
//	Board_LEDOn(LED2);
	Qi.led_on(&led1, 1);
	Qi.led_on(&led2, 1);
}
// mount
static void mount_storage(void)
{
	uint8_t down=0;
	unsigned int led=0;
	unsigned int count=0;
	unsigned int clock=0;

  USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS 
            USB_OTG_HS_CORE_ID,
#else            
            USB_OTG_FS_CORE_ID,
#endif            
            &USR_desc,
            &USBD_MSC_cb, 
            &USR_cb);
	printf("main ...\r\n");
	clock = 0;
	count = clock+200; // 0.2s
	led = clock+50; // 0.05s
	while(1)
	{
		while(DownLoadFlag)
		{
			DownLoadFlag = 0;
//			Board_LEDOn(LED1);
//			Board_LEDOn(LED2);
			Qi.led_on(&led1, 1);
			Qi.led_on(&led2, 1);
			delay_ms(1000);
		}
		delay_ms(10);
		clock += 10; // update clock
		//down = Board_PBGetState();
		down = Qi.key_down(&key);
		if(down) // down, update
		{
			count = clock+1000; // 1s
		}
		else if(clock>=count) // Boot app
		{
			printf("boot app\r\n");
			boot_reset();
			count = clock+1000; // 1s
		}
		if(clock > led)
		{
			led = clock+300; // 0.06s
//			Board_LEDToggle(LED1);
//			Board_LEDToggle(LED2);
			Qi.reversal(&led1);
			Qi.reversal(&led2);

		}
	}
}
/**
  * @brief  Program entry point
  * @param  None
  * @retval None
  */
int main(void)
{
	//uint8_t down=0;
	uint32_t VectorsAddr=0x08005400; // default
	VectorsAddr = (uint32_t)(&__Vectors);
	//VectorsAddr = VectorsAddr-0x08000000;
	VectorsAddr = VectorsAddr-0x20000000;
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectorsAddr); // &__Vectors
	NVIC_SetVectorTable(NVIC_VectTab_RAM, VectorsAddr); // &__Vectors

	QiFreeConfigIO((uint32_t*)LinkAddress);
	code_entry();
	uart_init(115200);
	QiFreeConfigLog((uint32_t*)LinkAddress);
	//printf("QiFreeUSBS version %s.%s.%s\r\n", VERSION, PATCHLEVEL, SUBLEVEL);
	mount_storage();
//  USBD_Init(&USB_OTG_dev,
//#ifdef USE_USB_OTG_HS 
//            USB_OTG_HS_CORE_ID,
//#else            
//            USB_OTG_FS_CORE_ID,
//#endif            
//            &USR_desc,
//            &USBD_MSC_cb, 
//            &USR_cb);
	
	while (1)
	{
//		Board_LEDToggle(LED1);
//		Board_LEDToggle(LED2);
		Qi.reversal(&led1);
		Qi.reversal(&led2);
		//down = Board_PBGetState();
		//down = (~down)&0x1;
		//printf("waitting ...\r\n");
		//printf("KEY:%d\r\n", down);
		delay_ms(500);   
	}
} 

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
