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

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */


/** @defgroup APP_MSC 
  * @brief Mass storage application module
  * @{
  */ 

/** @defgroup APP_MSC_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup APP_MSC_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup APP_MSC_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup APP_MSC_Private_Variables
  * @{
  */ 

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

__ALIGN_BEGIN USB_OTG_CORE_HANDLE     USB_OTG_dev __ALIGN_END ;

/**
  * @}
  */ 


/** @defgroup APP_MSC_Private_FunctionPrototypes
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup APP_MSC_Private_Functions
  * @{
  */ 
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
/**
  * @brief  Program entry point
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t down=0;
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32fxxx_xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32fxxx.c file
  */ 

  //uart_init(115200);
  //printf("QiFreeUSBS version %s.%s.%s\r\n", VERSION, PATCHLEVEL, SUBLEVEL);
  Board_LEDInit(LED1);
  Board_LEDInit(LED2);
  Board_PBInit();

	down = Board_PBGetState();
	//printf("down:%d\r\n", down);
  if (0x00 == down)
  { /* Test if user code is programmed starting from address 0x800C000 */
  	//printf("Addr:0x%08X ,0x%08X\r\n", APP_DEFAULT_ADD, ((*(__IO uint32_t*)APP_DEFAULT_ADD) & 0x2FFE0000 ));
	//printf("Addr:0x%08X ,0x%08X\r\n", APP_DEFAULT_ADD, (*(__IO uint32_t*)APP_DEFAULT_ADD ));
    if (((*(__IO uint32_t*)APP_DEFAULT_ADD) & 0x2FFE0000 ) == 0x20000000)
    { /* Jump to user application */

      JumpAddress = *(__IO uint32_t*) (APP_DEFAULT_ADD + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) APP_DEFAULT_ADD);
      Jump_To_Application();
    }
  } /* Otherwise enters DFU mode to allow user to program his application */
  uart_init(115200);
  printf("QiFreeUSBS version %s.%s.%s\r\n", VERSION, PATCHLEVEL, SUBLEVEL);
  USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS 
            USB_OTG_HS_CORE_ID,
#else            
            USB_OTG_FS_CORE_ID,
#endif            
            &USR_desc,
            &USBD_MSC_cb, 
            &USR_cb);
	
	while (1)
	{
		Board_LEDToggle(LED1);
		Board_LEDToggle(LED2);
		down = Board_PBGetState();
		//down = (~down)&0x1;
		//printf("waitting ...\r\n");
		//printf("KEY:%d\r\n", down);
		delay_ms(100);   
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
