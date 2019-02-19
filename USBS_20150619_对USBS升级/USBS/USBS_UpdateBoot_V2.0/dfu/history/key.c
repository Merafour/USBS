#include "config.h"

#if defined(BOARD_AT9)
#include "key_at9.c" // at9
#elif defined (BOARD_AT10)
#include "key_at10.c" // at10
#elif defined (BOARD_CB88)
#include "key_cb88.c" // cb88
#elif defined (BOARD_CP630) 
#include "key_cp630.c" // cp630
#elif defined (BOARD_OPEDDEV)
#include "key_opendev.c" // opendev ¿ª·¢°å
#else
#error "Please define BOARD!"
#endif

