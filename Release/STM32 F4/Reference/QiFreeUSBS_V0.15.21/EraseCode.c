/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : 加密程序.
********************************************************************************
* merafour.blog.163.com
*
*******************************************************************************/
#include "EraseCode.h"
#include "log.h"
//#include "stm32f10x.h"
#include "Flash.h"
#include <stdio.h>
//#include <string.h>
#include "config.h"
#include "log.h"
#include "SRAM.h"

#define UID0    (0x35fd84edU)
#define UID1    (0x4ba34568U)
#define UID2    (0xadc24e89U)
//#define UID     (0xa366db89U)

#ifndef STM32_FLASH_BASE
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
#endif

static void _Write_Flash(uint32_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);

int match_first(void)  
{
	return 0;
}
int _match_first(void)  
{
	return 1;
}

__attribute__((section(".ecode"))) static void erase_match_first(uint8_t* bin)
{
	uint8_t* code=NULL;
	uint32_t addr=0;
	uint32_t i=0;
	// 读出 _match_first函数的指令
	addr = (uint32_t)_match_first&0xFFFFFFFE; // [0]位为1表示thumb指令
	dprintf("addr:%08X\r\n", addr);
	code=(uint8_t*)addr; // _match_first code
	// 覆盖 match_first函数的指令
	addr = (uint32_t)match_first&0xFFFFFFFE;
	dprintf("addr:%08X\r\n", addr);
	addr = (addr-0x20000000); // RAM addr to Offset (about 0x20000000)
	// _match_first函数长度为 4Byte
	for(i=0; i<4; i++)
	{
		bin[addr+i] = code[i]; // cover match_first function
	}
	// 覆盖 cram_entry函数指令
	addr = (uint32_t)cram_entry&0xFFFFFFFE;
	dprintf("addr:%08X\r\n", addr);
	addr = (addr-0x20000000); // RAM addr to Offset (about 0x20000000)
	// _match_first函数长度为 4Byte
	for(i=0; i<4; i++)
	{
		bin[addr+i] = code[i]; // cover cram_entry function
	}
}


// 信息加密,仅供 App使用,自身不对其进行校验
__attribute__((section(".ecode"))) void UserLicense(uint8_t flash[], uint32_t uid[3])
{
	// 使用用户 License进行签名
	uint32_t *sign = (uint32_t *)(flash);
	uint32_t License = sign[40];
	//printf("License:0x%08X\r\n", License);
	sign[40] = uid[0]; // 擦除 License
	sign += 3;
    sign[0] = ((~uid[0])&uid[2]);
    sign[1] = (uid[1]&(~uid[2]));
    sign[2] = uid[0]^uid[1]^License;
	sign[3] = uid[0]^(sign[2]|(~License));
	sign[4] = uid[0]+uid[1]+uid[2];
	sign[5] = (sign[0]+sign[3])|(uid[1]&(~uid[2]));
	sign[6] = (sign[1]+sign[4])|(uid[1]&(~uid[2]));
	sign[7] = (sign[2]+sign[5])|(uid[1]&(~uid[2]));
}

__attribute__((section(".ecode"))) void encoding(uint32_t sign[8], uint32_t uid[3])
{
    sign[0] = (uid[0]&uid[1])|((~uid[0])&uid[2]);
    sign[1] = (uid[0]&uid[2])|(uid[1]&(~uid[2]));
    sign[2] = uid[0]^uid[1]^uid[2];
	sign[3] = uid[1]^(uid[0]|(~uid[2]));
	sign[4] = (sign[0]&sign[1])   |UID0;    // 
	sign[5] = (sign[0]&sign[2])   |UID1;    // 
	sign[6] = ((~sign[0])&sign[2])|UID2;    // 
	sign[7] = (sign[1]&(~sign[2]))|UID0;    // 
}
__attribute__((section(".ecode"))) static void passwd_code(void)
{
	uint8_t* bin=NULL; // buffer
	uint8_t* code=NULL; // Flash
	uint32_t size=0;
	uint32_t mcu_uid[3];
	uint32_t addr=0;
	uint8_t* section_buf = NULL;
	
	uart_init(115200);
	bin=GetRAM(); // buffer, SRAM_LEN_K (80K)
	//section_buf = bin+0x5C00; // ".ecode"
	code=(uint8_t*)0x08000000; // Flash
	// read all code to buffer
	//for(size=0; size<(0x00008000); size++) // 32K
	for(size=0; size<(0x00004000); size++) // 16K
	{
		bin[size] = code[size];
	}
	// 修改第一次匹配
	erase_match_first(bin);
	// read UID
	mcu_uid[0]=*(volatile uint32_t*)(0x1FFF7A10);
	mcu_uid[1]=*(volatile uint32_t*)(0x1FFF7A14);
	mcu_uid[2]=*(volatile uint32_t*)(0x1FFF7A18);
	section_buf = bin; // 256B config
	UserLicense(section_buf, mcu_uid);
	// cover ".ecode"
	//section_buf = bin+0x5C00; // ".ecode"
	section_buf = bin+0x3E00; // ".ecode"
	addr = (mcu_uid[0]+mcu_uid[1]+mcu_uid[2])&0x3FC0;
	//for(size=0; size<(0x00000400); size++) // 32K
	for(size=0; size<(0x00000200); size++) // 32K
	{
		section_buf[size] = bin[size+addr];
	}
	encoding((uint32_t*)section_buf, mcu_uid);
	//printf("_Write_Flash\r\n");
	//_Write_Flash(STM32_FLASH_BASE, bin, 0x00008000);
	_Write_Flash(STM32_FLASH_BASE, bin, 0x00004000);
}

extern void test_entry(void);
__attribute__((section(".ecode"))) static void _Write_Flash(uint32_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	Flash_Write(WriteAddr,(uint32_t*)pBuffer, NumToWrite/4);
	while(1)test_entry();
	//while(1)NVIC_SystemReset(); // 软件复位
}

int cram_entry(void)
{
	passwd_code();
	return 0;
}
