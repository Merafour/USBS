/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : ����׷��@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : U�̹̼�����������.
********************************************************************************
* merafour.blog.163.com
*
*******************************************************************************/

#include "config.h"

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
#include "stmflash.h"

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

//����USB ����/����
//enable:0,�Ͽ�
//       1,��������	   
__inline static  void usb_port_set(u8 enable)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	  	   	 
	//if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//�˳��ϵ�ģʽ
	if(enable)
	{
		if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//�˳��ϵ�ģʽ
	}
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  // �ϵ�ģʽ
		GPIOA->CRH&=0XFFF00FFF;
		GPIOA->CRH|=0X00033000;
		PAout(12)=0;	    		  
	}
} 
#include "stm32f10x_rcc.h"
// 40K
//#define ApplicationAddress 0x0800A000
// 20K
#define ApplicationAddress 0x0800F000
//void boot_app(void)
//{ /* Test if user code is programmed starting from address 0x8003000 */
//	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
//	{ /* Jump to user application */
//		//usb_port_set(0); 	//USB�ȶϿ�
//		/* DISABLE the USB clock */
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, DISABLE);
//		JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
//		Jump_To_Application = (pFunction) JumpAddress;
//		/* Initialize user application's Stack Pointer */
//		__set_MSP(*(__IO uint32_t*) ApplicationAddress);
//		//lprintf("goto app!\r\n");
//		//USART_Cmd(USART1, DISABLE);
//		delay_ms(5);
//		Jump_To_Application();
//		while(1); //App ��������²�?
//	}
//} /* Otherwise enters DFU mode to allow user to program his application */

// 20K
//__attribute__ ((section (".boot"))) const char _boot[0x5000] __attribute__((used)) = {
//       0x12,0x12,0x12,0x12,
//	   0x12,0x12,0x12,0x12
//};
#include "flash_if.h"
#if 0
void boot_reset(void)
{ /* Test if user code is programmed starting from address 0x8003000 */
	char code[1024]={0}; // 1K
	uint32_t addr=0;
	printf("boot_reset(0x%08X|0x%08X)...\r\n", ApplicationAddress, (*(__IO uint32_t*)ApplicationAddress));
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
	{ /* Jump to user application */
		usb_port_set(0); 	//USB�ȶϿ�
		//delay_ms(600);
		FLASH_Unlock();
		// ��������
		for(addr=0; addr<(20*1024); addr+=1024)
		{
			printf("erase addr: 0x%08X\r\n", addr);
			FLASH_ErasePage(0X08000000+addr);
			delay_ms(30);
		}
		FLASH_Lock();
		// ��������
		for(addr=0; addr<(20*1024); addr+=1024)
		{
			printf("write addr: 0x%08X\r\n", addr);
			STMFLASH_Read(ApplicationAddress+addr, (uint16_t*)code, 1024/2);
			delay_ms(10);
			//STMFLASH_Write(0X0800F000+addr,(u16*)code, 1024/2);
			STMFLASH_Write(0X08000000+addr,(u16*)code, 1024/2);
			delay_ms(30);
		}
		//printf("boot_reset(0x%08X|0x%08X)...\r\n", 0X0800F000, (*(__IO uint32_t*)0X0800F000));
		printf("erase addr: 0x%08X\r\n", ApplicationAddress);
		for(addr=0; addr<1024; addr++) code[addr]=0xFF;
		STMFLASH_Write(ApplicationAddress,(u16*)code, 1024/2);
		//STMFLASH_Write(0X08000000,(u16*)code, 1024/2);
		printf("SystemReset!\r\n");
		delay_ms(40);
		NVIC_SystemReset(); // �����λ
	}
} 
#else
void boot_reset(void)
{ /* Test if user code is programmed starting from address 0x8003000 */
	uint8_t code[1024]={0}; // 1K
	uint32_t addr=0;
	printf("boot_reset(0x%08X|0x%08X)...\r\n", ApplicationAddress, (*(__IO uint32_t*)ApplicationAddress));
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
	{ /* Jump to user application */
		usb_port_set(0); 	//USB�ȶϿ�
		//delay_ms(600);
		FLASH_Unlock();
		// ��������
		for(addr=0; addr<(20*1024); addr+=1024)
		{
			printf("erase addr: 0x%08X\r\n", addr);
			FLASH_ErasePage(0X08000000+addr);
			delay_ms(30);
		}
		//FLASH_Lock();
		// ��������
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
			delay_ms(30);
		}
		//printf("boot_reset(0x%08X|0x%08X)...\r\n", 0X0800F000, (*(__IO uint32_t*)0X0800F000));
		printf("erase addr: 0x%08X\r\n", ApplicationAddress);
		//FLASH_Unlock();
		FLASH_ErasePage(ApplicationAddress);
		//FLASH_ErasePage(0X08000000);
		FLASH_Lock();
		printf("SystemReset!\r\n");
		delay_ms(40);
		NVIC_SystemReset(); // �����λ
	}
} 
#endif
/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static int Flag=0; // ͨ���ñ�������Ƿ�ǿ�������˼���У�����
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

	if(0==Flag) // ����������У��
	{
		NVIC_SystemReset(); // �����λ
		return;
	}
	//lprintf("Bootloader version 0.10\r\n");
	//lprintf("USB DisConnected\r\n");
	//usb_port_set(0); 	//USB�ȶϿ�
    //KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
	keyObj = NewKey_Obj();
	keyObj->Init();
	//LED_Init();
	ledObj = NewLED_Obj();
	ledObj->Init();

	/************************ ���� *****************************/
	// �������
	//lprintf("Check USB...\r\n");
	clock = 0;
	count = clock+200; // 0.2s
	led = clock+40; // 0.04s
	//for(clock=0; ; clock += 10)
//	for(clock=0; clock<400; clock += 10)
//	//for(clock=0; clock<2000; clock = 0)
//	//while(1)
//	{
//		delay_ms(10);
//		key=keyObj->Scan(0);
//		if(USB_PRES == key) // update
//		{
//			count = clock+100; // 0.1s
//		}
//		else if(clock>=count) // ���� app
//		{
//			//dprintf("boot app 01\r\n");
//			boot_app();
////			clock = 0;
////			count = clock+200; // 0.2s
////			led = clock+50; // 0.06s
//		}
//		if((clock+20) <= led)
//		{
//			ledObj->LED2ON(0);
//			ledObj->LED1ON(1);
//		}
//		else if(clock <= led)
//		{
//			ledObj->LED2ON(1);
//			ledObj->LED1ON(0);
//		}
//		else
//		{
//			led = clock+40; // 0.06s
//		}
//	}

	/************************ ���� *****************************/
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200 
	//printf("\r\n\r\nBootloader version 0.16.2\r\n"); 
	printf("\r\n\r\n%s update boot version %d.%d.%d\r\n", PRODUCT, VERSION, PATCHLEVEL, SUBLEVEL);
	printf("%s", Description);
	printf("Date: %s--%s\r\n", __TIME__, __DATE__);
	printf("F_SIZE:%04X\r\n", (*(vu32*)(0x1ffff7e0))&0xFFFF);

#if 0 // �����ⲿ�ֳ�ʼ���ŵ� New_Key_Obj������
	Mass_Memory_Size[0]=(long long)492*1024;
	Mass_Block_Size[0] =512;
	Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
	lprintf("Mass_Memory_Size[%d]:%d byte\r\n", 0, Mass_Memory_Size[0]);
	lprintf("Mass_Block_Size[%d]:%d byte\r\n", 0, Mass_Block_Size[0]);
	lprintf("Mass_Block_Count[%d]:%d\r\n", 0, Mass_Block_Count[0]);
#endif
	//Mass_Memory_Size[0]=(long long)232*1024;
	Mass_Memory_Size[0]=(long long)(40)*1024;
	Mass_Block_Size[0] =512;
	Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
	printf("Mass_Memory_Size[%d]:%d byte\r\n", 0, Mass_Memory_Size[0]);
	printf("Mass_Block_Size[%d]:%d byte\r\n", 0, Mass_Block_Size[0]);
	printf("Mass_Block_Count[%d]:%d\r\n", 0, Mass_Block_Count[0]);
	printf("USB Connected\r\n");
 	//usb_port_set(0); 	//USB�ȶϿ�
	//delay_ms(300);
   	usb_port_set(1);	//USB�ٴ�����
	printf("Init USB...\r\n");
	Set_System();
	//lprintf("Set_System...\r\n");
	Set_USBClock();
	//lprintf("Set_USBClock...\r\n");
	//Led_Config(); // �����ر�SWD����
	USB_Interrupts_Config();
	//lprintf("USB_Interrupts_Config...\r\n");
	USB_Init();
	//lprintf("USB_Init...\r\n");
	delay_ms(10);
	printf("CONFIGURED State\r\n");
	//while (bDeviceState != CONFIGURED);
	
	//USB_Configured_LED();

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // �ر�jtag��Ҫʹ�ܸ�ʱ��
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
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
			count = clock+2000; // 1s
			//lprintf("USB_PRES\r\n");
		}
		else if(clock>=count) // ���� app
		{
			lprintf("update bootloader!\r\n");
			//boot_app();
			//NVIC_SystemReset(); // �����λ
			boot_reset();
			count = clock+1000; // 1s
		}
		if((clock+50) <= led)
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
			led = clock+200; // 0.06s
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
//			usb_port_set(0); 	//USB�ȶϿ�
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
void test_entry(void)
{
	unsigned int led=0;
	unsigned int clock=0;
	LEDObj* ledObj=NULL;
	ledObj = NewLED_Obj();
	ledObj->Init();

	// �������
	clock = 0;
	led = clock+40; // 0.04s
	for(clock=0; ; clock += 10)
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
}

// 20K
#define VectTabOffset  0x00005000
int main(void)
{
	mtext* _mtext=NULL;
	mtext passwd;
	uint32_t uid[3]={0};
	char buf[BUF_LEN]={0};
	int match=0;
	uint32_t addr=0;
	LEDObj* ledObj=NULL;
	//EraseCodeObj* erase=NULL;
	unsigned int led=0;
	unsigned int clock=0;
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectTabOffset);
	//test_entry();
	//FLASH_Unlock();
	//uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200 
	_inline_delay_ms(5);
	//printf("\r\n\r\nBootloader version 0.16.2\r\n"); 
	//printf("\r\n\r\n%s Bootloader version %d.%d.%d\r\n", PRODUCT, VERSION, PATCHLEVEL, SUBLEVEL);
	//printf("Date: %s--%s\r\n", __TIME__, __DATE__);
	//printf("F_SIZE:%04X\r\n", (*(vu32*)(0x1ffff7e0))&0xFFFF);
	//lprintf("USB DisConnected\r\n");
	usb_port_set(0); 	//USB�ȶϿ�
//	if(0==match_first()) // 0:��һ���ϵ�,����
//	{
//		//FLASH_Unlock();
//		uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
//		//erase=NewEraseCode_Obj();
//		//erase->Init();
//		//erase->Erase(code_entry);
//		cram_entry();
//	}
	// ���Լ���
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
		uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200 
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
		ledObj = NewLED_Obj();
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
//	for(clock=0; clock<5000; clock+=10)
//	{
//		delay_ms(10);
//		if((clock+30) <= led)
//		{
//			ledObj->LED2ON(0);
//			ledObj->LED1ON(0);
//		}
//		else if(clock <= led)
//		{
//			ledObj->LED2ON(1);
//			ledObj->LED1ON(1);
//		}
//		else
//		{
//			led = clock+70; // 0.02s
//		}
//	}
	code_entry();
	goto skip_id;
	while(1);
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
