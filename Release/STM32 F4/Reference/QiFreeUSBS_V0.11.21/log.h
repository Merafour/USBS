/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : 调试信息.
********************************************************************************
* merafour.blog.163.com
* 深圳市乐迪电子有限公司:www.radiolink.com.cn
*******************************************************************************/

#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

#define _DPRINTF
#undef _DPRINTF

#ifdef _DPRINTF
#define dprintf(fmt, ...) printf("@%s +%d %s " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define lprintf(fmt, ...) printf("@%s +%d %s " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define dprintf(...)
#define lprintf(fmt, ...) printf("[%s @%d] " fmt, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

extern void uart_init(uint32_t bound);

#ifdef __cplusplus
}
#endif
#endif /* _DEBUG_H_ */
