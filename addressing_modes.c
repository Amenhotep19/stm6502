// addressing_modes.c -- involving the various virtual cpu instruction
// addressing modes.
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

#include <stdio.h>
#include "addressing_modes.h"
#include "vm.h"
#include "cpu.h"


ADDRMODE(implied)
{
  return 0;
}


ADDRMODE(imm8)
{
  return vm_next_8(cpu);
}


ADDRMODE(imm16)
{
  return vm_next_16(cpu);
}


ADDRMODE(relative)
{
  signed char offset = (signed char)vm_next_8(cpu);
  return cpu->regs.pc + offset;
}


ADDRMODE(abs)
{
  return vm_next_16(cpu);
}


ADDRMODE(abs_deref)
{
  return cpu_peek(cpu, vm_next_16(cpu));
}


ADDRMODE(absx)
{
  return vm_next_16(cpu) + cpu->regs.x;
}


ADDRMODE(absx_deref)
{
  return cpu_peek(cpu, vm_next_16(cpu) + cpu->regs.x);
}


ADDRMODE(absy)
{
  return vm_next_16(cpu) + cpu->regs.y;
}


ADDRMODE(absy_deref)
{
  return cpu_peek(cpu, vm_next_16(cpu) + cpu->regs.y);
}


ADDRMODE(zp)
{
  return vm_next_8(cpu);
}


ADDRMODE(zp_deref)
{
  return cpu_peek(cpu, vm_next_8(cpu));
}


ADDRMODE(zpx)
{
  return (vm_next_8(cpu) + (cpu->regs.x & 255)) & 255;
}


ADDRMODE(zpx_deref)
{
  return cpu_peek(cpu, (vm_next_8(cpu) + (cpu->regs.x & 255)) & 255);
}


ADDRMODE(zpy)
{
  return (vm_next_8(cpu) + (cpu->regs.y & 255)) & 255;
}


ADDRMODE(zpy_deref)
{
  return cpu_peek(cpu, (vm_next_8(cpu) + (cpu->regs.y & 255)) & 255);
}


ADDRMODE(indirect)
{
  unsigned short dest = cpu_peek_16(cpu, vm_next_16(cpu));

  if ((dest & 255) == 0xFF)
    {
      printf("\n** Indirect JMP to address $%04x\n*** CPU halted\n", dest);
      while (1)
	;
    }
  return dest;
}


ADDRMODE(indy)
{
  return cpu_peek_16(cpu, vm_next_8(cpu)) + cpu->regs.y;
}


ADDRMODE(indy_deref)
{
  return cpu_peek(cpu, cpu_peek_16(cpu, vm_next_8(cpu)) + cpu->regs.y);
}


ADDRMODE(indx)
{
  unsigned char fa;
  unsigned short ret;

  fa = vm_next_8(cpu);
  ret = cpu_peek_16(cpu, fa + cpu->regs.x);
  //printf ("pc=%04X sta($%02X,X=%02X)=%04X A=%02X\n",
  //	  cpu->regs.pc,fa,cpu->regs.x,ret,cpu->regs.a);
  return ret;
}


ADDRMODE(indx_deref)
{
  unsigned char fa;
  cpu_t *ret;
  unsigned short addr;

  fa = vm_next_8(cpu);
  addr = cpu_peek_16(cpu, fa + cpu->regs.x);
  ret = cpu_peek_16(cpu, addr);
  //printf ("pc=%04X ld($%02X,X=%02X)=%04X\n",cpu->regs.pc,fa,cpu->regs.x,addr);
  return ret;
}

