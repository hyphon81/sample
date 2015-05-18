#ifndef __GPIO_H__
#define __GPIO_H__

#define INPUT_PULLDOWN  0b000
#define INPUT_PULLUP    0b001
#define GPIO_ALT0       0b100
#define GPIO_ALT1       0b101
#define GPIO_ALT2       0b110
#define GPIO_ALT3       0b111
#define GPIO_ALT4       0b011
#define GPIO_ALT5       0b010

#define TXD0_PIN        14
#define RXD0_PIN        15

#define OK_LED_PIN      16


uint32 set_gpio(uint32 pinNum,uint32 pinVal);

#endif
