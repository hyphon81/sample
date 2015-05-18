#include "defines.h"
#include "gpio.h"
#include "sys_timer.h"
#include "uart.h"

#define UART0_BASE ((volatile uint32*)0x20201000)

#define UART0_DR_REG   0
#define UART0_FR_REG   6
#define UART0_IBRD_REG 9
#define UART0_FBRD_REG 10
#define UART0_LCRH_REG 11
#define UART0_CR_REG   12

static int64 write_to = -1;
static int64 read_to = -1;

void set_uart_timeout(int read_ms, int write_ms)
{
  // 読込みタイムアウト設定
  if(read_ms <= 0){
    read_to = -1;
  }else{
    read_to = read_ms * 1000;
  }

  // 書込みタイムアウト設定
  if(write_ms <= 0){
    write_to = -1;
  }else{
    write_to = read_ms * 1000;
  }
}


uint32 init_uart(int baudrate)
{
  volatile uint32 *uart = UART0_BASE;
  uint32 baudrate_temp[11]={300, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200 };
  uint32 ib_list[] = {625, 156,78,39,19,13,9,6,4,3,1};
  uint32 fb_list[] = {0,16,8,4,34,1,49,33,57,16,40};
  int flag=0;
  int i;

  // baudrate check
  for(i=0;i<11;i++){
    if(baudrate == baudrate_temp[i]){
      flag = 1;
      break;
    }
  }
  if(flag == 0)
    return E_INVALID_VAL;

  /* UART初期設定開始 */
  // UART無効化
  uart[UART0_CR_REG] = 0;

  // ポートの設定
  set_gpio(TXD0_PIN, INPUT_PULLDOWN);
  set_gpio(RXD0_PIN, INPUT_PULLDOWN);
  set_gpio(TXD0_PIN, GPIO_ALT0);
  set_gpio(RXD0_PIN, GPIO_ALT0);
  
  // ボーレートの設定
  uart[UART0_IBRD_REG] = ib_list[i];
  uart[UART0_FBRD_REG] = fb_list[i];;

  // LCRH
  // stick parity dis, 8bit, FIFO en, two stop bit no, odd parity, paritydis, break no
  uart[UART0_LCRH_REG] = 0x70;

  // CTS dis, RTS dis, OUT1-2=0, RTS dis, DTR dis, RXE en, TXE en, loop back dis, SIRLP=0, SIREN=0, UARTEN en
  uart[UART0_CR_REG] = 0x0301;
  /* UART初期設定終了 */

  return 0;
}

int uart_getc(void)
{
  volatile uint32 *uart = UART0_BASE;
  uint64 to = 0;
  int c;

  // タイムアウトが設定されている場合はセット
  if(read_to != -1){
    to = get_timestamp() + (uint64)read_to;
  }

  // 受信FIFOにデータが来るのを待つ
  while(uart[UART0_FR_REG] & (1 << 4)){
    if(read_to != -1){
      delay_ms(100);
      set_gpio(OK_LED_PIN, INPUT_PULLDOWN);
      delay_ms(100);
      set_gpio(OK_LED_PIN, INPUT_PULLUP);
      if(to <= get_timestamp()){
        return E_TIMEOUT;
      }
    }
  }
  c = (int)(uart[UART0_DR_REG] & 0x00ff);

  return c;  
}

char *uart_gets(char *str,int len)
{
  int i=1;
  int c;

  while(1){
    c = uart_getc();
    if(c == (E_TIMEOUT) ){
      return NULL;
    }
    *str = c;
    str++;
    i++;
    if(i >= len && c == '\n')
      break;
  }
  *str = '\0';
    
  return str;    
}

int uart_putc(int c)
{
  static volatile uint32 *uart = UART0_BASE;
  uint64 to = 0;

  // タイムアウトが設定されている場合はセット
  if(write_to != -1){
    to = get_timestamp() + (uint64)write_to;
  }

  // FIFOが空くまで待つ
  while(uart[UART0_FR_REG] & (0x01 << 5)){
    if(write_to != -1){
      if(to <= get_timestamp()){
        return E_TIMEOUT;
      }
    }
  }

  uart[UART0_DR_REG] = c;

  return 1;
}

int uart_puts(char *str)
{
  int i = 0;
  while(*str != '\0' && *str != '\n'){
    if(uart_putc(*(str++)) != (E_TIMEOUT)){
      i++;
    }
  }

  if(*str == '\n'){
    uart_putc('\n');
    i++;
  }
  return i;
}
