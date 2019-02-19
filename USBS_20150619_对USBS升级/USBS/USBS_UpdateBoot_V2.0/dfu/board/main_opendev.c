/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Mass Storage demo main file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "hw_config.h" 
#include "usb_lib.h"
#include "usb_pwr.h"
#include "mass_mal.h"
#include "key.h"
//#include "delay.h"
#include "led.h"
#include "usart.h"
#include "log.h"
#include "EraseCode.h"
#include "match.h"

extern uint16_t MAL_Init (uint8_t lun);

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

//设置USB 连接/断线
//enable:0,断开
//       1,允许连接	   
__inline static  void usb_port_set(u8 enable)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	  	   	 
	//if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//退出断电模式
	if(enable)
	{
		if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//退出断电模式
	}
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  // 断电模式
		GPIOA->CRH&=0XFFF00FFF;
		GPIOA->CRH|=0X00033000;
		PAout(12)=0;	    		  
	}
} 
#include "stm32f10x_rcc.h"
// 40K
//#define ApplicationAddress 0x0800A000
// 20K
#define ApplicationAddress 0x08005000
void boot_app(void)
{ /* Test if user code is programmed starting from address 0x8003000 */
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
	{ /* Jump to user application */
		//usb_port_set(0); 	//USB先断开
		//FLASH_Lock();//上锁
		/* DISABLE the USB clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, DISABLE);
		JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
		Jump_To_Application = (pFunction) JumpAddress;
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) ApplicationAddress);
		//lprintf("goto app!\r\n");
		//USART_Cmd(USART1, DISABLE);
		delay_ms(5);
		Jump_To_Application();
		while(1); //App 正常情况下不?
	}
} /* Otherwise enters DFU mode to allow user to program his application */
void boot_reset(void)
{ /* Test if user code is programmed starting from address 0x8003000 */
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
	{ /* Jump to user application */
		usb_port_set(0); 	//USB先断开
		//FLASH_Lock();//上锁
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
#if 0
int main(void)
{
	u8 key=0;
	unsigned int led=0;
	unsigned int count=0;
	unsigned int clock=0;
	LEDObj* ledObj=NULL;
	KeyObj* keyObj=NULL;
	uart_init(115200);	 	//串口初始化为9600 
	lprintf("Bootloader version 0.10\r\n");
	lprintf("USB DisConnected\r\n");
	usb_port_set(0); 	//USB先断开
    //KEY_Init();          	//初始化与按键连接的硬件接口
	keyObj = NewAT9Key_Obj();
	//keyObj = NewCB88Key_Obj();
	//keyObj = NewOpendevKey_Obj();
	keyObj->Init();
	//LED_Init();
	ledObj = NewAT9LED_Obj();
	//ledObj = NewCB88LED_Obj();
	//ledObj = NewOpendevLED_Obj();
	ledObj->Init();
#if 0 // 现在这部分初始化放到 New_Key_Obj函数中
	Mass_Memory_Size[0]=(long long)492*1024;
	Mass_Block_Size[0] =512;
	Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
	lprintf("Mass_Memory_Size[%d]:%d byte\r\n", 0, Mass_Memory_Size[0]);
	lprintf("Mass_Block_Size[%d]:%d byte\r\n", 0, Mass_Block_Size[0]);
	lprintf("Mass_Block_Count[%d]:%d\r\n", 0, Mass_Block_Count[0]);
#endif

	// 连续检测
	lprintf("Check USB...\r\n");
	clock = 0;
	count = clock+200; // 0.2s
	led = clock+40; // 0.04s
	for(clock=0; clock<400; clock += 10)
	//for(clock=0; clock<2000; clock = 0)
	//while(1)
	{
		delay_ms(10);
		key=keyObj->Scan(0);
		if(USB_PRES == key) // update
		{
			count = clock+100; // 0.1s
		}
		else if(clock>=count) // 引导 app
		{
			lprintf("boot app 01\r\n");
			boot_app();
//			clock = 0;
//			count = clock+200; // 0.2s
//			led = clock+50; // 0.06s
		}
		if((clock+20) <= led)
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
			led = clock+40; // 0.06s
		}
	}

	lprintf("USB Connected\r\n");
 	//usb_port_set(0); 	//USB先断开
	//delay_ms(300);
   	usb_port_set(1);	//USB再次连接
	lprintf("Init USB...\r\n");
	Set_System();
	Set_USBClock();
	//Led_Config(); // 这里会关闭SWD功能
	USB_Interrupts_Config();
	USB_Init();
	delay_ms(10);
	lprintf("CONFIGURED State\r\n");
	//while (bDeviceState != CONFIGURED);
	
	//USB_Configured_LED();

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 关闭jtag需要使能该时钟
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	lprintf("while(1)\r\n");
	clock = 0;
	count = clock+200; // 0.2s
	led = clock+50; // 0.06s
	while(1)
	{
		delay_ms(10);
		clock += 10; // update clock
		key=keyObj->Scan(0);
		if(USB_PRES == key) // update
		{
			count = clock+1000; // 1s
			//lprintf("USB_PRES\r\n");
		}
		else if(clock>=count) // 引导 app
		{
			lprintf("boot app 02\r\n");
			boot_app();
			//count = clock+1000; // 1s
		}
		if((clock+200) <= led)
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
			led = clock+400; // 0.06s
		}
	}

  while (1)
  {}
}
#else
static int Flag=0; // 通过该变量检测是否强行跳过了加密校验代码
extern const char Description[];
static void code_entry(void)
{
	u8 key=0;
	//uint8_t usb=0;
	unsigned int led=0;
	unsigned int count=0;
	unsigned int clock=0;
	LEDObj* ledObj=NULL;
	KeyObj* keyObj=NULL;

	if(0==Flag) // 被跳过加密校验
	{
		//FLASH_Lock();//上锁
		NVIC_SystemReset(); // 软件复位
		return;
	}
	//lprintf("Bootloader version 0.10\r\n");
	//lprintf("USB DisConnected\r\n");
	//usb_port_set(0); 	//USB先断开
    //KEY_Init();          	//初始化与按键连接的硬件接口
	keyObj = NewOpendevKey_Obj();
	keyObj->Init();
	//LED_Init();
	ledObj = NewOpendevLED_Obj();
	ledObj->Init();

	/************************ 引导 *****************************/
	// 连续检测
	//lprintf("Check USB...\r\n");
	clock = 0;
	count = clock+200; // 0.2s
	led = clock+40; // 0.04s
	//for(clock=0; ; clock += 10)
	for(clock=0; clock<400; clock += 10)
	//for(clock=0; clock<2000; clock = 0)
	//while(1)
	{
		delay_ms(10);
		key=keyObj->Scan(0);
		if(USB_PRES == key) // update
		{
			count = clock+100; // 0.1s
		}
		else if(clock>=count) // 引导 app
		{
			//dprintf("boot app 01\r\n");
			boot_app();
//			clock = 0;
//			count = clock+200; // 0.2s
//			led = clock+50; // 0.06s
		}
		if((clock+20) <= led)
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
			led = clock+40; // 0.06s
		}
	}

	/************************ 升级 *****************************/
	//FLASH_Unlock();
	uart_init(115200);	 	//串口初始化为115200 
	//printf("\r\n\r\nBootloader version 0.16.2\r\n"); 
	printf("\r\n\r\n%s Bootloader version %d.%d.%d\r\n", PRODUCT, VERSION, PATCHLEVEL, SUBLEVEL);
	printf("%s", Description);
	printf("Date: %s--%s\r\n", __TIME__, __DATE__);
	printf("F_SIZE:%04X\r\n", (*(vu32*)(0x1ffff7e0))&0xFFFF);

#if 0 // 现在这部分初始化放到 New_Key_Obj函数中
	Mass_Memory_Size[0]=(long long)492*1024;
	Mass_Block_Size[0] =512;
	Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
	lprintf("Mass_Memory_Size[%d]:%d byte\r\n", 0, Mass_Memory_Size[0]);
	lprintf("Mass_Block_Size[%d]:%d byte\r\n", 0, Mass_Block_Size[0]);
	lprintf("Mass_Block_Count[%d]:%d\r\n", 0, Mass_Block_Count[0]);
#endif
	//Mass_Memory_Size[0]=(long long)232*1024;
	Mass_Memory_Size[0]=(long long)256*1024;
	Mass_Block_Size[0] =512;
	Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
	printf("Mass_Memory_Size[%d]:%d byte\r\n", 0, Mass_Memory_Size[0]);
	printf("Mass_Block_Size[%d]:%d byte\r\n", 0, Mass_Block_Size[0]);
	printf("Mass_Block_Count[%d]:%d\r\n", 0, Mass_Block_Count[0]);
	printf("USB Connected\r\n");
 	//usb_port_set(0); 	//USB先断开
	//delay_ms(300);
   	usb_port_set(1);	//USB再次连接
	printf("Init USB...\r\n");
	Set_System();
	//lprintf("Set_System...\r\n");
	Set_USBClock();
	//lprintf("Set_USBClock...\r\n");
	//Led_Config(); // 这里会关闭SWD功能
	USB_Interrupts_Config();
	//lprintf("USB_Interrupts_Config...\r\n");
	USB_Init();
	//lprintf("USB_Init...\r\n");
	delay_ms(10);
	printf("CONFIGURED State\r\n");
	//while (bDeviceState != CONFIGURED);
	
	//USB_Configured_LED();

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 关闭jtag需要使能该时钟
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	printf("while(1)\r\n");
	clock = 0;
	count = clock+200; // 0.2s
	led = clock+50; // 0.06s
	while(1)
	{
		delay_ms(10);
		clock += 10; // update clock
		key=keyObj->Scan(0);
		dprintf("clock:%d, key:%d, led:%d\r\n", clock, key, led);
		if(USB_PRES == key) // update
		{
			count = clock+1000; // 1s
			//lprintf("USB_PRES\r\n");
		}
		else if(clock>=count) // 引导 app
		{
			lprintf("boot app 02\r\n");
			//boot_app();
			//NVIC_SystemReset(); // 软件复位
			boot_reset();
			count = clock+1000; // 1s
		}
		if((clock+150) <= led)
		{
			ledObj->LED2ON(0); // AT9
			ledObj->LED1ON(0);
		}
		else if(clock <= led)
		{
			ledObj->LED2ON(1); // AT9
			ledObj->LED1ON(1);
		}
		else
		{
			led = clock+500; // 0.06s
		}
//		if((0==usb) && (CONFIGURED==bDeviceState))
//		{
//			usb = 1;
//			printf("USB Connected\r\n");
//		}
//		if((1==usb) && (CONFIGURED!=bDeviceState))
//		{
//			usb = 2;
//			printf("USB DisConnected\r\n");
//			usb_port_set(0); 	//USB先断开
//		}
	}
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
static void test_entry(void)
{
	unsigned int led=0;
	unsigned int count=0;
	unsigned int clock=0;
	LEDObj* ledObj=NULL;
	ledObj = NewOpendevLED_Obj();
	ledObj->Init();

	// 连续检测
	clock = 0;
	count = clock+200; // 0.2s
	led = clock+40; // 0.04s
	for(clock=0; ; clock += 10)
	{
		delay_ms(10);
		boot_app();
		if((clock+50) <= led)
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
}
int main(void)
{
	mtext* _mtext=NULL;
	mtext passwd;
	uint32_t uid[3]={0};
	char buf[BUF_LEN]={0};
	int match=0;
	uint32_t addr=0;
	LEDObj* ledObj=NULL;
	EraseCodeObj* erase=NULL;
	unsigned int led=0;
	unsigned int clock=0;
	//test_entry();
	//FLASH_Unlock();
	//uart_init(115200);	 	//串口初始化为115200 
	_inline_delay_ms(5);
	//printf("\r\n\r\nBootloader version 0.16.2\r\n"); 
	//printf("\r\n\r\n%s Bootloader version %d.%d.%d\r\n", PRODUCT, VERSION, PATCHLEVEL, SUBLEVEL);
	//printf("Date: %s--%s\r\n", __TIME__, __DATE__);
	//printf("F_SIZE:%04X\r\n", (*(vu32*)(0x1ffff7e0))&0xFFFF);
	//lprintf("USB DisConnected\r\n");
	usb_port_set(0); 	//USB先断开
	if(0==match_first()) // 0:第一次上电,加密
	{
		//FLASH_Unlock();
		uart_init(115200);	 	//串口初始化为115200
		erase=NewEraseCode_Obj();
		erase->Init();
		erase->Erase(code_entry);
	}
	// 测试加密
	{
		// read UID
		uid[0]=*(vu32*)(0x1ffff7e8);
		uid[1]=*(vu32*)(0x1ffff7ec);
		uid[2]=*(vu32*)(0x1ffff7f0);
		// passwd
		passwd_id(&passwd, uid);
		passwd_sign(&passwd, uid);
		Flag = 1;
		// read flash
		match_Read(0x08004C00, (uint16_t*)buf, BUF_LEN/2);
		_mtext = (mtext*)buf;
		// match
		if(passwd.id[0] != _mtext->id[0]) match=1;
		if(passwd.id[1] != _mtext->id[1]) match=1;
		if(passwd.id[2] != _mtext->id[2]) match=1;
		if(passwd.sign[0] != _mtext->sign[0]) match=1;
#if 0
		if(passwd.sign[1] != _mtext->sign[1]) match=1;
		if(passwd.sign[2] != _mtext->sign[2]) match=1;
#else
		if((passwd.sign[1]&0XFFFF0000) != (_mtext->sign[1]&0XFFFF0000)) match=1;
		if((passwd.sign[2]&0X0000FFFF) != (_mtext->sign[2]&0X0000FFFF)) match=1;
#endif
		if(passwd.sign[3] != _mtext->sign[3]) match=1;

		addr = (_mtext->sign[2]&0XFFFF0000) | (_mtext->sign[1]&0X0000FFFF);
		addr = addr ^ passwd.sign[0];
		//dprintf("bootloader addr :%08X\r\n", addr);
	}
	if(match)
	{
skip_id:
		uart_init(115200);	 	//串口初始化为115200 
		printf("\r\n\r\n%s Bootloader version %d.%d.%d\r\n", PRODUCT, VERSION, PATCHLEVEL, SUBLEVEL);
		printf("Date: %s--%s\r\n", __TIME__, __DATE__);
		printf("F_SIZE:%04X\r\n", (*(vu32*)(0x1ffff7e0))&0xFFFF);
		dprintf("bootloader addr :%08X\r\n", addr);
		printf("match error!\r\n");
		printf("uid:%08X, %08X, %08X\r\n", uid[0], uid[1], uid[2]);
		printf("id:%08X, _id:%08X\r\n", passwd.id[0], _mtext->id[0]);
		printf("id:%08X, _id:%08X\r\n", passwd.id[1], _mtext->id[1]);
		printf("id:%08X, _id:%08X\r\n", passwd.id[2], _mtext->id[2]);
		dprintf("id:%08X, _id:%08X\r\n", passwd.sign[0], _mtext->sign[0]);
		dprintf("id:%08X, _id:%08X\r\n", passwd.sign[1], _mtext->sign[1]);
		dprintf("id:%08X, _id:%08X\r\n", passwd.sign[2], _mtext->sign[2]);
		dprintf("id:%08X, _id:%08X\r\n", passwd.sign[3], _mtext->sign[3]);
		ledObj = NewOpendevLED_Obj();
		ledObj->Init();
		while(1)
		{
			delay_ms(10);
			clock += 10; // update clock
			if((clock+100) <= led)
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
				led = clock+400; // 0.02s
			}
		}
	}
	code_entry();
	goto skip_id;
	while(1);
}
#endif

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
