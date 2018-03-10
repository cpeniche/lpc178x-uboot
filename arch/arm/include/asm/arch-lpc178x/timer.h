/*
 * timer.h
 *
 *  Created on: Mar 9, 2018
 *      Author: linux
 */

#ifndef _LPC178X_TIMER_H_
#define _LPC178X_TIMER_H_

struct lpc178x_timer_reg {
  u32 ir;
  u32 tcr;
  u32 tc;
  u32 pr;
  u32 pc;
  u32 mcr;
  u32 mr0;
  u32 mr1;
  u32 mr2;
  u32 mr3;
  u32 ccr;
  u32 cr0;
  u32 cr1;
  u32 gap0[4];
  u32 emr;
  u32 gap1[13];
  u32 ctcr;

};

void lpc178x_timer_init(void);
void lpc178x_udelay(unsigned int usec);

#endif /* _LPC178X_TIMER_H_ */
