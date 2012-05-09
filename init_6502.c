// init_6502.c -- initalize the memory-mapped hooks for the virtual cpu
// Copyright (C) 2012 Chris J. Baird <cjb@brushtail.apana.org.au>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>

// derived from unlicenced/BSD-2clause code by Charlie Somerville
// https://github.com/charliesome/6502


#include "config.h"
#include "usart.h"
#include "init_6502.h"


static void writeport_set(cpu_t* cpu, unsigned short addr, unsigned char val);
static unsigned char readport_get(cpu_t* cpu, unsigned short addr);
static unsigned char readport_checkstatus(cpu_t* cpu, unsigned short addr);


void init_6502(cpu_t* cpu)
{
  mmapseg_t mmap;

  mmap.address = 0xFFF0;
  mmap.length = 1;
  mmap.set = writeport_set;
  mmap.get = readport_get;
  cpu_mmap(cpu, &mmap);

  mmap.address = 0xFFF1;
  mmap.get = readport_checkstatus;
  cpu_mmap(cpu, &mmap);

  //TODO: A ctrl-C test (check if character incoming, and if it's Ctrl-c)
}


static void writeport_set(cpu_t* cpu, unsigned short addr, unsigned char val)
{
#if defined(USE_USART)
  usart_putchar(val);
#endif
#if defined(USE_USB_CDCACM)
  usb_putchar(val);
#endif
  if (val == 13)		/* this'll get in the way of xmodem.. */
    usart_putchar(10);
}


static unsigned char readport_get(cpu_t* cpu, unsigned short addr)
{
  int c;

#ifdef USE_USART
  c = usart_getchar();
#endif
#ifdef USE_USB_CDCACM
  c = usb_getchar();
#endif
  // translate unix line feed to carriage return - preferred by 6502 computers
  return (unsigned char)(c == 10 ? 13 : c);
}


static unsigned char readport_checkstatus(cpu_t* cpu, unsigned short addr)
{
#ifdef USE_USART
#include <libopencm3/stm32/usart.h>
  return  ((USART_SR(USART2) & USART_SR_RXNE) > 0 ? 128 : 0);
#endif
}
