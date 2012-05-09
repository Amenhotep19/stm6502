// main.c -- 6502 simulator startup and control main loop.
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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "config.h"
#include "cpu.h"
#include "vm.h"
#include "init_6502.h"
#include "usart.h"
#include "usb_cdcacm.h"
#include "rom.h"
#include "timer.h"


int debug_mode = 0;		/* protip: you can set this while running using gdb */
int send_rst = 0;		/* ..maybe this as well */
// int restore_rom = 0;
int go_slow = 0;		/* 1000000 typical for debugging */


void main(void)
{
  volatile unsigned int i;
  int pass=0;
  unsigned char *m, *p;

#ifdef OCM3_TIMER_EVER_BLOODY_WORKS
  timer_init();
#endif
#ifdef USE_USART
  usart_init();
#endif
#ifdef USE_USB_CDCACM
  usb_cdcacm_init();
#endif

  printf("\em\n\n*** STM32F4-Disco 6502 Simulator ***\n");

  cpu_t* cpu = new_cpu();
  init_6502(cpu);

  send_rst = 1;

  /* copy in the supermon 'rom' to the 6502 memory space */
  m = (unsigned char *)0x2000F800;
  p = (unsigned char *)&ROM_F800;
  while (p < (unsigned char *)&ROM_F800_END)
    *m++ = *p++;
  /* set up the 6502 RESET vector */
  m = (unsigned char *)0x2000FFFC;
  *m = 0x00;
  m = (unsigned char *)0x2000FFFD;
  *m = 0xF8;

  while (1)
    {
      if (send_rst)
	{
	  cpu_rst(cpu);
	  send_rst = 0;
	}

      vm_step(cpu);

      if(debug_mode)
	{
	  reg_t* r = &cpu->regs;

	  printf ("PC_After:$%04x A:%02x X:%02x Y:%02x SP:01%02x "
		  "C%c Z%c N%c V%c I%c D%c B%c\n",
		  r->pc & 65535, r->a & 255, r->x & 255, r->y & 255, r->sp & 255,
		  (r->flags & FCARRY ? 49 : 48),
		  (r->flags & FZERO  ? 49 : 48),
		  (r->flags & FNEG   ? 49 : 48),
		  (r->flags & FOFLOW ? 49 : 48),
		  (r->flags & FIRQ   ? 49 : 48),
		  (r->flags & FBCD   ? 49 : 48),
		  (r->flags & FBRK   ? 49 : 48));
	}

      if (go_slow)
	for (i = go_slow; i > 0; i--)
	  ;

#if 0
#ifdef OCM3_TIMER_WORKING
      // I /wanted/ to use an ISR for this.. but after 2 straight
      // days of failing to get any libopencm3 code to work, including
      // their provided examples, I gave up.. -cjb
      if (++pass > 16)
	{
	  usbd_poll();
	  pass=0;
	}
#endif
#endif
    }
}
