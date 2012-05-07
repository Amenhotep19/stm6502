// vm.h -- what the rest of the code should know about vm.c
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


#ifndef VM_H
#define VM_H

#include "cpu.h"


void vm_step(cpu_t* cpu);
unsigned char vm_next_8(cpu_t* cpu);
unsigned short vm_next_16(cpu_t* cpu);


#endif	/* VM_H */
