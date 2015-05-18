#include "defines.h"
#include "lib.h"
#include "interrupt.h"
#include "timer.h"

#define ARM_TIMER_BASE ((volatile uint32 *)0x2000b400)

#define TIMER_LOAD_REG        0
#define TIMER_VAL_REG         1
#define TIMER_CTRL_REG        2
#define TIMER_IRQ_CLR_REG     3
#define TIMER_RAWIRQ_REG      4
#define TIMER_MASKEDIRQ_REG   5
#define TIMER_RELOAD_REG      6
#define TIMER_PREDIVIDER_REG  7
#define TIMER_FREECOUNTER_REG 8

#define TMR_EN     (0x00000080)
#define TMR_INT_EN (0x00000020)

extern void enable_IRQ(void);

// Timer IRQ function
void (*timer_IRQ_func)(void);

void enable_timer_intr(void)
{
  volatile uint32 *timer = ARM_TIMER_BASE;

  enable_timer_IRQ();
  timer[TIMER_CTRL_REG] |= TMR_INT_EN;
}

uint32 show_timer_intr(void)
{
  volatile uint32 *timer = ARM_TIMER_BASE;

  return timer[TIMER_CTRL_REG];
}

uint32 show_timer_val(void)
{
  volatile uint32 *timer = ARM_TIMER_BASE;

  return timer[TIMER_VAL_REG];
}

void disable_timer_intr(void)
{
  volatile uint32 *timer = ARM_TIMER_BASE;
  timer[TIMER_CTRL_REG] &= ~TMR_INT_EN;
}

void start_timer(void)
{
  volatile uint32 *timer = ARM_TIMER_BASE;

  // Timer start
  timer[TIMER_CTRL_REG] |= TMR_EN;
}

void stop_timer(void)
{
  volatile uint32 *timer = ARM_TIMER_BASE;

  // Timer stop
  timer[TIMER_CTRL_REG] &= ~TMR_EN;
}

void set_timer_period(uint32 period){
  volatile uint32 *timer = ARM_TIMER_BASE;

  // period(us)
  timer[TIMER_RELOAD_REG] = period;
  timer[TIMER_LOAD_REG] = period;
}

uint32 get_timer_rawIRQ(void)
{
  volatile uint32 *timer = ARM_TIMER_BASE;
  return timer[TIMER_RAWIRQ_REG];
}

void clear_timer_flag(void)
{
  volatile uint32 *timer = ARM_TIMER_BASE;
  timer[TIMER_IRQ_CLR_REG] = 0;
}

uint32 set_timer_clock(uint32 clock){
  volatile uint32 *timer = ARM_TIMER_BASE;
  uint32 prediv;

  // 単位はHz
  if(clock > APB_CLOCK){
    // error
    return E_INVALID_VAL;
  }
  
  prediv = div_uint32(APB_CLOCK, clock - 1);

  // pre-divider設定
  timer[TIMER_PREDIVIDER_REG] = prediv;

  //putdval(timer[TIMER_PREDIVIDER_REG],0);
  
  return 0;
}

void init_timer(uint32 period){
  volatile uint32 *timer = ARM_TIMER_BASE;

  // timer clock を1MHzに設定
  set_timer_clock(1000000);

  // Timer 32bit
  timer[TIMER_CTRL_REG] |= 0x00000002;

  // period(us)
  stop_timer();
  // Timer_start(period);
  set_timer_period(period);
} 

void timer_attach_intr(void (*f)(void))
{
  volatile uint32 *timer = ARM_TIMER_BASE;

  // 割り込みフラグクリア
  timer[TIMER_IRQ_CLR_REG] = 0;
  // 関数ポインタセット
  timer_IRQ_func = f;

  // タイマー開始
  start_timer();

  // 割り込み有効
  enable_timer_intr();

  enable_IRQ();
}
