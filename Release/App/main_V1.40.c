/******************** (C) COPYRIGHT 2015 merafour ********************
* Author             : 冷月追风@merafour.blog.163.com
* Version            : V1.0.0
* Date               : 2/26/2015
* Description        : 加密测试程序.
********************************************************************************
* merafour.blog.163.com
*******************************************************************************/
#include "delay.h"
#include "stm32f10x.h"
#include "usart.h"

#define BUF_LEN   32
// 40K
//#define VectTabOffset  0xA000
// 20K
#define VectTabOffset  0x5000

static const uint32_t crc32_tab[] =
{
  0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
  0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
  0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
  0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
  0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
  0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
  0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
  0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
  0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
  0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
  0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
  0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
  0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
  0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
  0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
  0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
  0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
  0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
  0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
  0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
  0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
  0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
  0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
  0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
  0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
  0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
  0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
  0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
  0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
  0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
  0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
  0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

static const uint32_t crc_support[4][4] = {
	//{0x1FC6392A , 0x220E1C1F , 0x4C83FB63 , 0x17194601}, // CB88
	//{0x6BE73B6A , 0xBA8E3843 , 0x97ED6DCB , 0xE2434645 } // AT9
	/** 将 CRC32经简单运算之后存储
	 * eg: printf("\t[%08X | %08X | %08X | %08X | %08X]\r\n", crc32[0]-1, crc32[1]-2, crc32[2]-3, crc32[3]-4, crc32[4]-5);
	 */
	{0x7516BC9C , 0x6F78390D , 0x116BC695 , 0x12073FDA }, // opendev
	{0xD737681C , 0xD2C36AC5 , 0x21B025CF , 0xD1326D5A }, // opendev V0.18
	{0x8D0BD7C9 , 0x730B35B5 , 0x0EEDB7A9 , 0x5219DF57 }, // USBS V1.20
	{0x7BDEF9C1 , 0x7C72C5A5 , 0x769BADD0 , 0xE41A1E8B }
};

static const uint32_t License = 0x990951ba;

/************************************************************************************************
 * Name: crc32part
 *
 * Description:
 *   Continue CRC calculation on a part of the buffer.
 *
 ************************************************************************************************/

__inline static uint32_t crc32part(const uint8_t *src, size_t len, uint32_t crc32val)
{
  size_t i;

  for (i = 0;  i < len;  i++)
    {
      crc32val = crc32_tab[(crc32val ^ src[i]) & 0xff] ^ (crc32val >> 8);
    }
  return crc32val;
}
__inline static  void calculate_fw_crc(uint32_t sum[])
{
#define APP_SIZE_MAX_1 0x2800 // 10K
#define APP_SIZE_MAX_2 0x3C00 // 15K
#define APP_SIZE_MAX_3 0x4800 // 18K
#define APP_SIZE_MAX_4 0x4C00 // 19K
#define APP_SIZE_MAX_5 0x5000 // 20K
//#define BLOCK_SIZE_MAX 0x1000 // 4K
#define APP_LOAD_ADDRESS 0x08000000
	// compute CRC of the current firmware
	// 1024 Byte
	uint32_t code[1024/4] = {0};
	//uint32_t sum = 0;
	unsigned int p = 0;
	uint32_t CodeCRC=0;

	// read code
	for (p = 0; p < 1024/4; p ++) {
		code[p] = *(uint32_t *)(p + APP_LOAD_ADDRESS);
	}
	for (p = 0x0C; p < (0x0C+8); p ++) {
		code[p] = 0xFFFFFFFF;
	}
	// CRC
//	for (p = 0; p < 1024; p += 4) {
//		uint32_t bytes = code[p];
//		CodeCRC = crc32part((uint8_t *)&bytes, sizeof(bytes), CodeCRC);
//	}
	CodeCRC = crc32part((uint8_t *)code, sizeof(code), CodeCRC);
	//int block=0;
	// 最后 1K有加密信息会导致校验值不同
	// 10 + 5 + 3 + 1 + 1
	*sum = CodeCRC;
	for (p = 1024; p < APP_SIZE_MAX_1; p += 4) {
		uint32_t bytes = *(uint32_t *)(p + APP_LOAD_ADDRESS);
		*sum = crc32part((uint8_t *)&bytes, sizeof(bytes), *sum);
	}
	sum++;
	*sum = CodeCRC;
	for (p = 1024; p < APP_SIZE_MAX_2; p += 4) {
		uint32_t bytes = *(uint32_t *)(p + APP_LOAD_ADDRESS);
		*sum = crc32part((uint8_t *)&bytes, sizeof(bytes), *sum);
	}
	sum++;
	*sum = CodeCRC;
	for (p = 1024; p < APP_SIZE_MAX_3; p += 4) {
		uint32_t bytes = *(uint32_t *)(p + APP_LOAD_ADDRESS);
		*sum = crc32part((uint8_t *)&bytes, sizeof(bytes), *sum);
	}
	sum++;
	*sum = CodeCRC;
	for (p = 1024; p < APP_SIZE_MAX_4; p += 4) {
		uint32_t bytes = *(uint32_t *)(p + APP_LOAD_ADDRESS);
		*sum = crc32part((uint8_t *)&bytes, sizeof(bytes), *sum);
	}
	sum++;
	*sum = CodeCRC;
	for (p = 1024; p < APP_SIZE_MAX_5; p += 4) {
		uint32_t bytes = *(uint32_t *)(p + APP_LOAD_ADDRESS);
		*sum = crc32part((uint8_t *)&bytes, sizeof(bytes), *sum);
	}
	//return sum;
}

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
__inline static uint16_t match_ReadHalfWord(uint32_t faddr)
{
	return *(vu16*)faddr; 
}
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
__inline static void match_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=match_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

// 信息加密,仅供 App使用,自身不对其进行校验
static void UserLicense(uint32_t flash[], uint32_t uid[3])
{
	// 使用用户 License进行签名
	//uint32_t *sign = (uint32_t *)(flash);
	uint32_t *sign = (flash);
    sign[0] = ((~uid[0])&uid[2]);
    sign[1] = (uid[1]&(~uid[2]));
    sign[2] = uid[0]^uid[1]^License;
	sign[3] = uid[0]^(sign[2]|(~License));
	sign[4] = uid[0]+uid[1]+uid[2];
	sign[5] = (sign[0]+sign[3])|(uid[1]&(~uid[2]));
	sign[6] = (sign[1]+sign[4])|(uid[1]&(~uid[2]));
	sign[7] = (sign[2]+sign[5])|(uid[1]&(~uid[2]));
}

 int main(void)
 {
	//mtext* _mtext=NULL;
	//mtext passwd;
	uint32_t* _mtext=NULL;
	uint32_t passwd[8]={0};
	uint32_t uid[3]={0};
	char buf[BUF_LEN]={0};
	int match=0;	
	u8 t;
	//u8 len;	
	u16 times=0; 
	//uint32_t addr=0;
	//char str[64] = {0};
	int Flag=0; // 通过该变量检测是否强行跳过了加密校验代码
	uint32_t crc32[5] = {0};
	const uint32_t *support = NULL;
	int crc_flag = 0;
 
 	NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectTabOffset);
	delay_init();	    	 //延时函数初始化	
	NVIC_Configuration();// 设置中断优先级分组
	uart_init(115200);	 //串口初始化为9600
	printf("\r\nApp start!\r\n");
	// 测试加密
	{
		// read UID
		uid[0]=*(vu32*)(0x1ffff7e8);
		uid[1]=*(vu32*)(0x1ffff7ec);
		uid[2]=*(vu32*)(0x1ffff7f0);
		// passwd
		UserLicense(passwd, uid);
		Flag = 1;
#if 1
		// read flash
		match_Read(0x0800000C, (uint16_t*)buf, BUF_LEN/2);
		_mtext = (uint32_t*)buf;
#else
		_mtext = (uint32_t*)0x0800000C;
#endif
		// match
		for(t=0; t<8; t++)
		{
			if(passwd[t] != _mtext[t]) match=1;
		}
		printf("bootloader:%s", (char*)0x080000F0);

		calculate_fw_crc(crc32); // crc32校验 bootloader
		// CRC校验,得到 5个CRC值,但实际只能使用4个, crc32[4]包含了加密信息,每个芯片都不一样
		//printf("CRC32:[%08X | %08X | %08X | %08X | %08X]\r\n", crc32[0], crc32[1], crc32[2], crc32[3], crc32[4]);
		crc_flag = 0;
		for(t=0; t<sizeof(crc_support)/sizeof(crc_support[0]); t++)
		{
			support = &(crc_support[t][0]);
			if((crc32[0]==(support[0]+1)) && (crc32[1]==(support[1]+1)) && (crc32[2]==(support[2]+1)) && (crc32[3]==(support[3]+1)))
			{
				crc_flag = 1;
			}
		}
	}
	if(0 != match)
	{
		printf("match error!\r\n");
		goto UN_PERMISSION; // 出错处理
	}
	printf("match OK!\r\n");
	if(1!=crc_flag)
	{
		printf("not support board!\r\n");
		goto UN_PERMISSION; // 出错处理
	}
	// 测试,用于获取 CRC32值
	printf("CRC32:[%08X | %08X | %08X | %08X | %08X]\r\n", crc32[0], crc32[1], crc32[2], crc32[3], crc32[4]);
	printf("support board!\r\n");
	if(0==Flag) // 被跳过加密校验
	{
		printf("match2 error!\r\n");
		goto UN_PERMISSION; // 出错处理
	}

loop:
	while(1)
	{
		times++;  
		if(times>50)
		{
			printf("\r\n USBS Test App\r\n");
			times = 0;
		}
		delay_ms(10);   
		//printf("\r\n test %d\r\n", times);
	}
// unPermission,没有权限
UN_PERMISSION:
	printf("You device not run!\r\n");
	printf("ERROR:\r\n");
	printf("\t[%08X | %08X | %08X | %08X | %08X]\r\n", uid[0], uid[1], uid[2], 0, 0);
	printf("\t[%08X | %08X | %08X | %08X | %08X]\r\n", _mtext[0]-1, _mtext[1]-2, _mtext[2]-3, 0, 0);
	//printf("\t[%08X | %08X | %08X | %08X | %08X]\r\n", crc32[0]-1, crc32[1]-1, crc32[2]-1, crc32[3]-1, crc32[4]-1);
	printf("\t[0x%08X ,0x%08X ,0x%08X ,0x%08X ,0x%08X]\r\n", crc32[0]-1, crc32[1]-1, crc32[2]-1, crc32[3]-1, crc32[4]-1);
	//support = &(crc_support[2][0]);
	//printf("\t[%08X | %08X | %08X | %08X ]\r\n", support[0], support[1], support[2], support[3]);
	//while(1);
	goto loop;
}


