/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 6/12/2015
* Description        : Board.
********************************************************************************
* merafour.blog.163.com
* 深圳市乐迪电子有限公司:www.radiolink.com.cn
*******************************************************************************/
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARD_H_
#define __BOARD_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

typedef enum 
{
  LED1 = 0,
  LED2 = 1,
} Led_TypeDef;

#define LEDn                             2

#define LED1_PIN                         GPIO_Pin_9
#define LED1_GPIO_PORT                   GPIOF
#define LED1_GPIO_CLK                    RCC_AHB1Periph_GPIOF  
  
#define LED2_PIN                         GPIO_Pin_10
#define LED2_GPIO_PORT                   GPIOF
#define LED2_GPIO_CLK                    RCC_AHB1Periph_GPIOF  

void Board_LEDInit(Led_TypeDef Led);
void Board_LEDOn(Led_TypeDef Led);
void Board_LEDOff(Led_TypeDef Led);
void Board_LEDToggle(Led_TypeDef Led);
  
#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H_ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
