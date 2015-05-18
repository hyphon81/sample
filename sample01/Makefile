PREFIX  = /usr/local
ARCH    = arm-none-eabi
BINDIR  = $(PREFIX)/bin
ADDNAME = $(ARCH)-

AR      = $(BINDIR)/$(ADDNAME)ar
AS      = $(BINDIR)/$(ADDNAME)as
CC      = $(BINDIR)/$(ADDNAME)gcc
LD      = $(BINDIR)/$(ADDNAME)ld
NM      = $(BINDIR)/$(ADDNAME)nm
OBJCOPY = $(BINDIR)/$(ADDNAME)objcopy
OBJDUMP = $(BINDIR)/$(ADDNAME)objdump
RANLIB  = $(BINDIR)/$(ADDNAME)ranlib
STRIP   = $(BINDIR)/$(ADDNAME)strip

OBJS    = startup.o main.o intr.o interrupt.o timer.o
OBJS   += mailbox.o framebuffer.o gpio.o sys_timer.o uart.o
OBJS   += lib.o

TARGET  = kernel

CFLAGS  = -Wall -mfpu=vfp -mfloat-abi=hard -nostartfiles -nostdinc -nostdlib -fno-builtin
CFLAGS += -march=armv6zk -mtune=arm1176jzf-s -mlittle-endian
CFLAGS += -Os
CFLAGS += -I.
#CFLAGS += -g
#CFLAGS += -DRPI2

LFLAGS  = -static -T ldr.lds -L.

.SUFFIXES: .c .o
.SUFFIXES: .S .o

all:		$(TARGET).img

$(TARGET).elf:	$(OBJS)
		$(CC) $(OBJS) -o $(TARGET).elf $(CFLAGS) $(LFLAGS)
		$(STRIP) $(TARGET).elf

.c.o:		$<
		$(CC) -c $(CFLAGS) $<

.S.o:		$<
		$(CC) -c $(CFLAGS) $<

$(TARGET).img : $(TARGET).elf
		$(OBJCOPY) $(TARGET).elf -O binary $(TARGET).img

clean         : 
		rm -f $(OBJS) $(TARGET).elf $(TARGET).img
