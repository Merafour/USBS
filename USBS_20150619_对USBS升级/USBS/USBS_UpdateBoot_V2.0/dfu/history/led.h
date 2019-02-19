#ifndef __LED_H
#define __LED_H	 

typedef struct {
	void (*Init)(void);
	//int (*LED_Blink)(int delay_ms);
	void (*LED1ON)(int on);
	void (*LED2ON)(int on);
}LEDObj;
		 				    
/**** 对象初始化 ****/
//extern LEDObj* NewAT9LED_Obj(void);
//extern LEDObj* NewCB88LED_Obj(void);
//extern LEDObj* NewOpendevLED_Obj(void);
extern LEDObj* NewLED_Obj(void);
		 				    
#endif
