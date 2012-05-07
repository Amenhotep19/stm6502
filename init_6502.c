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


#include "usart.h"
#include "init_6502.h"


static void writeport_set(cpu_t* cpu, unsigned short addr, unsigned char val);
static unsigned char readport_get(cpu_t* cpu, unsigned short addr);


void init_6502(cpu_t* cpu)
{
  mmapseg_t mmap;

  mmap.address = 0xFFF0;
  mmap.length = 1;
  mmap.set = writeport_set;
  mmap.get = readport_get;
  cpu_mmap(cpu, &mmap);
}


static void writeport_set(cpu_t* cpu, unsigned short addr, unsigned char val)
{
  usart_putchar(val);
  if (val == 13)		/* this'll get in the way of xmodem.. */
    usart_putchar(10);
}


static unsigned char readport_get(cpu_t* cpu, unsigned short addr)
{
  int c = usart_getchar();
  // translate unix line feed to carriage return - commonly used by 6502 based computers
  return (unsigned char)(c == 10 ? 13 : c);
}
