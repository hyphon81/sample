#ifndef __INTR_H__
#define __INTR_H__

void set_vector_table(void);
void disable_all_IRQ(void);
void enable_timer_IRQ(void);

uint32 show_timer_IRQ(void);
 
#endif /* __INTR_H__ */
