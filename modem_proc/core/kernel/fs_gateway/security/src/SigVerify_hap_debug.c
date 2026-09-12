#include <string.h>
#include <stdio.h>
#include "AEEstd.h"

#define HAP_BUF_SIZE 64
char last_hap_buf[HAP_BUF_SIZE];

typedef __builtin_va_list va_list;

void HAP_debug_v2(int level, const char* file, int line,
	const char* format, ...){

  va_list args;
  va_start(args, format);
  
  printf(format,args);
  va_end(args);
  memset(last_hap_buf, 0, HAP_BUF_SIZE);
  strlcpy(last_hap_buf, format, sizeof(last_hap_buf));
}

void HAP_debug(const char *msg, int level, const char *filename, int line){
	HAP_debug_v2(level, filename, line, msg);
}

int std_memsmove(void *dst, int dst_size, const void *src, int src_size){
	return (int) memmove(dst,src,src_size);
}

int std_memscpy(void *dst, int dst_size, const void *src, int src_size){
	return (int) memcpy(dst,src,src_size);
}

