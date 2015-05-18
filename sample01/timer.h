#ifndef __TIMER_H__
#define __TIMER_H__

uint32 get_timer_rawIRQ(void);
void clear_timer_flag(void);
void enable_timer_intr(void);
void start_timer(void);
void init_timer(uint32 period);
void timer_attach_intr(void (*f)(void));

uint32 show_timer_intr(void);
uint32 show_timer_val(void);

// apb_clock = 250MHz?
#define APB_CLOCK       (250000000)

#endif /* __TIMER_H__ */
