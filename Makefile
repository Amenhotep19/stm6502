# Makefile -- project building without any of that newfangled IDE
# mucking about.
# Copyright (C) 2012 Chris J. Baird <cjb@brushtail.apana.org.au>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>


CC=arm-none-eabi-gcc
LDSCRIPT=linker.ld
INCS=/usr/local/arm-none-eabi/include
LIBS=/usr/local/arm-none-eabi/lib/thumb2

CFLAGS=-O3 -g -std=gnu99 -fno-common -Wall
ARCHFLAGS=-mcpu=cortex-m4 -mtune=cortex-m4 -mthumb -march=armv7-m

OBJS=addressing_modes.o cpu.o init_6502.o instructions.o main.o opcodes.o vm.o usart.o rom.o usb_cdcacm.o
PROG=stm6502

all:	$(OBJS)
	$(CC) $(ARCHFLAGS) -o $(PROG).elf $(OBJS) \
	 -lopencm3_stm32f4 -lc -lnosys \
	 -L$(LIBS) -L$(LIBS)/stm32/f4 \
	 -T$(LDSCRIPT) -nostartfiles -Wl,--gc-sections

clean:
	rm -f *.o *.elf *.d

.s.o:
	$(CC) $(CFLAGS) $(ARCHFLAGS) -o $@ -c $<

.c.o:
	$(CC) $(CFLAGS) $(ARCHFLAGS) \
	 -DSTM32F4 -I. -I$(INCS) \
	 -o $@ -c $<

timer:	timer.o
	$(CC) $(ARCHFLAGS) -o timer.elf timer.o \
	 -lopencm3_stm32f4 -lc -lnosys \
	 -L$(LIBS) -L$(LIBS)/stm32/f4 \
	 -T$(LDSCRIPT) -nostartfiles -Wl,--gc-sections

