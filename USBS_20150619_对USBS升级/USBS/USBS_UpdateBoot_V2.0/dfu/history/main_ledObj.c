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
//#include "hw_config.h" 
//#include "usb_lib.h"
//#include "usb_pwr.h"
//#include "mass_mal.h"
#include "key.h"
//#include "delay.h"
#include "led.h"
#include "usart.h"
#include "log.h"
#include "EraseCode.h"
#include "match.h"
#include "stmflash.h"

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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


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

#include "stm32f10x_rcc.h"
// 40K
//#define ApplicationAddress 0x0800A000
// 60K
//#define ApplicationAddress 0x0800F000
// 30K
#define ApplicationAddress 0x08007800

// 20K
//__attribute__ ((section (".boot"))) const char _boot[0x5000] __attribute__((used)) = {
// 2K
//__attribute__ ((section (".boot"))) const char _boot[0x800] __attribute__((used)) = {
// 8K
//__attribute__ ((section (".boot"))) const char _boot[0x2000-8] __attribute__((used)) = {
//       0x12,0x12,0x12,0x12,
//	   0x12,0x12,0x12,0x12
//};
#include "flash_if.h"
void boot_reset(void)
{ /* Test if user code is programmed starting from address 0x8003000 */
	uint8_t code[1024]={0}; // 1K
	uint32_t addr=0;
	dprintf("boot_reset(0x%08X|0x%08X)...\r\n", ApplicationAddress, (*(__IO uint32_t*)ApplicationAddress));
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
	{ /* Jump to user application */
		//usb_port_set(0); 	//USB先断开
		//delay_ms(600);
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
		for(addr=0; addr<(20*1024); addr+=1024)
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
		//printf("boot_reset(0x%08X|0x%08X)...\r\n", 0X0800F000, (*(__IO uint32_t*)0X0800F000));
		dprintf("erase addr: 0x%08X\r\n", ApplicationAddress);
		//FLASH_Unlock();
		FLASH_ErasePage(ApplicationAddress);
		//FLASH_ErasePage(0X08000000);
		FLASH_Lock();
		printf("SystemReset!\r\n");
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
static void code_entry(void)
{
//	u8 key=0;
	//uint8_t usb=0;
//	unsigned int led=0;
//	unsigned int count=0;
//	unsigned int clock=0;
	LEDObj* ledObj=NULL;
	KeyObj* keyObj=NULL;

    //KEY_Init();          	//初始化与按键连接的硬件接口
	keyObj = NewKey_Obj();
	keyObj->Init();
	//LED_Init();
	ledObj = NewLED_Obj();
	ledObj->Init();

	/************************ 引导 *****************************/
	// 连续检测
	//lprintf("Check USB...\r\n");
//	clock = 0;
//	count = clock+200; // 0.2s
//	led = clock+40; // 0.04s

	/************************ 升级 *****************************/
	uart_init(115200);	 	//串口初始化为115200 
	//printf("\r\n\r\nBootloader version 0.16.2\r\n"); 
	printf("\r\n\r\n%s update boot version %d.%d.%d\r\n", PRODUCT, VERSION, PATCHLEVEL, SUBLEVEL);
	printf("%s", Description);
	printf("Date: %s--%s\r\n", __TIME__, __DATE__);
	printf("F_SIZE:%04X\r\n", (*(vu32*)(0x1ffff7e0))&0xFFFF);

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 关闭jtag需要使能该时钟
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	lprintf("update bootloader!\r\n");
	ledObj->LED2ON(0);
	ledObj->LED1ON(0);
	boot_reset();
}
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
	LEDObj* ledObj=NULL;
	ledObj = NewLED_Obj();
	ledObj->Init();

	// 连续检测
	clock = 0;
	led = clock+40; // 0.04s
	//for(clock=0; ; clock += 10)
	for(clock=0; clock<2000; clock += 10)
	//while(1)
	{
		delay_ms(10);
		//boot_app();
		if((clock+30) <= led)
		{
			ledObj->LED2ON(0);
			ledObj->LED1ON(1);
		}
		else if(clock <= led)
		{
			ledObj->LED2ON(1);
			ledObj->LED1ON(0);
		}
		else
		{
			led = clock+100; // 0.06s
		}
	}
	ledObj->LED2ON(1);
	ledObj->LED1ON(1);
	delay_ms(1000);
}

// 12K
//#define VectTabOffset  0x00003000
// 20K
#define VectTabOffset  0x00005000
int main(void)
{
//	mtext* _mtext=NULL;
//	mtext passwd;
//	uint32_t uid[3]={0};
//	char buf[BUF_LEN]={0};
//	int match=0;
//	uint32_t addr=0;
	LEDObj* ledObj=NULL;
	//EraseCodeObj* erase=NULL;
	unsigned int led=0;
	unsigned int clock=0;
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectTabOffset);
	_inline_delay_ms(5);
	ledObj = NewLED_Obj();
	ledObj->Init();
	ledObj->LED2ON(1);
	ledObj->LED1ON(1);
	delay_ms(1000);
	for(clock=0; clock<2000; clock+=10)
	{
		delay_ms(10);
		if((clock+30) <= led)
		{
			ledObj->LED2ON(0);
			ledObj->LED1ON(0);
		}
		else if(clock <= led)
		{
			ledObj->LED2ON(1);
			ledObj->LED1ON(1);
		}
		else
		{
			led = clock+70; // 0.02s
		}
	}
	
	code_entry(); 
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
