#include "defines.h"
#include "lib.h"
#include "gpio.h"

#define MAX_PIN_NUM 53
#define MAX_PIN_VAL 7

#ifdef RPI2
#define GPIO_BASE ((volatile uint32*)0x3f200000)
#else
#define GPIO_BASE ((volatile uint32*)0x20200000)
#endif

#define GPIO_GPSET_OFFSET 7
#define GPIO_GPCLR_OFFSET 10

uint32 set_gpio(uint32 pinNum,uint32 pinVal)
{
  volatile uint32 *gpio = GPIO_BASE;
  uint32 gpio_gpfsel;
  uint32 gpio_pin;
  uint32 gpio_bank;
  uint32 gpio_bit;

  if(pinNum > MAX_PIN_NUM || pinNum < 0){
    return E_INVALID_VAL;
  }

  if(pinVal > MAX_PIN_VAL || pinVal < 0){
    return E_INVALID_VAL;
  }

  gpio_gpfsel = div_uint32(pinNum,10);
  gpio_pin    = mod_uint32(pinNum,10);
  gpio_bank   = div_uint32(pinNum,32);
  gpio_bit    = mod_uint32(pinNum,32);

  gpio[gpio_gpfsel * 4] |= (pinVal << (gpio_pin * 3));

  if(pinVal != 0){
    gpio[GPIO_GPSET_OFFSET + gpio_bank] = (1 << (gpio_bit));
  }else{
    gpio[GPIO_GPCLR_OFFSET + gpio_bank] = (1 << (gpio_bit));
  }

  return 0;
}
