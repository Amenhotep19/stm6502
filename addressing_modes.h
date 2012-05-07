// addressing_modes.h -- addressing_modes.c public declarations and defines.
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

#ifndef ADDRESSING_MODES_H
#define ADDRESSING_MODES_H

#include "cpu.h"

typedef unsigned short(*addrmode_t)(cpu_t*);

#define ADDRMODE(mode) unsigned short addrmode_##mode(cpu_t* cpu)

ADDRMODE(implied);		/* Accumulator, etc */
ADDRMODE(imm8);			/* Immediate */
ADDRMODE(imm16);		/* #$xxxx (jmp) */
ADDRMODE(relative);		/* branches */
ADDRMODE(abs);			/* Absolute (jsr) .. how is this different from imm16? */
ADDRMODE(abs_deref);		/* Absolute (value from) */
ADDRMODE(absx);			/* Absoulte,x WRITE */
ADDRMODE(absx_deref);		/* Absolute,x READ */
ADDRMODE(absy);			/* Absolute,y WRITE */
ADDRMODE(absy_deref);		/* Absolute,y READ ONLY */
ADDRMODE(zp);			/* Zero Page WRITE */
ADDRMODE(zp_deref);		/* Zero Page READ ONLY */
ADDRMODE(zpx);			/* Zero Page,x WRITED */
ADDRMODE(zpx_deref);		/* Zero Page,x READ ONLY */
ADDRMODE(zpy);			/* Zero Page,y WRITED */
ADDRMODE(zpy_deref);		/* Zero Page,y READ ONLY */
ADDRMODE(indirect);		/* jmp Indirect */
ADDRMODE(indy);			/* (Indirect),y WRITE */
ADDRMODE(indy_deref);		/* (Indirect),y READONLY */
ADDRMODE(indx);			/* (Indirect,x) WRITE */
ADDRMODE(indx_deref);		/* (Indirect,x) READONLY */

#endif	/* ADDRESSING_MODES_H */
