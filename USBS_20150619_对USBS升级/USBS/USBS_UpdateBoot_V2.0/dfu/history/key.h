#ifndef __KEY_H
#define __KEY_H	 
//#include <stdint.h>
//#define KEY0_PRES	1		//KEY0  
//#define KEY1_PRES	2		//KEY1 
//#define WKUP_PRES	3		//WK_UP  
#define USB_PRES	3		//USB  


typedef struct {
	//IO初始化
	void (*Init)(void);
	//按键扫描函数
	unsigned char (*Scan)(unsigned char mode);
	unsigned char (*USB)(void);
}KeyObj;
		 				    
/**** 对象初始化 ****/
//extern KeyObj* NewAT9Key_Obj(void);
//extern KeyObj* NewCB88Key_Obj(void);
//extern KeyObj* NewOpendevKey_Obj(void);
extern KeyObj* NewKey_Obj(void);
				    
#endif
