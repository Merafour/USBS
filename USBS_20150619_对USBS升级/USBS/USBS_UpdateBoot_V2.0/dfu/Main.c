/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : U盘固件升级主程序.
********************************************************************************
* merafour.blog.163.com
*
*******************************************************************************/

#include "config.h"

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "log.h"
//#include "EraseCode.h"
//#include "match.h"
#include "stmflash.h"
#include "QiFreeIO.h"
#include "stm32f10x_rcc.h"
#include "flash_if.h"

extern uint32_t __Vectors;
extern uint32_t __Vectors_End;
extern uint32_t __Vectors_Size;
extern uint32_t Reset_Handler;

const char Description[]="\
* Author   : merafour.blog.163.com\r\n\
//* Version  : V1.0.0\r\n\
* Date     : 2/26/2015\r\n";

/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunction)(void);
uint8_t DeviceState;
uint8_t DeviceStatus[6];
pFunction Jump_To_Application;
uint32_t JumpAddress;

static QiFreeIO led1={0,0,0};
static QiFreeIO led2={0,0,0};
static QiFreeIO key={0,0,0};
static QiFree_ioObj Qi;
static uint32_t LinkAddress = 0x08005000; // 链接地址,default 0K
static uint32_t ApplicationAddress = 0x0800A000; // app地址,(LinkAddress+20K)default 40K
uint8_t code[1024]={0}; // 1K
uint8_t passwd[1024]={0}; // 1K

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
static uint16_t match_ReadHalfWord(uint32_t faddr)
{
	return *(vu16*)faddr; 
}
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
static void match_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=match_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

char *Enable[2] = {"DisEnable", "Enable"};
/*
 * Flash map
 * bin       |-----------------| start
 * uint32_t 0| __initial_sp    | 000H
 *          1| Reset_Handler   | 004H
 *          2| Flash KB        | 008H  | swd| erase| flashH| flashL|
 *           |-----------------|
 *          3|                 | 00CH
 *        ...| mtext           | ...H
 *         10|                 | {8*4}B
 *           |-----------------|
 *         11| empty           |
 *           |-----------------|
 *         12| KEY_GPIO        | 030H
 *         13| KEY_Pin         |       | Low| Pin|
 *         14| LED1_GPIO       |
 *         15| LED1_Pin        |       | Low| Pin|
 *         16| LED2_GPIO       |
 *         17| LED2_Pin        |       | Low| Pin|
 *           |-----------------|
 *         18|                 | 48H
 */
static void QiFreeConfig(void)
{
	//char buf[BUF_LEN]={0};
	uint32_t *map=NULL;
	int led_low=0;
	int key_low=0;
	uint32_t swd=1;
	// 读取配置
	//match_Read(LinkAddress, (uint16_t*)buf, 1024/2); // 20K
	//buf = (uint8_t *)LinkAddress;
	//map = (uint32_t*)code;
	map = (uint32_t*)LinkAddress;

	// 关闭 jtag
	// | swd| erase| flashH| flashL|
	swd = (map[2]>>24)&0x1;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 关闭jtag需要使能该时钟
	//if(((*(uint32_t*)(buf+8))>>24)&0x1) GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	if(swd) GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	else GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); // 关闭jtag，关闭SWD，只能使用串口烧录

	// IO default
	//QiFreeIO_Default((uint32_t*)(buf+32*4));
	QiFreeIO_Default(map+32);

	// IO Select
	//config = (uint32_t*)(buf+48);
	// 获取相应的 IO函数集
	key_low = (map[13]>>16)&0x1;
	led_low = (map[15]>>16)&0x1;
	//newQiFree_ioObj(&Qi, 1, 1); // LED低电平亮, KEY低电平有效
	newQiFree_ioObj(&Qi, led_low, key_low); // LED低电平亮, KEY低电平有效
	//newQiFree_ioObj(&Qi, (config[3]>>16)&0x1, (config[1]>>16)&0x1); // LED低电平亮, KEY低电平有效
	//QiFreeIO_config(&cfg[2], &led);
	QiFreeIO_config(map+12, &key);
	QiFreeIO_config(map+14, &led1);
	QiFreeIO_config(map+16, &led2);

	Qi.led_init(&led1);
	Qi.led_init(&led2);
	Qi.key_init(&key);
}

static void QiFreeUartInit(void)
{
	uint32_t *map = NULL;
	int led_low=0;
	int key_low=0;
	uint32_t KB=128;
	uint32_t swd=1;
	//uint32_t erase=1;
	int i=0;
	uint32_t *gpio_value=NULL;

	/************************ 配置串口 *****************************/
	uart_init(115200);	 	//串口初始化为115200 
	//printf("\r\n\r\nBootloader version 0.16.2\r\n"); 
	//printf("\r\n\r\n%s QiFreeUSBS update version %d.%d.%d\r\n", PRODUCT, VERSION, PATCHLEVEL, SUBLEVEL);
	//printf("%s", Description);
	//printf("Date: %s--%s\r\n", __TIME__, __DATE__);
	//printf("F_SIZE:%04X\r\n", (*(vu32*)(0x1ffff7e0))&0xFFFF);
	//match_Read(LinkAddress, (uint16_t*)buf, 1024/2); // 20K
	map = (uint32_t *)LinkAddress;
	//config = (uint32_t*)(buf+48);
	key_low = (map[13]>>16)&0x1;
	led_low = (map[15]>>16)&0x1;
	//printf("\r\n%s QiFreeUSBS version %d.%d.%d\r\n", buf+18*4, VERSION, PATCHLEVEL, SUBLEVEL);
	//printf("\r\n%s QiFreeUSBS version %s.%s.%s\r\n", (char*)(map+18), VERSION, PATCHLEVEL, SUBLEVEL);
	printf("QiFreeUSBS version %s.%s.%s\r\n", VERSION, PATCHLEVEL, SUBLEVEL);
	//printf("%s", buf+0xA0); // 40*4
	printf("%s", Description);
	printf("Date: %s--%s\r\n", __TIME__, __DATE__);
	//printf("Date: %s--%s\r\n", buf+0xE0, buf+0xD0); // VERSION: 0x40, 0x30
	//printf("%s", buf+0xD0); // VERSION: 0x30
	//KB = (*(vu32*)(0x1ffff7e0))&0xFFFF;
	// Flash Init
	KB = (*(vu32*)(0x1ffff7e0))&0xFFFF;
	FlashInit(KB); // Flash初始化
	//printf("F_SIZE:%04X\r\n", (*(vu32*)(0x1ffff7e0))&0xFFFF);
	printf("F_SIZE:%04X\r\n", KB);
	//printf("Flash Address: %08X\r\n", LinkAddress);
	//printf("ApplicationAddress: %08X\r\n", ApplicationAddress);
	printf("KEY : P%c_%d\r\n", map[12], map[13]&0xFFFF);
	printf("LED1: P%c_%d\r\n", map[14], map[15]&0xFFFF);
	printf("LED2: P%c_%d\r\n", map[16], map[17]&0xFFFF);
	printf("led_low:%d\r\n", led_low);
	printf("key_low:%d\r\n", key_low);
	// | swd| erase| flashH| flashL|
	//KB = *(uint32_t*)(buf+8);
	KB = map[2];
	//erase = (KB>>16)&0x1;
	swd = (KB>>24)&0x1;
	//printf("Flash:%08X\r\n", KB);
	//printf("Flash:%dKB\r\n", KB&0x0000FFFF);
	//printf("Flash:%08X\r\n", KB);
	//printf("erase:%s\r\n", erase?"Enable":"DisEnable");
	//printf("swd:%s\r\n", swd?"Enable":"DisEnable");
	//printf("erase:%s\r\n", Enable[erase&0x1]);
	//printf("swd:%s\r\n", Enable[swd&0x1]);
	printf("swd:%s\r\n", Enable[swd]);
	//gpio_value = (uint32_t*)(buf+32*4);
	gpio_value = map+32;
	for(i=0; i<7; i++) // {A,B,C,D,E,F,G} 7
	{
		//printf("GPIO%c:%04X %s\r\n", (char)('A'+led), *gpio_value&0xFFFF, ((*gpio_value>>16)&0x1)?"Enable":"DisEnable");
		printf("GPIO%c:%04X Enable:%04X\r\n", (char)('A'+i), *gpio_value&0xFFFF, (*gpio_value>>16)&0xFFFF);
		gpio_value++;
	}
	//printf("Board:%s\r\n", buf+18*4);
	//printf("Date:%s\r\n", buf+22*4);
	//printf("Board:%s\r\n", (char*)(map+18));
	printf("Date:%s\r\n", (char*)(map+22));
	//printf("0x08004C20:%s", (char*)0x08004C20);
	//printf("0x08004C50:%s", (char*)0x08004C40);
	printf("0x080000F0:%s", (char*)0x080000F0);
	//printf("0x08000120:%s", (char*)0x08000120);
	/*
	printf("__Vectors: %08X\r\n", __Vectors);
	printf("__Vectors_End: %08X\r\n", __Vectors_End);
	printf("__Vectors_Size: %08X\r\n", __Vectors_Size);
	printf("Reset_Handler: %08X\r\n", Reset_Handler);
	printf("&__Vectors: %08X\r\n", &__Vectors);
	printf("&__Vectors_End: %08X\r\n", &__Vectors_End);
	printf("&__Vectors_Size: %08X\r\n", &__Vectors_Size);
	printf("&Reset_Handler: %08X\r\n", &Reset_Handler);
	*/
}

void delay_ms(int ms)
{
	int ms1=0;
	while(ms>0)
	{
		ms--;
		for(ms1=0; ms1<12000; ms1++);
	}
	//delay_us(12);
}

void boot_reset(void)
{ /* Test if user code is programmed starting from address 0x8003000 */
	//uint8_t code[1024]={0}; // 1K
	//uint8_t passwd[32]={0}; // 1K
	uint32_t addr=0;
	dprintf("boot_reset(0x%08X|0x%08X)...\r\n", ApplicationAddress, (*(__IO uint32_t*)ApplicationAddress));
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
	{ /* Jump to user application */
		//usb_port_set(0); 	//USB先断开
		//delay_ms(600);
		// 读取加密信息
		//STMFLASH_Read(0X08004C00, (uint16_t*)passwd, 32/2);
//		for(addr=0; addr<32; addr+=4) // 加密已经存在,最后 1K数据不擦除
//		{
//			printf("passwd: 0X%02X%02X%02X%02X\r\n", passwd[addr+3], passwd[addr+2], passwd[addr+1], passwd[addr]);
//		}
		FLASH_Unlock();
		// 擦除代码
		for(addr=0; addr<(20*1024); addr+=1024)
		{
			printf("erase addr: 0x%08X\r\n", addr);
			FLASH_ErasePage(0X08000000+addr);
			delay_ms(10);
		}
		//FLASH_Lock();
		// 拷贝数据
		for(addr=0; addr<(20*1024); addr+=1024)
		{
			printf("write addr: 0x%08X\r\n", addr);
			STMFLASH_Read(ApplicationAddress+addr, (uint16_t*)code, 1024/2);
			delay_ms(10);
			//STMFLASH_Write(0X0800F000+addr,(u16*)code, 1024/2);
			//STMFLASH_Write(0X08000000+addr,(u16*)code, 1024/2);
			//FLASH_Unlock();
			FLASH_If_Write(0X08000000+addr, code, 1024);
			//FLASH_Lock();
			delay_ms(10);
		}
		//  write passwd
		//FLASH_If_Write(0X08004C00, passwd, 1024);
		//printf("boot_reset(0x%08X|0x%08X)...\r\n", 0X0800F000, (*(__IO uint32_t*)0X0800F000));
		printf("erase addr: 0x%08X\r\n", ApplicationAddress);
		//FLASH_Unlock();
		FLASH_ErasePage(ApplicationAddress);
		//FLASH_ErasePage(0X08000000);
		FLASH_Lock();
		printf("SystemReset!\r\n");
		STMFLASH_Read(0X08004C00, (uint16_t*)code, 1024/2);
//		for(addr=0; addr<32; addr+=4) // 加密已经存在,最后 1K数据不擦除
//		{
//			printf("code: 0X%02X%02X%02X%02X\r\n", code[addr+3], code[addr+2], code[addr+1], code[addr]);
//		}
		delay_ms(40);
		NVIC_SystemReset(); // 软件复位
	}
} 

void boot_reset_save(void)
{ /* Test if user code is programmed starting from address 0x8003000 */
	//uint8_t code[1024]={0}; // 1K
	//uint8_t passwd[32]={0}; // 1K
	uint32_t addr=0;
	dprintf("boot_reset(0x%08X|0x%08X)...\r\n", ApplicationAddress, (*(__IO uint32_t*)ApplicationAddress));
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
	{ /* Jump to user application */
		//usb_port_set(0); 	//USB先断开
		//delay_ms(600);
		// 读取加密信息
		STMFLASH_Read(0X08004C00, (uint16_t*)passwd, 32/2);
//		for(addr=0; addr<32; addr+=4) // 加密已经存在,最后 1K数据不擦除
//		{
//			printf("passwd: 0X%02X%02X%02X%02X\r\n", passwd[addr+3], passwd[addr+2], passwd[addr+1], passwd[addr]);
//		}
		FLASH_Unlock();
		// 擦除代码
		for(addr=0; addr<(20*1024); addr+=1024)
		{
			dprintf("erase addr: 0x%08X\r\n", addr);
			FLASH_ErasePage(0X08000000+addr);
			delay_ms(10);
		}
		//FLASH_Lock();
		// 拷贝数据
		for(addr=0; addr<(19*1024); addr+=1024)
		{
			dprintf("write addr: 0x%08X\r\n", addr);
			STMFLASH_Read(ApplicationAddress+addr, (uint16_t*)code, 1024/2);
			delay_ms(10);
			//STMFLASH_Write(0X0800F000+addr,(u16*)code, 1024/2);
			//STMFLASH_Write(0X08000000+addr,(u16*)code, 1024/2);
			//FLASH_Unlock();
			FLASH_If_Write(0X08000000+addr, code, 1024);
			//FLASH_Lock();
			delay_ms(10);
		}
		//  write passwd
		FLASH_If_Write(0X08004C00, passwd, 1024);
		//printf("boot_reset(0x%08X|0x%08X)...\r\n", 0X0800F000, (*(__IO uint32_t*)0X0800F000));
		dprintf("erase addr: 0x%08X\r\n", ApplicationAddress);
		//FLASH_Unlock();
		FLASH_ErasePage(ApplicationAddress);
		//FLASH_ErasePage(0X08000000);
		FLASH_Lock();
		printf("SystemReset!\r\n");
		STMFLASH_Read(0X08004C00, (uint16_t*)code, 1024/2);
//		for(addr=0; addr<32; addr+=4) // 加密已经存在,最后 1K数据不擦除
//		{
//			printf("code: 0X%02X%02X%02X%02X\r\n", code[addr+3], code[addr+2], code[addr+1], code[addr]);
//		}
		delay_ms(40);
		NVIC_SystemReset(); // 软件复位
	}
} 

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
extern const char Description[];
__inline static void _inline_delay_ms(int ms)
{
	int ms1=0;
	while(ms>0)
	{
		ms--;
		for(ms1=0; ms1<12000; ms1++);
	}
}
void test_entry(void)
{
	unsigned int led=0;
	unsigned int clock=0;
	// 连续检测
	clock = 0;
	led = clock+40; // 0.04s
	//for(clock=0; ; clock += 10)
	lprintf("waiting ...\r\n");
	for(clock=0; clock<2000; clock += 10)
	//while(1)
	{
		delay_ms(10);
		//boot_app();
		if((clock+30) <= led)
		{
			Qi.led_on(&led1, 0);
			Qi.led_on(&led2, 0);
		}
		else if(clock <= led)
		{
			Qi.led_on(&led1, 1);
			Qi.led_on(&led2, 1);
		}
		else
		{
			led = clock+500; // 0.06s
		}
	}
	Qi.led_on(&led1, 1);
	Qi.led_on(&led2, 1);
	delay_ms(2000);
}
// 12K
//#define VectTabOffset  0x00003000
// 20K
//#define VectTabOffset  0x00005000
int main(void)
{
	unsigned int led=0;
	unsigned int clock=0;
	uint8_t *buf=NULL;
	//uint32_t i=0;
	//uint32_t erase=1;
	uint32_t __initial_sp=0;
    uint32_t Reset_Handler=0;
	uint32_t *bin=NULL;
	
	// 设置中断向量表
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectTabOffset);
	uint32_t VectorsAddr=0x08005400; // default
	VectorsAddr = (uint32_t)(&__Vectors);
	VectorsAddr = VectorsAddr-0x08000000;
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectorsAddr); // &__Vectors
	//LinkAddress = 0x08000000+VectorsAddr-0x0400; // 1K
	LinkAddress = 0x08000000+VectorsAddr-0x0100; // 256B
	//LinkAddress = 0x08000000+VectorsAddr-0x0100-0x2000; // 256B, 8K
	//ApplicationAddress = LinkAddress+20*1024; // 20K
	ApplicationAddress = LinkAddress+12*1024; // 12K
	_inline_delay_ms(5);
	/************************ 配置串口 *****************************/
	QiFreeConfig();
	QiFreeUartInit();

	//printf("\r\n\r\n%s QiFreeUSBS update program version %d.%d.%d\r\n", buf+18*4, VERSION, PATCHLEVEL, SUBLEVEL);
	printf("QiFreeUSBS update program version %s.%s.%s\r\n", VERSION, PATCHLEVEL, SUBLEVEL);
	printf("%s", Description);
	printf("Date: %s--%s\r\n", __TIME__, __DATE__);
	//printf("F_SIZE:%04X\r\n", (*(vu32*)(0x1ffff7e0))&0xFFFF);
	//printf("Address: %08X\r\n", LinkAddress);
	//printf("Address: %08X\r\n", LinkAddress);
	//printf("ApplicationAddress: %08X\r\n", ApplicationAddress);
	//printf("Flash:%08X\r\n", KB);
	delay_ms(2000);
	for(clock=0; clock<2000; clock+=10)
	{
		delay_ms(10);
		if((clock+30) <= led)
		{
			Qi.led_on(&led1, 0);
			Qi.led_on(&led2, 0);
		}
		else if(clock <= led)
		{
			Qi.led_on(&led1, 1);
			Qi.led_on(&led2, 1);
		}
		else
		{
			led = clock+70; // 0.02s
		}
	}
	
	//code_entry(); 
	lprintf("update QiFreeUSBS now!\r\n");
	Qi.led_on(&led1, 0);
	Qi.led_on(&led2, 0);
	//boot_reset();
	// 配置信息
	STMFLASH_Read(ApplicationAddress, (uint16_t*)code, 1024/2);
    __initial_sp = *(uint32_t *)(code+256); // __initial_sp
    Reset_Handler = *(uint32_t *)(code+260); // Reset_Handler
    match_Read(LinkAddress, (uint16_t*)buf, (256-16)/2); // 读取配置信息
    bin = (uint32_t *)(buf);
    bin[0] = __initial_sp; // __initial_sp
    bin[1] = Reset_Handler; // Reset_Handler
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
	{
		STMFLASH_Write_zone(ApplicationAddress,(u16*)buf, (256-16)/2);
	}
#if 1
		// 20K,删除原来的加密信息
		boot_reset(); 
#else
		// 19K,保留加密信息
		boot_reset_save();
#endif
	while(1)test_entry();
}

#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: lprintf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
