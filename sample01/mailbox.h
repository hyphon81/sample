#ifndef __MAILBOX_H_INCLUDE__
#define __MAILBOX_H_INCLUDE__

void mailbox_write(uint32 value, uint8 channel);
uint32 mailbox_read(uint8 channel);

#endif
