/*
 * lpc178_periph.h
 *
 *  Created on: Mar 8, 2018
 *      Author: linux
 */

#ifndef _LPC178_PERIPH_H_
#define _LPC178_PERIPH_H_


/*
 * Peripherals required for pinmux configuration. List will
 * grow with support for more devices getting added.
 * Numbering based on interrupt table.
 *
 */
enum periph_id {
  PERIPH_ID_USART1 = 22,
};

enum periph_clock {
  TIMER2_CLOCK_CFG,
};


#endif /* _LPC178_PERIPH_H_ */
