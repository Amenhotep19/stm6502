// vm.c -- dispatch the virtual cpu instructions
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
#include <stdlib.h>
#include "cpu.h"
#include "vm.h"
#include "opcodes.h"
#include "instructions.h"
#include "addressing_modes.h"


inline unsigned char vm_next_8(cpu_t* cpu)
{
  return cpu_peek(cpu, cpu->regs.pc++);
}


inline unsigned short vm_next_16(cpu_t* cpu)
{
  unsigned char lsb = vm_next_8(cpu);
  unsigned char msb = vm_next_8(cpu);
  return msb << 8 | lsb;
}


void vm_step(cpu_t* cpu)
{
  unsigned short pc = cpu->regs.pc;
  unsigned char opcode = vm_next_8(cpu);
  ins_t instruction = (ins_t)opcodes[opcode * 2];
  addrmode_t addressing_mode = (addrmode_t)opcodes[opcode * 2 + 1];

  if (instruction == NULL)
    {
      printf("\n** Illegal opcode: $%X at address $%X\n*** CPU halted\n",
	     opcode, pc);
      while (1)
	;
    }

  instruction(cpu, addressing_mode(cpu));
}
