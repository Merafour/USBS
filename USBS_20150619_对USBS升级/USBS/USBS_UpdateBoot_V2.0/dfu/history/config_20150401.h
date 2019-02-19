/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : 工程配置文件.
********************************************************************************
* merafour.blog.163.com
* 原则上从 V0.16.10开始就应该是 V0.18.0版本.
*******************************************************************************/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// 版本号定义,同 Linux内核版本号定义
// version
#define VERSION 0
// patchlevel 
#define PATCHLEVEL 18
// sublevel
#define SUBLEVEL 8

#if 0
enum {
	BOARD_AT9 = 2,
	BOARD_CB88, //BOARD_CB86S, // 128KB
	BOARD_OPEDDEV,
	BOARD_AT10,
	BOARD_CP630, // 256K
	BOARD_NULL
};
#endif

#define BOARD_AT10  1
#define STM32_FLASH_SIZE 512
// product,定义产品
#if defined(BOARD_AT9)
#define PRODUCT  "AT9" // AT9
#elif defined (BOARD_AT10)
#define PRODUCT  "AT10" // AT10
#elif defined (BOARD_CB88)
#define PRODUCT  "CB86S" // cb86S
#undef STM32_FLASH_SIZE
#define STM32_FLASH_SIZE 128
#elif defined (BOARD_CP630)
#define PRODUCT  "CP630" // cp630
#undef STM32_FLASH_SIZE
#define STM32_FLASH_SIZE 256
#elif defined (BOARD_OPEDDEV)
#undef STM32_FLASH_SIZE
#define STM32_FLASH_SIZE 256
#define PRODUCT  "QiFree" // opendev 开发板
#else
#error "Please define BOARD!"
#endif


#ifdef __cplusplus
}
#endif
#endif /* _CONFIG_H_ */
