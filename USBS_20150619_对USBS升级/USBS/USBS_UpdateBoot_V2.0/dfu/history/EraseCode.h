#ifndef _ERASE_H_
#define _ERASE_H_	 

#include <stdint.h>

typedef void (*start_main)(void);
//typedef int (*FuncSection)(void);
typedef void (*FuncSection)(void);

typedef struct {
	void (*Init)(void);
	int (*Erase)(start_main start);
}EraseCodeObj;
		 				    
/**** 对象初始化 ****/
//extern EraseCodeObj* NewEraseCode_Obj(void);
extern int match_first(void); // return 0;
extern int cram_entry(void);
		 				    
#endif //_ERASE_H_
