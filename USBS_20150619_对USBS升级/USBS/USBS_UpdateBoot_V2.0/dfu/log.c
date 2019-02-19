/*
 */

#include <stdio.h>
#include <stdlib.h>
#include "log.h"

//FILE *debug_fp=NULL;
FILE *stdlog=NULL;

int log_open(void)
{
    //fd = open("dfu.Log", O_WRONLY|O_CREAT|O_TRUNC);
    stdlog = fopen("update.Log", "wb");
    fprintf(stdlog,"Log.......\n\n");
    //fwrite("Log.......\n", strlen("Log.......\n"), 1, debug_fp);
//    fwrite("Radiolink5\n", strlen("Radiolink1\n"), 1, fp);
//    fwrite("Radiolink7\n", strlen("Radiolink1\n"), 1, fp);
//    fwrite("Radiolink8\n", strlen("Radiolink1\n"), 1, fp);
//    fwrite("Radiolink3\n", strlen("Radiolink1\n"), 1, fp);
//    fprintf(fp,"Cannotopeninputfile.\n");
//    fprintf(fp,"Len:%d\n", 100);
    return 0;
}
void log_fflush(void)
{
    fprintf(stdlog,"\nfflush!\n\n");
    fflush(stdlog);
    fclose(stdlog);
    stdlog = fopen("update.Log", "a");
}
void log_close(void)
{
    fprintf(stdlog,"\nLog end!\n");
    fflush(stdlog);
    fclose(stdlog);
}
void log_exit(int _Code)
{
    fprintf(stdlog,"\n--------exit(%d)--------\n\n", _Code);
    log_close();
    exit(_Code);
}

//int __cdecl _lprintf(const char * __restrict__ _Format,...)
//{
//    fprintf(stdlog, _Format);
//    //fprintf(stdlog, "[%s @%d] " , __PRETTY_FUNCTION__, __LINE__, _Format);
//}
int __cdecl lfprintf(FILE * __restrict__ _File,const char * __restrict__ _Format,...)
{
    (void)_File;
    fprintf(stdlog, _Format);
    return 0;
}

