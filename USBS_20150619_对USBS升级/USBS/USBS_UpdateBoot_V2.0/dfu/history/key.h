#ifndef __KEY_H
#define __KEY_H	 
//#include <stdint.h>
//#define KEY0_PRES	1		//KEY0  
//#define KEY1_PRES	2		//KEY1 
//#define WKUP_PRES	3		//WK_UP  
#define USB_PRES	3		//USB  


typedef struct {
	//IO��ʼ��
	void (*Init)(void);
	//����ɨ�躯��
	unsigned char (*Scan)(unsigned char mode);
	unsigned char (*USB)(void);
}KeyObj;
		 				    
/**** �����ʼ�� ****/
//extern KeyObj* NewAT9Key_Obj(void);
//extern KeyObj* NewCB88Key_Obj(void);
//extern KeyObj* NewOpendevKey_Obj(void);
extern KeyObj* NewKey_Obj(void);
				    
#endif
