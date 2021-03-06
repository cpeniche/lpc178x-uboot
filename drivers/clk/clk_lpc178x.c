/*
 * clk_lpc178x.c
 *
 *  Created on: Mar 6, 2018
 *      Author: linux
 */


#include <common.h>
#include <clk-uclass.h>
#include <dm.h>
#include <lpc178x_clk.h>
#include <asm/io.h>

//#include <dt-bindings/mfd/lpc178x-clk.h>

#define CLKSRC        BIT(0)
#define CCLKDIV_MASK  GENMASK(4, 0)
#define CCLKDIV_SHIFT 0

#define MSEL_MASK     GENMASK(4, 0)
#define MSEL_SHIFT    0
#define PSEL_MASK     GENMASK(6, 5)
#define PSEL_SHIFT    5

#define CCLKSEL       BIT(8)
#define USBSEL_MASK   GENMASK(9,8)
#define USBSEL_SHIFT  8

#define PLLE          BIT(0)
#define PLLE_STAT     BIT(8)



struct psel
{
  u8 value;
  u8 sel_bits;
};

struct psel psel_values[] =
{
  {.value=1,.sel_bits=0},
  {.value=2,.sel_bits=1},
  {.value=4,.sel_bits=2},
  {.value=8,.sel_bits=3},
};


struct lpc178x_clk
{

  struct lpc178x_clk_regs *base;
  struct lpc178x_clk_info info;
  u32 pll_freq;
  u32 osc_freq;
  fdt_addr_t pconp;
};

static int calculate_pll_values( struct lpc178x_clk *priv);


int calculate_pll_values(struct lpc178x_clk *priv)
{
  int ret = 0;
  u8 idx;
  u32 fcco;
  u32 size = sizeof(psel_values)/sizeof(struct psel);

  priv->info.pll0msel_value = priv->pll_freq / priv->osc_freq;

  for(idx=0; idx<size; idx++)
  {
    fcco = priv->pll_freq * 2 * psel_values[idx].value;
    if (fcco >= MIN_PLL_OUT && fcco <= MAX_PLL_OUT)
    {
      priv->info.pll0psel_selbits = psel_values[idx].sel_bits;
      ret = 1;
      break;
    }

  }
  return ret;
}


ulong lpc178x_clk_get_rate(struct clk *clk)
{

  return 0;
}

ulong lpc178x_set_rate(struct clk *clk, ulong rate)
{

  return 0;
}

int lpc178x_clk_enable(struct clk *clk)
{
  struct lpc178x_clk *priv = dev_get_priv(clk->dev);

  debug("%s: clkid = %ld \n",__func__, clk->id);
  setbits_le32(priv->pconp, BIT(clk->id));

  return 0;
}

static int lpc178x_clk_of_xlate(struct clk *clk, struct ofnode_phandle_args *args)
{

debug("%s(clk=%p)\n", __func__, clk);

  if (args->args_count != 2) {
    debug("Invaild args_count: %d\n", args->args_count);
    return -EINVAL;
  }

  if (args->args_count)
    clk->id = args->args[1];
  else
    clk->id = 0;

  return 0;

}

static int lpc178x_clk_probe(struct udevice *dev)
{
  struct ofnode_phandle_args args;
  struct lpc178x_clk *priv = dev_get_priv(dev);
  struct udevice *osc_dev = NULL;
  fdt_addr_t addr;
  int err;
  u32 sysclk, cpuclk;

  debug("%s\n", __func__);

  addr = dev_read_addr(dev);
  if (addr == FDT_ADDR_T_NONE)
    return -EINVAL;

  priv->pconp = addr;

  /* look for main oscillator configuration  */
  err = uclass_get_device_by_name(UCLASS_CLK, "clk-hse",
            &osc_dev);

  if (err) {
    pr_err("Can't find fixed clock (%d)", err);
    return err;
  }


  /* look for pll phandle */
  err = dev_read_phandle_with_args(osc_dev, "pll", NULL, 0, 0,
                   &args);
  if (err) {
      pr_err("Can't find pll configuration (%d)", err);
      return err;
  }
  priv->pll_freq=args.args[1];


  priv->base = (struct lpc178x_clk_regs *)ofnode_get_addr(args.node);

/* look for system clock selection */
  err = dev_read_u32(osc_dev,"system-clock",&sysclk);
  if (err) {
      pr_err("Can't find system-clock (%d)",
             err);
      return err;
  }

  /* look for cpu clock selection */
  err = dev_read_u32(osc_dev,"cpu-clock",&cpuclk);
   if (err) {
       pr_err("Can't find cpu-clock (%d)",
              err);
       return err;
   }

  if(sysclk==1)
    setbits_le32(&priv->base->clksrcsel, CLKSRC);
  else
    clrbits_le32(&priv->base->clksrcsel, CLKSRC);

  if(cpuclk==1)
  {
    /* configure pll */
    err = dev_read_u32(dev,"clock-frequency",&priv->osc_freq);
    if (err) {
         pr_err("Can't find clock-frequency (%d)",err);
         return err;
     }

    if(!calculate_pll_values(priv))
    {
      pr_err("clock configuration error");
            return -EINVAL;
    }

    clrbits_le32(&priv->base->cclksel,CCLKSEL);
    clrsetbits_le32(&priv->base->usbclksel, USBSEL_MASK,
                    0x0 << USBSEL_SHIFT);

    /*turn off the pll */
    clrbits_le32(&priv->base->pll0con,PLLE);
    /* send the feed sequence */
    writel(0xAA, &priv->base->pll0feed);
    writel(0x55, &priv->base->pll0feed);

    /* write setup values */
    writel(priv->info.pll0msel_value | priv->info.pll0psel_selbits << PSEL_SHIFT,
            &priv->base->pll0cfg);

    setbits_le32(&priv->base->pll0con,PLLE);
    /* send the feed sequence */
    writel(0xAA, &priv->base->pll0feed);
    writel(0x55, &priv->base->pll0feed);
    clrsetbits_le32(&priv->base->usbclksel, CCLKDIV_MASK,
                        0x1 << CCLKDIV_SHIFT);

    while (!(readl(&priv->base->pll0stat) & PLLE_STAT));
    setbits_le32(&priv->base->cclksel,CCLKSEL);

  }

  return 0;
}

static struct clk_ops lpc178x_clk_ops = {
  .of_xlate = lpc178x_clk_of_xlate,
  .enable   = lpc178x_clk_enable,
  .get_rate = lpc178x_clk_get_rate,
  .set_rate = lpc178x_set_rate,
};



U_BOOT_DRIVER(lpc178x_clk) = {
  .name     = "lpc178x_clock",
  .id     = UCLASS_CLK,
  .ops      = &lpc178x_clk_ops,
  .probe      = lpc178x_clk_probe,
  .priv_auto_alloc_size = sizeof(struct lpc178x_clk),
  .flags      = DM_FLAG_PRE_RELOC,
};
