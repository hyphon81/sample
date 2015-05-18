#include "defines.h"
#include "mailbox.h"

#define MAILBOX_RECV   ((volatile uint32*)0x2000b880)
#define MAILBOX_STATUS ((volatile uint32*)0x2000b898)
#define MAILBOX_SEND   ((volatile uint32*)0x2000b8a0)

void mailbox_write(uint32 value, uint8 channel){
  volatile uint32 *status = MAILBOX_STATUS;
  volatile uint32 *send_mail_ptr = MAILBOX_SEND;

  if((value & 0xf) != 0){
    return;
  }
  if(channel > 15){
    return;
  }
  while(1){
    if((*status & 0x80000000) == 0){
      break;
    }
  }
  *send_mail_ptr = value | channel;

  return;
}

uint32 mailbox_read(uint8 channel)
{
  volatile uint32 *status = MAILBOX_STATUS;
  volatile uint32 *mail_recv_ptr = MAILBOX_RECV;
  uint32 count = 0;

  if(channel > 15){
    return E_INVALID_VAL;
  }

  while(1){
    while(1){
      if((*status & (uint32)0x40000000) == 0){
        break;
      }
      /* This is an arbritarily large number */
      if(count++ > (1<<25)){
        return E_COUNTUP_MAX;
      }
    }
    if((uint8)(*mail_recv_ptr & 0xf) == channel){
      break;
    }
  }

  return (*mail_recv_ptr & 0xfffffff0);
}
