#include "config.h"

#if defined(BOARD_AT9)
#include "led_at9.c" // at9
#elif defined (BOARD_AT10)
#include "led_at10.c" // at10
#elif defined (BOARD_CB88)
#include "led_cb88.c" // cb88
#elif defined (BOARD_CP630) 
#include "led_cp630.c" // cp630
#elif defined (BOARD_OPEDDEV)
#include "led_opendev.c" // opendev ¿ª·¢°å
#else
#error "Please define BOARD!"
#endif

