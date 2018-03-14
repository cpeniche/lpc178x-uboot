/*
 * gpio.h
 *
 *  Created on: Mar 6, 2018
 *      Author: linux
 */

#ifndef _LPC178X_GPIO_H_
#define _LPC178X_GPIO_H_

enum lcp178x_gpio_port {
  LPC178X_GPIO_PORT_0 = 0,
  LPC178X_GPIO_PORT_1,
  LPC178X_GPIO_PORT_2,
  LPC178X_GPIO_PORT_3,
  LPC178X_GPIO_PORT_4,
  LPC178X_GPIO_PORT_5,
};

enum lpc178x_gpio_pin {
  LPC178X_GPIO_PIN_0 = 0,
  LPC178X_GPIO_PIN_1,
  LPC178X_GPIO_PIN_2,
  LPC178X_GPIO_PIN_3,
  LPC178X_GPIO_PIN_4,
  LPC178X_GPIO_PIN_5,
  LPC178X_GPIO_PIN_6,
  LPC178X_GPIO_PIN_7,
  LPC178X_GPIO_PIN_8,
  LPC178X_GPIO_PIN_9,
  LPC178X_GPIO_PIN_10,
  LPC178X_GPIO_PIN_11,
  LPC178X_GPIO_PIN_12,
  LPC178X_GPIO_PIN_13,
  LPC178X_GPIO_PIN_14,
  LPC178X_GPIO_PIN_15
};


struct lpc178x_gpio_regs {

  u32 dir0;   /* port direction control register */
  u32 dummy0[3];
  u32 mask0;  /* mask register */
  u32 pin0;   /* port pin value register using FIOMASK */
  u32 set0;   /* port output set register using FIOMASK */
  u32 clr0;   /* port output clear register using FIOMASK */

  u32 dir1;   /* port direction control register */
  u32 dummy1[3];
  u32 mask1;  /* mask register */
  u32 pin1;   /* port pin value register using FIOMASK */
  u32 set1;   /* port output set register using FIOMASK */
  u32 clr1;   /* port output clear register using FIOMASK */

  u32 dir2;   /* port direction control register */
  u32 dummy2[3];
  u32 mask2;  /* mask register */
  u32 pin2;   /* port pin value register using FIOMASK */
  u32 set2;   /* port output set register using FIOMASK */
  u32 clr2;   /* port output clear register using FIOMASK */

  u32 dir3;   /* port direction control register */
  u32 dummy3[3];
  u32 mask3;  /* mask register */
  u32 pin3;   /* port pin value register using FIOMASK */
  u32 set3;   /* port output set register using FIOMASK */
  u32 clr3;   /* port output clear register using FIOMASK */

  u32 dir4;   /* port direction control register */
  u32 dummy4[3];
  u32 mask4;  /* mask register */
  u32 pin4;   /* port pin value register using FIOMASK */
  u32 set4;   /* port output set register using FIOMASK */
  u32 clr4;   /* port output clear register using FIOMASK */

  u32 dir5;   /* port direction control register */
  u32 dummy5[3];
  u32 mask5;  /* mask register */
  u32 pin5;   /* port pin value register using FIOMASK */
  u32 set5;   /* port output set register using FIOMASK */
  u32 clr5;   /* port output clear register using FIOMASK */

};

struct lpc178x_gpio_int_regs {

  u32 status;   /* gpio overall interrupt status */
  u32 statr0;   /* gpio interrupt status for rising edge for port 0 */
  u32 statf0;   /* gpio interrupt status for falling edge for port 0*/
  u32 clr0;     /* gpio interrupt celar */
  u32 enr0;     /* gpio interrupt enable for rising edge for port 0*/
  u32 enf0;     /* gpio interrupt enable for falling edge for port 0*/
  u32 statr2;
  u32 statf2;
  u32 clr2;
  u32 enr2;
  u32 enf2;

};


#endif /* _LPC178X_GPIO_H_ */
