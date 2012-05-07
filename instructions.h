// instructions.h -- declare everything in instructions.c, even if it
// doesn't quite look like it..
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


#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

typedef void(*ins_t)(cpu_t*, unsigned short);

#define INS(name) void ins_##name(cpu_t* cpu, unsigned short param)

INS(adc);
INS(and);
INS(asl);
INS(asl_a);
INS(bcc);
INS(bcs);
INS(beq);
INS(bit);
INS(bmi);
INS(bne);
INS(bpl);
INS(brk);
INS(bvc);
INS(bvs);
INS(clc);
INS(cld);
INS(cli);
INS(clv);
INS(cmp);
INS(cpx);
INS(cpy);
INS(dec);
INS(dex);
INS(dey);
INS(eor);
INS(inc);
INS(inx);
INS(iny);
INS(jmp);
INS(jsr);
INS(lda);
INS(ldx);
INS(ldy);
INS(lsr);
INS(lsr_a);
INS(nop);
INS(ora);
INS(pha);
INS(php);
INS(pla);
INS(plp);
INS(rol);
INS(rol_a);
INS(ror);
INS(ror_a);
INS(rti);
INS(rts);
INS(sbc);
INS(sec);
INS(sed);
INS(sei);
INS(sta);
INS(stx);
INS(sty);
INS(tax);
INS(tay);
INS(tsx);
INS(txa);
INS(txs);
INS(tya);

#endif	/* INSTRUCTIONS_H */
