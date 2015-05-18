#include "defines.h"
#include "sys_timer.h"

#define SYSTEM_TIMER_BASE ((volatile uint32 *)0x20003000)

#define TIMER_CLO_REG 1
#define TIMER_CHI_REG 2

volatile uint64 get_timestamp(void)
{
  volatile uint32 *systemTimer = SYSTEM_TIMER_BASE;
  uint64 t;
  uint32 chi;
  uint32 clo;

  // カウンタの値を取得
  clo = systemTimer[TIMER_CLO_REG];
  chi = systemTimer[TIMER_CHI_REG];

  // 桁上りチェック
  if(chi != systemTimer[TIMER_CHI_REG]){
    // 桁上りが起こっているならchiとcloを更新する
    clo = systemTimer[TIMER_CLO_REG];
    chi = systemTimer[TIMER_CHI_REG];
  }

  // 64bitに組み直す
  t = chi;
  t = t << 32;
  t += clo;

  return t;
}

void delay_ms(uint32 delay)
{
uint64 alermTime;
  alermTime = get_timestamp() + delay * 1000;

  while(1){
    if(get_timestamp() >= alermTime){
      break;
    }
  }

  return;
}
