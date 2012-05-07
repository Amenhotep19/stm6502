// cpu.h -- cpu.c public declarations and defines.
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


#ifndef CPU_H
#define CPU_H

#define FCARRY	1		/* "C" */
#define FZERO	2		/* "Z" */
#define FIRQ	4		/* "I" */
#define FBCD	8		/* "D" */
#define FBRK	16		/* "B" */
#define FOFLOW	64		/* "V" */
#define FNEG	128		/* "N" */

#define GET_FLAG(state,flag)	((state)->regs.flags & (flag))
#define SET_FLAG(state,flag)	(state)->regs.flags |= (flag)
#define CLEAR_FLAG(state,flag)	(state)->regs.flags &= ~(flag);

#define FLAG_IF(state,flag,condition)	  \
  if(condition) { SET_FLAG(state,flag); } \
  else { CLEAR_FLAG(state,flag); }


typedef struct reg
{ unsigned short pc;
  unsigned char sp;
  unsigned char a;
  unsigned char x;
  unsigned char y;
  unsigned char flags;
} reg_t;

struct mmapseg;


typedef struct cpu
{ unsigned char* mem;
  reg_t regs;
  struct mmapseg* mmapped_chain_head;
  struct mmapseg* mmapped_chain_tail;
} cpu_t;


typedef struct mmapseg
{ unsigned short address;
  unsigned short length;
  unsigned char(*get)(cpu_t*, unsigned short);
  void(*set)(cpu_t*, unsigned short, unsigned char);
  struct mmapseg* next;
} mmapseg_t;


cpu_t* new_cpu();
void cpu_push_8(cpu_t* cpu, unsigned char val);
void cpu_push_16(cpu_t* cpu, unsigned short val);
unsigned char cpu_pop_8(cpu_t* cpu);
unsigned short cpu_pop_16(cpu_t* cpu);
void cpu_nmi(cpu_t* cpu);
void cpu_rst(cpu_t* cpu);
void cpu_brk(cpu_t* cpu);
void cpu_mmap(cpu_t* cpu, mmapseg_t* segment);
unsigned char cpu_peek(cpu_t* cpu, unsigned short address);
void cpu_poke(cpu_t* cpu, unsigned short address, unsigned char val);
unsigned short cpu_peek_16(cpu_t* cpu, unsigned short address);

#endif	/* CPU_H */
