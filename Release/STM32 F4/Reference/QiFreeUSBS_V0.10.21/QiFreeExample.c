/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 6/12/2015
* Description        : QiFree IO.
********************************************************************************
* merafour.blog.163.com
* 深圳市乐迪电子有限公司:www.radiolink.com.cn
*******************************************************************************/
#include "QiFreeIO.h"
#include <stdio.h>
#include "Config.h"
#include "log.h"

QiFreeIO led1={0,0,0};
QiFreeIO led2={0,0,0};
QiFreeIO key={0,0,0};
QiFree_ioObj Qi;
uint32_t UserData = 0; // KB

const char Description[]="\
* Author   : merafour.blog.163.com\r\n\
* Version  : V1.0.0\r\n\
* Date     : 2/26/2015\r\n";
//const char Description[]="\
//* Author   : merafour.blog.163.com\r\n\
//* Date     : 2/26/2015\r\n";

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
 *        ...| Board name      | ...H
 *           |                 | {4*4}B
 *           |-----------------|
 *         22| signDate        | 58H
 *        ...|                 | ...H
 *           |                 | {8*4}B
 *           |-----------------|
 *         32| empty           | 80H
 *           |                 | ...H
 *           |                 |	   40*4:VERSION
 */
// 256B
const uint32_t ConfigExamples[(0x0F0)/4] = {
	   0x00000000, // __initial_sp
	   0x00000000, // Reset_Handler
       0x01010000, // default //0K	| swd| erase| flashH| flashL|
	   //0x00010100, // default //256K	| swd| erase| flashH| flashL|
	   0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12, //mtext 
	   0x12, // empty
	   'E', 0x00000004,// KEY :PE4,High
	   'F', 0xFFFF0009,// LED1:PF9 ,Low
	   'F', 0xFFFF000A,// LED2:PF10 ,Low
	   0x72466951, 0x00006565, 0x00000000, 0x00000000, // Board:QiFree
	   // signDate
	   0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
	   // IO default
	   0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
	   // License
	   0x990951ba,
	   0x00000000, 0x00000000, 0x00000000, 0x00000000, 
	   0x00000000, 0x00000000, 0x00000000, 0x00000000, 
	   0x00000000, 0x00000000, 0x00000000, 0x00000000, 
};
__attribute__ ((section (".version"))) const uint8_t _version[0x10] __attribute__((used)) = "V "VERSION"."PATCHLEVEL"."SUBLEVEL"\r\n\0";

//char *Enable[2] = {"Enable", "DisEnable"};
static const char *Enable[2] = {"DisEnable", "Enable"};

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
//static int led_low=0;
//static int key_low=0;
//static uint32_t swd=1;
void QiFreeConfigIO(uint32_t map[])
{
    int led_low=0;
  int key_low=0;
  uint32_t swd=1;
	// 关闭 jtag
	// | swd| erase| flashH| flashL|   
	swd = (map[2]>>24)&0x1;
 	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 关闭jtag需要使能该时钟
	//if(((*(uint32_t*)(buf+8))>>24)&0x1) GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	//if(swd) GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	//else GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); // 关闭jtag，关闭SWD，只能使用串口烧录

	// IO default
	QiFreeIO_Default(map+32);

	// IO Select
	// 获取相应的 IO函数集
	key_low = (map[13]>>16)&0x1;
	led_low = (map[15]>>16)&0x1;
	//newQiFree_ioObj(&Qi, 1, 1); // LED低电平亮, KEY低电平有效
	newQiFree_ioObj(&Qi, led_low, key_low); // LED低电平亮, KEY低电平有效
	QiFreeIO_config(map+12, &key);
	QiFreeIO_config(map+14, &led1);
	QiFreeIO_config(map+16, &led2);

	Qi.led_init(&led1);
	Qi.led_init(&led2);
	Qi.key_init(&key);
}
void QiFreeConfigLog(uint32_t map[])
{
	int led_low=0;
	int key_low=0;
	uint32_t KB=128;
	uint32_t swd=1;
	int i=0;
	uint32_t *gpio_value=NULL;

	/************************ Log *****************************/
	//printf("\r\n\r\nBootloader version 0.16.2\r\n"); 
	//printf("\r\n\r\n%s QiFreeUSBS update version %d.%d.%d\r\n", PRODUCT, VERSION, PATCHLEVEL, SUBLEVEL);
	//printf("%s", Description);
	//printf("Date: %s--%s\r\n", __TIME__, __DATE__);
	//printf("F_SIZE:%04X\r\n", (*(vu32*)(0x1ffff7e0))&0xFFFF);
	//match_Read(LinkAddress, (uint16_t*)buf, 1024/2); // 20K
	//map = (uint32_t *)LinkAddress;
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
	KB = (*(vu32*)(0x1ffff7e0))&0xFFFF;
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
	KB = (*(vu32*)(0x1FFF7A22))&0xFFFF;
	printf("Flash:%08X\r\n", KB);
	//printf("Flash:%dKB\r\n", KB&0x0000FFFF);
	UserData = KB&0x0000FFFF;
	if(UserData>32) UserData=32;
	printf("UserData:%dKB\r\n", KB&0x0000FFFF);
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
	printf("Board:%s\r\n", (char*)(map+18));
	printf("Date:%s\r\n", (char*)(map+22));
	//printf("0x08004C20:%s", (char*)0x08004C20);
	//printf("0x08004C50:%s", (char*)0x08004C40);
	printf("0x080000F0:%s\r\n", (char*)0x080000F0);
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


