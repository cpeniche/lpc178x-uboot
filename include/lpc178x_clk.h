/*
 * lpc178x_clk.h
 *
 *  Created on: Mar 6, 2018
 *      Author: linux
 */

#ifndef _LPC178X_CLK_H_
#define _LPC178X_CLK_H_

#define MAX_PLL_OUT  320000000
#define MIN_PLL_OUT  156000000

enum {
    PCLCD,
    PCTIM0,
    PCTIM1,
    PCUART0,
    PCUART1,
    PCPWM0,
    PCPWM1,
    PCI2C0,
    PCUART4,
    PCRTC,
    PCSSP1,
    PCEMC,
    PCADC,
    PCCAN1,
    PCCAN2,
    PCGPIO,
    PCSPIFI,
    PCMCPWM,
    PCQEI,
    PCI2C1,
    PCSSP2,
    PCSSP0,
    PCTIM2,
    PCTIM3,
    PCUART2,
    PCUART3,
    PCI2C2,
    PCI2S,
    PCSDC,
    PCGPDMA,
    PCENET,
    PCUSB,
};

struct lpc178x_clk_regs{

  u32 pll0con;
  u32 pll0cfg;
  u32 pll0stat;
  u32 pll0feed;
  u32 pll1con;
  u32 pll1cfg;
  u32 pll1stat;
  u32 pll1feed;
  u32 rsv[15];
  u32 emcclksel;
  u32 cclksel;
  u32 usbclksel;
  u32 clksrcsel;
};

struct lpc178x_clk_info {
  u8 pll0msel_value;
  u8 pll0psel_selbits;
  u8 pll1msel_value;
  u8 pll1psel_selbits;

};

#endif /* _LPC178X_CLK_H_ */
