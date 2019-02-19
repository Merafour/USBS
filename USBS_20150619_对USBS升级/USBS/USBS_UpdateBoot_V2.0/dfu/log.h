/*
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

//#ifdef DEBUG
//#define DLog(fmt, ...) fprintf(stderr, "%s [Line %d] " fmt, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);
//#else
//#define DLog(...)
//#endif
//#define DLog(fmt, ...) fprintf(stderr, "%s [Line %d] " fmt, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);
//printf("@%s +%d %s... [0][%s]\n", __FILE__, __LINE__, __func__, argv[0]);


//extern FILE *debug_fp;
//extern FILE *stdlog;


//#ifdef DEBUG_LOG
//#ifdef DEBUG_DLOG /* full debug info */
////#define Log(fmt, ...) DLog(fmt, ...)
////#define Log(fmt, ...) printf("@%s +%d %s " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
//#define Log(fmt, ...) fprintf(stdlog, "@%s +%d %s " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
//#else /* log info */
////#define Log(fmt, ...) printf("%s [Line %d] " fmt, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
////#define Log(fmt, ...) printf("[%s @%d] " fmt, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
////#define Log(fmt, ...) fprintf(debug_fp, "[%s @%d] " fmt, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); fflush(debug_fp);
//#define Log(fmt, ...) fprintf(stdlog, "[%s @%d] " fmt, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);
//#endif /* DEBUG_DLOG */
//#else
//#define Log(...)
//#endif /* DEBUG_LOG */

#define _DPRINTF
#undef _DPRINTF

#ifdef _DPRINTF
#define dprintf(fmt, ...) printf("@%s +%d %s " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define lprintf(fmt, ...) printf("@%s +%d %s " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define dprintf(...)
#define lprintf(fmt, ...) printf("[%s @%d] " fmt, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

//#define debug_fflush()  fflush(debug_fp)

//extern int __cdecl lprintf(const char * __restrict__ _Format,...);
//extern int __cdecl lfprintf(FILE * __restrict__ _File,const char * __restrict__ _Format,...);

extern int log_open(void);
extern void log_close(void);
extern void log_fflush(void);
extern void log_exit(int _Code);

#ifdef __cplusplus
}
#endif
#endif /* _DEBUG_H_ */
