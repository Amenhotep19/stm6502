// config.h -- compile-time feature options for when it gets that far..
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

#ifndef CONFIG_H
#define CONFIG_H

// select one of the following
#define USE_USART 1
#undef USE_USB_CDCACM

// this might happen one day too..
#define CPU_6502
#undef CPU_65C02
#undef CPU_65SC02
#undef CPU_M740			/* Mitsubitshi 740, M50734, embedded controllers */
#undef CPU_65CJB02		/* "..." */

#endif	/* CONFIG_H */
