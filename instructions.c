// instructions.c -- perform the virtual cpu instructions.
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

// [Distantly] derived from unlicenced/BSD-2clause code by Charlie
// Somerville https://github.com/charliesome/6502 ..except that it
// works.. :P


#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "instructions.h"


INS(adc)
{
  if(!GET_FLAG(cpu, FBCD))
    {
      unsigned short total;
      unsigned char c;
      int v;

      v = (char)cpu->regs.a + (char)param + (GET_FLAG(cpu, FCARRY) ? 1 : 0);
      total = cpu->regs.a + (unsigned char)param + (GET_FLAG(cpu, FCARRY) ? 1 : 0);
      c = total & 255;
      FLAG_IF(cpu, FNEG, c & 128);
      FLAG_IF(cpu, FOFLOW, ((c & 128) > 0) ^ (v < 0));
      FLAG_IF(cpu, FZERO, c == 0);
      FLAG_IF(cpu, FCARRY, (total & 256) > 0);
      cpu->regs.a = c;
    }
  else
    {
      printf("\nHALTING: bcd adc PC=%04X\n", cpu->regs.pc);
      while (1)
	;
    }
}


INS(and)
{
  cpu->regs.a = cpu->regs.a & param;

  FLAG_IF(cpu, FZERO, cpu->regs.a == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.a & 128);
}


INS(asl)
{
  unsigned char val = cpu_peek(cpu, param);
  FLAG_IF(cpu, FCARRY, val & 128);
  val <<= 1;
  FLAG_IF(cpu, FZERO, val == 0);
  FLAG_IF(cpu, FNEG, val & 128);
  cpu_poke(cpu, param, val);
}


INS(asl_a)
{
  FLAG_IF(cpu, FCARRY, cpu->regs.a & 128);
  cpu->regs.a <<= 1;
  FLAG_IF(cpu, FZERO, cpu->regs.a == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.a & 128);
}


INS(bcc)
{
  if(!GET_FLAG(cpu, FCARRY))
    cpu->regs.pc = param;
}


INS(bcs)
{
  if(GET_FLAG(cpu, FCARRY))
    cpu->regs.pc = param;
}


INS(beq)
{
  if(GET_FLAG(cpu, FZERO))
    cpu->regs.pc = param;
}


INS(bit)
{
  unsigned char result = cpu->regs.a & param;

  FLAG_IF(cpu, FZERO, result == 0);
  FLAG_IF(cpu, FOFLOW, param & 64);
  FLAG_IF(cpu, FNEG, param & 128);
  //  printf ("PC=%04x Z=%d V=%d N=%d\n",
  //	  cpu->regs.pc, GET_FLAG(cpu, FZERO),GET_FLAG(cpu, FOFLOW),GET_FLAG(cpu, FNEG));
}


INS(bmi)
{
  if(GET_FLAG(cpu, FNEG))
    cpu->regs.pc = param;
}


INS(bne)
{
  if(!GET_FLAG(cpu, FZERO))
    cpu->regs.pc = param;
}


INS(brk)
{
  cpu_brk(cpu);
}


INS(bpl)
{
  if(!GET_FLAG(cpu, FNEG))
    cpu->regs.pc = param;
}


INS(bvc)
{
  if(!GET_FLAG(cpu, FOFLOW))
    cpu->regs.pc = param;
}


INS(bvs)
{
  if(GET_FLAG(cpu, FOFLOW))
    cpu->regs.pc = param;
}


INS(clc)
{
  CLEAR_FLAG(cpu, FCARRY);
}


INS(cld)
{
  CLEAR_FLAG(cpu, FBCD);
}


INS(cli)
{
  CLEAR_FLAG(cpu, FIRQ);
}


INS(clv)
{
  CLEAR_FLAG(cpu, FOFLOW);
}


INS(cmp)
{
  FLAG_IF(cpu, FCARRY, cpu->regs.a >= param);
  FLAG_IF(cpu, FZERO, cpu->regs.a == param);
  FLAG_IF(cpu, FNEG, (cpu->regs.a - param) & 128);
  //FLAG_IF(cpu, FNEG, cpu->regs.a < param);
}


INS(cpx)
{
  FLAG_IF(cpu, FCARRY, cpu->regs.x >= param);
  FLAG_IF(cpu, FZERO, cpu->regs.x == param);
  FLAG_IF(cpu, FNEG, (cpu->regs.x - param) & 128);
  //FLAG_IF(cpu, FNEG, cpu->regs.x < param);
}


INS(cpy)
{
  FLAG_IF(cpu, FCARRY, cpu->regs.y >= param);
  FLAG_IF(cpu, FZERO, cpu->regs.y == param);
  FLAG_IF(cpu, FNEG, (cpu->regs.y - param) & 128);
  //FLAG_IF(cpu, FNEG, cpu->regs.y < param);
}


INS(dec)
{
  unsigned char val = cpu_peek(cpu, param) - 1;

  FLAG_IF(cpu, FZERO, val == 0);
  FLAG_IF(cpu, FNEG, val & 128);

  cpu_poke(cpu, param, val);
}


INS(dex)
{
  cpu->regs.x--;

  FLAG_IF(cpu, FZERO, cpu->regs.x == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.x & 128);
}


INS(dey)
{
  cpu->regs.y--;

  FLAG_IF(cpu, FZERO, cpu->regs.y == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.y & 128);
}


INS(eor)
{
  cpu->regs.a ^= (unsigned char)param;

  FLAG_IF(cpu, FZERO, cpu->regs.a == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.a & 128);
}


INS(inc)
{
  unsigned char n = cpu_peek(cpu, param) + 1;

  FLAG_IF(cpu, FZERO, n == 0);
  FLAG_IF(cpu, FNEG, n & 128);
  cpu_poke(cpu, param, n);
}


INS(inx)
{
  cpu->regs.x++;

  FLAG_IF(cpu, FZERO, cpu->regs.x == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.x & 128);
}


INS(iny)
{
  cpu->regs.y++;

  FLAG_IF(cpu, FZERO, cpu->regs.y == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.y & 128);
}


INS(jsr)
{
  cpu_push_16(cpu, cpu->regs.pc - 1);
  cpu->regs.pc = param;
}


INS(jmp)
{
  cpu->regs.pc = param;
}


INS(lda)
{
  cpu->regs.a = param & 255;

  FLAG_IF(cpu, FZERO, cpu->regs.a == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.a & 128);
}


INS(ldx)
{
  cpu->regs.x = param & 255;

  FLAG_IF(cpu, FZERO, cpu->regs.x == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.x & 128);
}


INS(ldy)
{
  cpu->regs.y = param & 255;

  FLAG_IF(cpu, FZERO, cpu->regs.y == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.y & 128);
}


INS(lsr)
{
  unsigned char val = cpu_peek(cpu, param);
  FLAG_IF(cpu, FCARRY, val & 1);
  val >>= 1;
  FLAG_IF(cpu, FZERO, val == 0);
  CLEAR_FLAG(cpu, FNEG);
  cpu_poke(cpu, param, val);
}


INS(lsr_a)
{
  FLAG_IF(cpu, FCARRY, cpu->regs.a & 1);
  cpu->regs.a >>= 1;
  FLAG_IF(cpu, FZERO, cpu->regs.a == 0);
  CLEAR_FLAG(cpu, FNEG);
}


INS(nop)
{
}


INS(ora)
{
  cpu->regs.a |= (unsigned char)param;

  FLAG_IF(cpu, FZERO, cpu->regs.a == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.a & 128);
}


INS(pha)
{
  cpu_push_8(cpu, cpu->regs.a);
}


INS(php)
{
  cpu_push_8(cpu, cpu->regs.flags);
}


INS(pla)
{
  cpu->regs.a = cpu_pop_8(cpu);
  FLAG_IF(cpu, FZERO, cpu->regs.a == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.a & 128);
}


INS(plp)
{
  cpu->regs.flags = cpu_pop_8(cpu);
}


INS(rol)
{
  unsigned char c;
  unsigned char val;

  c = (GET_FLAG(cpu, FCARRY) ? 1 : 0);
  val = cpu_peek(cpu, param);
  FLAG_IF(cpu, FCARRY, val & 128);
  val <<= 1;
  val += c;
  FLAG_IF(cpu, FZERO, val == 0);
  FLAG_IF(cpu, FNEG, val & 128);
  cpu_poke(cpu, param, val);
}


INS(rol_a)
{
  unsigned char c;

  c = (GET_FLAG(cpu, FCARRY) ? 1 : 0);
  FLAG_IF(cpu, FCARRY, cpu->regs.a & 128);
  cpu->regs.a <<= 1;
  cpu->regs.a += c;
  FLAG_IF(cpu, FZERO, cpu->regs.a == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.a & 128);
}


INS(ror)
{
  unsigned char c;
  unsigned char val;

  c = (GET_FLAG(cpu, FCARRY) ? 128 : 0);
  val = cpu_peek(cpu, param);
  FLAG_IF(cpu, FCARRY, val & 1);
  val >>= 1;
  val += c;
  FLAG_IF(cpu, FZERO, val == 0);
  FLAG_IF(cpu, FNEG, c);
  cpu_poke(cpu, param, val);
}


INS(ror_a)
{
  unsigned char c;

  c = (GET_FLAG(cpu, FCARRY) ? 128 : 0);
  FLAG_IF(cpu, FCARRY, cpu->regs.a & 1);
  cpu->regs.a >>= 1;
  cpu->regs.a += c;
  FLAG_IF(cpu, FZERO, cpu->regs.a == 0);
  FLAG_IF(cpu, FNEG, c);
}


INS(rti)
{
  cpu->regs.flags = cpu_pop_8(cpu);
  cpu->regs.pc = cpu_pop_16(cpu);
}


INS(rts)
{
  cpu->regs.pc = cpu_pop_16(cpu) + 1;
}


INS(sbc)
{

  if(!GET_FLAG(cpu, FBCD))
    {
      unsigned short total;
      unsigned char c;
      int v;

      /* 30+ attempts at getting this to work... ;_; -cjb */
      v = (char)cpu->regs.a - (char)param - (GET_FLAG(cpu, FCARRY) ? 0 : 1);
      total = cpu->regs.a - param - (GET_FLAG(cpu, FCARRY) ? 0 : 1);
      c = total & 255;
      FLAG_IF(cpu, FOFLOW, ((c & 128) > 0) ^ ((v & 256) != 0) );
      FLAG_IF(cpu, FCARRY, total < 256);
      FLAG_IF(cpu, FZERO, c == 0);
      FLAG_IF(cpu, FNEG, c & 128);
      cpu->regs.a = c;
    }
  else
    {
      printf("\nHALTING: bcd sbc PC=%04X\n", cpu->regs.pc);
      while (1)
	;
    }
}


INS(sec)
{
  SET_FLAG(cpu, FCARRY);
}


INS(sed)
{
  SET_FLAG(cpu, FBCD);
}


INS(sei)
{
  SET_FLAG(cpu, FIRQ);
}


INS(sta)
{
  cpu_poke(cpu, param, cpu->regs.a);
}


INS(stx)
{
  cpu_poke(cpu, param, cpu->regs.x);
}


INS(sty)
{
  cpu_poke(cpu, param, cpu->regs.y);
}


INS(tax)
{
  cpu->regs.x = cpu->regs.a;

  FLAG_IF(cpu, FZERO, cpu->regs.x == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.x & 128);
}


INS(tsx)
{
  cpu->regs.x = cpu->regs.sp;
  FLAG_IF(cpu, FZERO, cpu->regs.x == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.x & 128);
}


INS(txa)
{
  cpu->regs.a = cpu->regs.x;

  FLAG_IF(cpu, FZERO, cpu->regs.a == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.a & 128);
}


INS(txs)
{
  cpu->regs.sp = cpu->regs.x;
}


INS(tya)
{
  cpu->regs.a = cpu->regs.y;
  FLAG_IF(cpu, FZERO, cpu->regs.a == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.a & 128);
}


INS(tay)
{
  cpu->regs.y = cpu->regs.a;
  FLAG_IF(cpu, FZERO, cpu->regs.y == 0);
  FLAG_IF(cpu, FNEG, cpu->regs.y & 128);
}
