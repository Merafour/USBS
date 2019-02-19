#include "EraseCode.h"
#include "log.h"
#include "stm32f10x.h"
#include "stmflash.h"

#define UID0    (0x357584edU)
#define UID1    (0x46734568U)
#define UID2    (0xadc86789U)
#define UID     (0xa366db89U)

typedef struct {
    uint32_t id[3];  // 加密信息
	uint32_t sign[4];   // 加密签名
    FuncSection code;   //函数指针
}mtext;

#ifndef STM32_FLASH_BASE
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
#endif

#define ERASE_SECTOR_SIZE 1024 //字节 //擦除段的大小
// 19K 0x4C00
//#define _mtext_start               0x4C00	/* Passwd */
//#define _mtext_size                0x0020   /* 32B */
//#define _mdata_start               0x4C20	/* Data */
//#define _mdata_size                0x0020   /* 32B */
//#define _etext_start               0x4C40	/* erase Code */
//#define _etext_size                0x00A0   /* 160B */
//#define _passwd_start              0x4CE0	/* Data */
//#define _passwd_size               0x0020   /* 32B */
#define _ERASE_CODE_ADDR             0x4C00   /* 19K */
/*
|      19K(0x4C00)           |                        |
|----------------------------|------------------------|
|                            |       0x4C00           |
|      .mtext  (0x20)        |                        |
|----------------------------|------------------------|
|                            |       0x4C20           |
|      .mdata  (0x20)        |                        |
|----------------------------|------------------------|
|                            |       0x4C40           |
|      .etext  (0x80+0x20)   |                        |
|----------------------------|------------------------|
|                            |       0x4CE0           |
|      .passwd (0x20)        |                        |
|----------------------------|------------------------|
|      19K(0x4C00)+0x100     |                        |

==> new
|      19K(0x4C00)           |                        |
|----------------------------|------------------------|
|                            |       0x4C00           |
|      .etext  (0x08)        |                        |
|----------------------------|------------------------|
|                            |       0x4C08           |
|      .mdata  (0x10)        |                        |
|----------------------------|------------------------|
|                            |       0x4C18           |
|      .mtext  (0x20)        |                        |
|----------------------------|------------------------|
|                            |       0x4C38           |
|      .passwd+.code (0x20)  |                        |
|----------------------------|------------------------|
|      19K(0x4C00)+0x200     |                        |
 */
static start_main code_entry; // 加密之后的入口
//static FuncSection _passwd_code_func;

void TestSection(void);
void passwd_code(void);
//static int _erase_code(uint8_t section[]);
static void _erase_code(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);

__attribute__ ((section (".mtext"))) const mtext __MtextFlash[] __attribute__((used)) = {
//__attribute__ ((section (".mtext"))) mtext __MtextFlash[1] = { 
        {{0x12345678,0x12345678,0x12345678}, {0xFF,0xFF,0xFF}, &passwd_code}
};
void _TestSection(void)  
{
	//lprintf("******** Test Flash OK! ********\r\n");
	code_entry();
	//start_main _entry=NULL;
	//STMFLASH_Write_zone(_mtext_start+STM32_FLASH_BASE, (u16*)&_entry, sizeof(_entry)/2);
	//return 0;
}
__attribute__ ((section (".mdata"))) __attribute__((used)) void TestSection(void)  
//__attribute__((section(".mdata"))) int TestSection(void)  
//__attribute__( ( ".mdata" ) ) __STATIC_INLINE int TestSection(void)
{
	//lprintf("******** Test Flash OK! ********\r\n");
	//_TestSection();
	//passwd_code();
	code_entry();
	//return 0;
}
#if 1
__attribute__ ((section (".match"))) __attribute__((used)) int match_first(void)  
{
	return 0;
}
__attribute__ ((section ("._match"))) __attribute__((used)) int _match_first(void)  
{
	return 1;
}
#else
int match_first(void)  
{
	return 0;
}
int _match_first(void)  
{
	return 1;
}
#endif
__attribute__ ((section (".ftext"))) __attribute__((used)) int erase_first(void)  
{
	char code[4]={0};
	uint32_t addr=0;
	// 读出 _code_first函数的指令
	addr = (uint32_t)_code_first;
	STMFLASH_Read(addr, (uint16_t*)code, 0x04/2);
	// 覆盖 code_first函数的指令
	addr = (uint32_t)code_first;
	STMFLASH_Write_zone(addr, (uint16_t*)code, 0x04/2);
	return 1;
}
__attribute__ ((section (".passwd"))) const mtext __passwd[] __attribute__((used)) = {
//__attribute__ ((section (".mtext"))) mtext __MtextFlash[1] = { 
        {{0xAABB3344,0xABED2345,0x88779933}, {0xFF,0xFF,0xFF}, 0}
};
//__attribute__ ((section (".edata"))) __attribute__((used)) int passwd_code(void)  
//__attribute__ ((section (".mtext.edata"))) __attribute__((used)) int passwd_code(void)  
#if 1
//static uint8_t section_buf[ERASE_SECTOR_SIZE] = {0};
//static uint32_t mcu_uid[3];
//static mtext* _mtext=NULL;
//static mtext* test_mtext=NULL;

static __attribute__ ((section ("._code"))) __attribute__((used)) void encoding(mtext* _mtext, uint32_t uid[3])
{
    _mtext->id[0] = (uid[0]&uid[1])|((~uid[0])&uid[2]);
    _mtext->id[1] = (uid[0]&uid[2])|(uid[1]&(~uid[2]));
    _mtext->id[2] = uid[0]^uid[1]^uid[2];
	_mtext->sign[0]  = uid[0]^(_mtext->id[2]|(~UID2));
	_mtext->sign[1]  = ((uid[0]>>16)&0XFFFF0000) | (uid[1]&0XFFFF0000); // uid1[15:8] uid0[15:8]
	_mtext->sign[2]  = ((uid[1]<<16)&0XFFFF0000) | (uid[2]&0X0000FFFF); // uid1[7:0] uid2[7:0]
	_mtext->sign[3]  = ((uid[0]<<16)&0XFFFF0000) | ((uid[2]>>16)&0X0000FFFF); // uid0[7:0] uid2[15:8]
}

static __attribute__ ((section ("._code"))) __attribute__((used)) void _passwd_code(void)
{
	mtext* test_mtext=NULL;
	uint8_t* etext=NULL;
	uint8_t* mdata=NULL;
	uint8_t* _code=NULL;
	uint32_t mcu_uid[3];
	uint8_t section_buf[ERASE_SECTOR_SIZE] = {0};
	uint32_t i=0;
	printf("TestSection:%08X\r\n", TestSection);
	printf("passwd_code:%08X\r\n", passwd_code);
	printf("match_first:%08X\r\n", match_first);
	printf("_match_first:%08X\r\n", _match_first);
	// read UID
	mcu_uid[0]=*(vu32*)(0x1ffff7e8);
	mcu_uid[1]=*(vu32*)(0x1ffff7ec);
	mcu_uid[2]=*(vu32*)(0x1ffff7f0);
	STMFLASH_Read(_ERASE_CODE_ADDR+STM32_FLASH_BASE, (uint16_t*)section_buf, ERASE_SECTOR_SIZE/2);//读出整个扇区的内容
	// code erase
	etext = section_buf;
	mdata = section_buf + 0x08;
	_code = section_buf + 0x08 + 0x10 + 0x20;
	printf("section_buf: %08X\r\n", section_buf);
	printf("etext: %08X\r\n", etext);
	printf("mdata: %08X\r\n", mdata);
	printf("_code: %08X\r\n", _code);
	//dprintf("_mdata_size: %08X\r\n", _mdata_size);
	for(i=0; i<0x08; i++) // erase .etext (0x08)
	{
		etext[i] = 0xFF;
	}
	for(i=0; i<0x10; i++) // copy .mdata (0x10)
	{
		etext[i] = mdata[i];
	}
	for(i=0; i<0x10; i++) // erase .mdata (0x10)
	{
		//mdata[i] = 0xFF;
	}
	for(i=0; i<0x03C8; i++) // erase ._code
	{
		_code[i] = 0xFF;
	}
	test_mtext=(mtext*)(section_buf+0x08+0x10);	
	printf("test_mtext:%08X\r\n", test_mtext);
	test_mtext->code = TestSection;
	encoding(test_mtext, mcu_uid);
	//printf("******** write flash! ********\r\n");
	printf("ID0:%08X\r\n", test_mtext->id[0]);
	printf("ID1:%08X\r\n", test_mtext->id[1]);
	printf("ID2:%08X\r\n", test_mtext->id[2]);
	printf("_erase_code\r\n");
	//_erase_code(section_buf);
	_erase_code(_ERASE_CODE_ADDR+STM32_FLASH_BASE, (u16*)section_buf, ERASE_SECTOR_SIZE/2);
}
//static char passwd1[]="******** passwd! ********\r\n";
//static char passwd2[]="******** write flash! ********\r\n";
__attribute__ ((section (".etext"))) __attribute__((used)) void passwd_code(void)  
{
#if 0
	mtext* _mtext=NULL;
	uint32_t uid[3];
	printf("p1\r\n");
	// flash passwd
	_mtext=(mtext*)section_buf;	
	uid[0] = mcu_uid[0];
	uid[1] = mcu_uid[1];
	uid[2] = mcu_uid[2];
	//_mtext=test_mtext;
#if 0
    _mtext->id[0] = uid[1]^uid[2]^UID0;
    _mtext->id[1] = uid[1]^UID0^UID1;
    _mtext->id[2] = uid[0]^uid[2]^UID2;
#else
    _mtext->id[0] = (uid[0]&uid[1])|((~uid[0])&uid[2]);
    _mtext->id[1] = (uid[0]&uid[2])|(uid[1]&(~uid[2]));
    _mtext->id[2] = uid[0]^uid[1]^uid[2];
	//_mtext->sign[0]  = _mtext->id[0]^(uid[1]|(~UID2));
	//_mtext->sign[0]  = _mtext->sign[0]+_mtext->id[1]+uid[0];
	_mtext->sign[0]  = uid[0]^(uid[1]|(~UID2));
	_mtext->sign[1]  = (uid[0]>>16) | (uid[1]&0XFFFF0000);
	_mtext->sign[2]  = ((uid[0]<<16)&0XFFFF0000) | (uid[1]&0X0000FFFF);
	_mtext->sign[3]  = uid[2];
#endif
	//_mtext->code = TestSection;
	printf("p2\r\n");
	_erase_code(section_buf);
	return 1;
#else // 最终使用这种方式以较少该段的大小
	_passwd_code();
	//return 1;
#endif
}
#else
__attribute__ ((section (".etext"))) __attribute__((used)) int passwd_code(void)  
{
	//mtext* _mtext=NULL;
	//uint8_t section_buf[SECTOR_SIZE] = {0};
	//uint8_t* etext=NULL;
	//uint8_t* mdata=NULL;
	uint32_t i=0;
	//uint32_t uid[3];
	//lprintf("******** passwd! ********\r\n");
	// read UID
	//uid[0]=*(vu32*)(0x1ffff7e8);
	//uid[1]=*(vu32*)(0x1ffff7ec);
	//uid[2]=*(vu32*)(0x1ffff7f0);
	//STMFLASH_Read(_mtext_start+STM32_FLASH_BASE, (uint16_t*)section_buf, SECTOR_SIZE/2);//读出整个扇区的内容
	// flash passwd
	//_mtext=(mtext*)section_buf;	
#if 0
    _mtext->id[0] = uid[1]^uid[2]^UID0;
    _mtext->id[1] = uid[1]^UID0^UID1;
    _mtext->id[2] = uid[0]^uid[2]^UID2;
#else
    _mtext->id[0] = (uid[0]&uid[1])|((~uid[0])&uid[2]);
    _mtext->id[1] = (uid[0]&uid[2])|(uid[1]&(~uid[2]));
    _mtext->id[2] = uid[0]^uid[1]^uid[2];
	_mtext->sign  = _mtext->id[0]^(uid[1]|(~UID2));
	_mtext->sign  = _mtext->sign+_mtext->id[1]+uid[0];
#endif
	_mtext->code = TestSection;
	// code erase
	//etext = section_buf + _mtext_size;
	//mdata = section_buf + _mtext_size + _etext_size;
	//lprintf("etext: %08X\r\n", etext);
	//lprintf("mdata: %08X\r\n", mdata);
	//lprintf("_mdata_size: %08X\r\n", _mdata_size);
	for(i=0; i<_etext_size; i++)
	{
		etext[i] = mdata[i];
		if(i<_mdata_size)mdata[i] = 0xFF;
	}
	//lprintf("******** write flash! ********\r\n");
	_erase_code(section_buf);
	return 1;
}
#endif
//static int _erase_code(uint8_t section[])  
static void _erase_code(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)  
{
	//printf("_erase_code\r\n");
	//STMFLASH_Write_zone(_mtext_start+STM32_FLASH_BASE, (u16*)section, ERASE_SECTOR_SIZE/2);
	STMFLASH_Write_zone(WriteAddr, pBuffer, NumToWrite);
	code_entry();
	//return 1;
}

static void erase_init(void)
{
	mtext _mtext;
	dprintf("erase_init\r\n");
	//dprintf("TestSection: %08X\r\n", TestSection);
	//dprintf("passwd_code: %08X\r\n", passwd_code);
	//获取CPU唯一ID
	_mtext.id[0]=*(vu32*)(0x1ffff7e8);
	_mtext.id[1]=*(vu32*)(0x1ffff7ec);
	_mtext.id[2]=*(vu32*)(0x1ffff7f0);
	dprintf("ID0:%08X\r\n", _mtext.id[0]);
	dprintf("ID1:%08X\r\n", _mtext.id[1]);
	dprintf("ID2:%08X\r\n", _mtext.id[2]);
	STMFLASH_Read(_ERASE_CODE_ADDR+STM32_FLASH_BASE+0x18, (uint16_t*)&_mtext, sizeof(mtext)/2);//读出整个扇区的内容
	//STMFLASH_Read(_passwd_start+STM32_FLASH_BASE, (uint16_t*)&_mtext, sizeof(mtext)/2);//读出整个扇区的内容
	printf("ID0:%08X\r\n", _mtext.id[0]);
	printf("ID1:%08X\r\n", _mtext.id[1]);
	printf("ID2:%08X\r\n", _mtext.id[2]);
	printf("F_SIZE:%04X\r\n", (*(vu32*)(0x1ffff7e0))&0xFFFF);
}

static int erase_code(start_main start)
{
	code_entry = start;
	//_passwd_code();
	passwd_code();
	return 0;
}

static EraseCodeObj erase;
EraseCodeObj* NewEraseCode_Obj(void)
{
	erase.Init = erase_init;
	erase.Erase = erase_code;
	return &erase;
}
