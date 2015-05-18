#include "defines.h"
#include "gpio.h"
#include "timer.h"
#include "lib.h"

#ifdef RPI2
#define INTERRUPT_BASE ((volatile uint32*)0x3f00b200)
#else
#define INTERRUPT_BASE ((volatile uint32*)0x2000b200)
#endif

#define IRQ_BASIC_PENDING_REG  0
#define IRQ_PENDING1_REG       1
#define IRQ_PENDING2_REG       2
#define FIQ_CTRL_REG           3
#define ENABLE_IRQS1_REG       4
#define ENABLE_IRQS2_REG       5
#define ENABLE_BASIC_IRQS_REG  6
#define DISABLE_IRQS1_REG      7
#define DISABLE_IRQS2_REG      8
#define DISABLE_BASIC_IRQS_REG 9

extern void enable_IRQ(void);
extern void disable_IRQ(void);
extern uint32 getmode(void);
extern void (*timer_IRQ_func)(void);

void set_vector_table(void)
{
  extern void *_initial_vector_start;
  extern void *_initial_vector_end;
  volatile uint32 *vec = 0;
  volatile uint32 *p;
  volatile uint32 *s = (uint32 *)&_initial_vector_start;
  volatile uint32 *e = (uint32 *)&_initial_vector_end;

  puts("Vector table check\r\n");
  puts("Addr : Hex\r\n");
  for(p = s; p < e; p++){
    *vec = *p;
    puts("0x"); putxval((uint32)vec,0); puts(" : 0x"); putxval(*vec,0); puts("\r\n");
    vec++;
  }
}

void hangup(void)
{
  puts("hangup !!\r\n");
  puts("CPSR = 0x"); putxval(getmode(),0);puts("\r\n");
}

// IRQ割り込みハンドラ
void IRQ_handler(void)
{
  volatile uint32 *intr = INTERRUPT_BASE;

  puts("in IRQ_handler\r\n");

  // IRQ割り込みを停止
  disable_IRQ();

  puts("get IRQ in IRQ handler\r\n");
  puts("CPSR(in IRQ_handler) = 0x"); putxval(getmode(),0);puts("\r\n");

  // Basic IRQ pendingをチェック
  if((intr[IRQ_BASIC_PENDING_REG] & 0x01) != 0){
    // タイマー割り込み
    puts("Timer IRQ start\r\n");
    puts("Timer Raw IRQ before: 0x");
    putxval(get_timer_rawIRQ(),0); puts("\r\n");

    puts("irq_pending0 before: 0x");
    putxval(intr[IRQ_BASIC_PENDING_REG],0); puts("\r\n");

    // タイマー割り込み処理
    timer_IRQ_func();

    // 割り込みフラグクリア
    clear_timer_flag();

    // フラグがクリアされたかチェック
    puts("irq_pending0 after: 0x");
    putxval(intr[IRQ_BASIC_PENDING_REG],0); puts("\r\n");

    puts("Timer Raw IRQ after: 0x");
    putxval(get_timer_rawIRQ(),0); puts("\r\n");

    puts("Timer IRQ end\r\n");
  }
  // TODO: その他の割り込みも調べる

  // IRQ割り込みを許可
  enable_IRQ();

  return;
}

void disable_all_IRQ(void)
{
  volatile uint32 *intr = INTERRUPT_BASE;

  intr[DISABLE_BASIC_IRQS_REG] = 0xffffffff;
  intr[DISABLE_IRQS1_REG] = 0xffffffff;
  intr[DISABLE_IRQS2_REG] = 0xffffffff;
  intr[FIQ_CTRL_REG] = 0;

  // IRQ不許可
  disable_IRQ();
}

void enable_timer_IRQ(void)
{
  volatile uint32 *intr = INTERRUPT_BASE;

  intr[ENABLE_BASIC_IRQS_REG] |= 0x01;
}

uint32 show_timer_IRQ(void)
{
  volatile uint32 *intr = INTERRUPT_BASE;

  return intr[ENABLE_BASIC_IRQS_REG];
}
