#ifndef __LIB_H__
#define __LIB_H__

uint32 div_uint32(uint32 a, uint32 b);
uint32 mod_uint32(uint32 a, uint32 b);

void *memset(void *b, int c, uint32 len);
void *memcpy(void *dst, const void *src, uint32 len);
int memcmp(const void *b1, const void *b2, uint32 len);
int strlen(const char *str);
char *strcpy(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, uint32 len);
int putxval(uint32 value, uint32 column);
int putdval(uint32 value, uint32 column);
int puts(char *s);

#endif
