// cpu.c -- memory access routines for the stm6502 virtual cpu.
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
#include <string.h>
#include "cpu.h"


// The real address of the 6502 core memory in the F4's address space.
// Possibly could be relocated to the DMA area?
unsigned char *rawmemory=0x20000000;


cpu_t* new_cpu()
{
  cpu_t* cpu = (cpu_t*)calloc(1, sizeof(cpu_t));
  cpu->mem = rawmemory;
  cpu->regs.sp = 255;
  return cpu;
}


inline void cpu_push_8(cpu_t* cpu, unsigned char val)
{
  cpu->mem[256 + cpu->regs.sp] = val;
  cpu->regs.sp--;
}


void cpu_push_16(cpu_t* cpu, unsigned short val)
{
  cpu_push_8(cpu, (val >> 8) & 255);
  cpu_push_8(cpu, val & 255);
}


inline unsigned char cpu_pop_8(cpu_t* cpu)
{
  cpu->regs.sp++;
  return cpu->mem[256 + cpu->regs.sp];
}


unsigned short cpu_pop_16(cpu_t* cpu)
{
  unsigned char lsb = cpu_pop_8(cpu);
  unsigned char msb = cpu_pop_8(cpu);

  return msb << 8 | lsb;
}


void cpu_nmi(cpu_t* cpu)
{
  cpu_push_16(cpu, cpu->regs.pc);
  cpu_push_8(cpu, cpu->regs.flags);

  cpu->regs.pc = cpu->mem[0xFFFA] | (cpu->mem[0xFFFB] << 8);
}


void cpu_rst(cpu_t* cpu)
{
  cpu_push_16(cpu, cpu->regs.pc);
  cpu_push_8(cpu, cpu->regs.flags);

  cpu->regs.pc = cpu->mem[0xFFFC] | (cpu->mem[0xFFFD] << 8);
}


void cpu_brk(cpu_t* cpu)
{
  cpu_push_16(cpu, cpu->regs.pc);
  cpu_push_8(cpu, cpu->regs.flags | FBRK);
  cpu->regs.pc = cpu->mem[0xFFFE] | (cpu->mem[0xFFFF] << 8);
}


void cpu_mmap(cpu_t* cpu, mmapseg_t* segment)
{
  mmapseg_t* new_segment = (mmapseg_t*)malloc(sizeof(mmapseg_t));

  memcpy(new_segment, segment, sizeof(mmapseg_t));

  new_segment->next = NULL;

  if(cpu->mmapped_chain_tail != NULL)
    cpu->mmapped_chain_tail->next = new_segment;

  if(cpu->mmapped_chain_head == NULL)
    cpu->mmapped_chain_head = new_segment;

  cpu->mmapped_chain_tail = new_segment;
}


unsigned char cpu_peek(cpu_t* cpu, unsigned short address)
{
  if (address >= 0xFFF0)
    for(mmapseg_t* node = cpu->mmapped_chain_head; node != NULL; node = node->next)
      if(node->address <= address && node->address + node->length > address)
	return node->get(cpu, address);
  return cpu->mem[address];
}


void cpu_poke(cpu_t* cpu, unsigned short address, unsigned char val)
{
  if (address >= 0xFFF0)
    for(mmapseg_t* node = cpu->mmapped_chain_head; node != NULL; node = node->next)
      if(node->address <= address && node->address + node->length > address)
	return node->set(cpu, address, val);
  cpu->mem[address] = val;
}


inline unsigned short cpu_peek_16(cpu_t* cpu, unsigned short address)
{
   // calculating the 16bit address values won't involve the mmaps directly,
   // so let's avoid cpu_peek()..
  return cpu->mem[address] | (cpu->mem[address+1] << 8);
}
