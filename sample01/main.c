#include "defines.h"
#include "lib.h"
#include "gpio.h"
#include "sys_timer.h"
#include "framebuffer.h"
#include "uart.h"
#include "interrupt.h"
#include "timer.h"

void LEDblink(void){
  // LEDの状態保持用変数
  static uint32 led = 0;

  if(led){
    puts("LED: ON\r\n");
    set_gpio(OK_LED_PIN, INPUT_PULLDOWN);
  }else{
    puts("LED: OFF\r\n");
    set_gpio(OK_LED_PIN, INPUT_PULLUP);
  }
  // LEDの状態を反転
  led = !led;
}

void clear_bss(void)
{
  extern void *__bss_start;
  extern void *__bss_end;

  unsigned int *p;
  unsigned int *start = (unsigned int *)&__bss_start;
  unsigned int *end = (unsigned int *)&__bss_end;

  for(p = start;p < end; p++){
    *p = 0x00;
  }
}

int main(void)
{
  FrameBufferDescription *fbInfoAddr;
  uint8 *fbAddr;
  int x,y;
  uint16 colour;

  clear_bss();
  disable_all_IRQ();
  set_vector_table();
  init_uart(115200);
  set_uart_timeout(-1, -1);
  fbInfoAddr = init_frame_buffer(1024,768,16);
  /* Error route */
  if(fbInfoAddr == NULL){
    while(1){
      uart_puts("Error!\r\n");
      delay_ms(1000);
      set_gpio(OK_LED_PIN, INPUT_PULLDOWN);
      delay_ms(1000);
      set_gpio(OK_LED_PIN, INPUT_PULLUP);
    }    
  }
  while(1){
    fbAddr=fbInfoAddr->pointer;
    colour = 0x0000;
    for(y=0;y<div_uint32(768,3);y++){
      for(x=0;x<1024;x++){
        *fbAddr = colour &  0xff;
        fbAddr++;
        *fbAddr = (colour >> 8) & 0xff;
        fbAddr++;
      }
      if(colour < 0b0000000000011111){
        colour += (1 << 0);
      }
      uart_puts("blue colour = 0x"); putxval(colour,0); uart_puts("\r\n");
    }
    colour = 0x0000;
    for(y=0;y<div_uint32(768,3);y++){
      for(x=0;x<1024;x++){
        *fbAddr = colour &  0xff;
        fbAddr++;
        *fbAddr = (colour >> 8) & 0xff;
        fbAddr++;
      }
      if(colour < 0b0000011111100000){
        colour += (1 << 5);
      }
      uart_puts("green colour = 0x"); putxval(colour,0); uart_puts("\r\n");
    }
    colour = 0x0000;
    for(y=0;y<div_uint32(768,3);y++){
      for(x=0;x<1024;x++){
        *fbAddr = colour &  0xff;
        fbAddr++;
        *fbAddr = (colour >> 8) & 0xff;
        fbAddr++;
      }
      if(colour < 0b1111100000000000){
        colour += (1 << 11);
      }
      uart_puts("red colour = 0x"); putxval(colour,0); uart_puts("\r\n");
    }

    puts("Timer Raw IRQ before init: 0x");
    putxval(get_timer_rawIRQ(),0); puts("\r\n");
    init_timer(4000000);
    puts("init_timer() end\r\n");
    timer_attach_intr(LEDblink);
    puts("attach_intr() end\r\n");
    puts("Timer Raw IRQ after init: 0x");
    putxval(get_timer_rawIRQ(),0); puts("\r\n");

    puts("Timer IRQ flag after init: 0x");
    putxval(show_timer_IRQ(),0); puts("\r\n");

    puts("Timer intr flag after init: 0x");
    putxval(show_timer_intr(),0); puts("\r\n");

    //IRQ_handler();

    while(1);
    //while(1){
      //puts("Timer val: 0x");
      //putxval(show_timer_val(),0);
      //puts("  getmode() val: 0x");
      //putxval(getmode(),0);
      //puts("\r\n");
    //}
  }

  return 0;
}
