/*
 * This file [was] part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/* original code whittled-down to what I needed -cjb */


#include "config.h"

#ifdef USE_USART
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "usart.h"


void clock_setup(void)
{
  rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
  /* Enable GPIOD clock for LED & USARTs. */
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPAEN);

  /* Enable clocks for USART2. */
  rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_USART2EN);
}


void usart_setup(void)
{
  /* Setup USART2 parameters. */
  usart_set_baudrate(USART2, 115200);
  usart_set_databits(USART2, 8);
  usart_set_stopbits(USART2, USART_STOPBITS_1);
  usart_set_mode(USART2, USART_MODE_TX|USART_MODE_RX);
  usart_set_parity(USART2, USART_PARITY_NONE);
  usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

  /* Finally enable the USART. */
  usart_enable(USART2);
}


void gpio_setup(void)
{
  /* Setup GPIO pins for USART2 transmit. */
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2|GPIO3);

  /* Setup USART2 TX pin as alternate function. */
  gpio_set_af(GPIOA, GPIO_AF7, GPIO2|GPIO3);
}


void usart_putchar(int c)
{
  usart_send_blocking(USART2,c);
  if (c == '\n')
    usart_send_blocking(USART2,'\r');
}


int usart_getchar(void)
{
  return usart_recv_blocking(USART2);
}


void usart_putstr (char *str)
{
  char c;

  while ((c = *str++))
    usart_putchar(c);
}


/* the magic routine that gives us printf()... */
int _write(int file, char *ptr, int len)
{
  int i;

  if ((file == 1) || (file == 2))
    {
      for (i = 0; i < len; i++)
	usart_putchar(ptr[i]);
      return i;
    }
  errno = EIO;
  return -1;
}


void usart_init(void)
{
  rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
  clock_setup();
  gpio_setup();
  usart_setup();
}


#endif /* USE_USART */
