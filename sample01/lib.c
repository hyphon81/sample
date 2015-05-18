#include "defines.h"
#include "uart.h"
#include "lib.h"

/* %演算子を代替 */
uint32 mod_uint32(uint32 a, uint32 b)
{
  if(b == 0){
    return E_INVALID_VAL;
  }

  while(1){
    if(a < b){
      break;
    }
    a = a - b;
  }

  return a;
}           

/* /演算子を代替 */
uint32 div_uint32(uint32 a, uint32 b)
{
  uint32 count;

  if(b == 0){
    return E_INVALID_VAL;
  }

  count = 0;
  while(1){
    if(a < b){
      break;
    }
    a = a - b;
    count++;
  }

  return count;
}

/* メモリを特定パターンで埋める */
void *memset(void *b, int c, uint32 len)
{
  uint8 *p;
  for(p = b; len > 0; len--){
    *(p++) = c;
  }
  return b;
}

/* メモリのコピーを行う */
void *memcpy(void *dst, const void *src, uint32 len)
{
  uint8 *d = dst;
  const uint8 *s = src;
  for(; len >0; len--){
    *(d++) = *(s++);
  }
  return dst;
}

/* メモリ上のデータ比較を行う */
int memcmp(const void *b1, const void *b2, uint32 len)
{
  const uint8 *p1 = b1, *p2 = b2;
  for(; len > 0; len--){
    if(*p1 != *p2)
      return (*p1 > *p2) ? 1 : -1;
    p1++;
    p2++;
  }
  return 0;
}

/* 文字列の長さを返す */
int strlen(const char *str)
{
  int len;
  for(len =0; *str; str++, len++)
    ;
  return len;
}

/* 文字列のコピーを行う */
char *strcpy(char *dst, const char *src)
{
  char *d = dst;
  for(;; dst++, src++){
    *dst = *src;
    if(!*src)break;
  }
  return d;
}

/* 文字列の比較を行う */
int strcmp(const char *s1, const char *s2)
{
  while(*s1 || *s2){
    if(*s1 != *s2)
      return (*s1 > *s2) ? 1 : -1;
    s1++;
    s2++;
  }
  return 0;
}

/* 長さ制限有りで文字列の比較を行う */
int strncmp(const char *s1, const char *s2, uint32 len)
{
  while((*s1 || *s2 ) && (len > 0)){
    if(*s1 != *s2)
      return (*s1 > *s2) ? 1 : -1;
    s1++;
    s2++;
    len--;
  }
  return 0;
}

/* 数値の16進表示 */
int putxval(uint32 value, uint32 column)
{
  char buf[9];
  char *p;

  p = buf + sizeof(buf) - 1;
  *(p--) = '\0';

  if(!value && !column)
    column++;

  while(value || column){
    *(p--) = "0123456789abcdef"[value & 0xf];
    value >>= 4;
    if(column)column--;
  }

  uart_puts(p + 1);

  return 0;
}

/* 数値の10進表示 */
int putdval(uint32 value, uint32 column)
{
  char buf[9];
  char *p;

  p = buf + sizeof(buf) - 1;
  *(p--) = '\0';

  if(!value && !column)
    column++;

  while(value || column){
    *(p--) = "0123456789"[mod_uint32(value, 10)];
    value = div_uint32(value, 10);
    if(column)column--;
  }

  uart_puts(p + 1);

  return 0;
}

/* 文字列を表示 */
int puts(char *s)
{
  return uart_puts(s);
}
