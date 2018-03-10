/*
 * lpc178x_timer.c
 *
 *  Created on: Mar 9, 2018
 *      Author: linux
 */


#include <common.h>
#include <dm.h>
#include <dm/ofnode.h>
#include <mapmem.h>
#include <asm/arch/timer.h>
#include <dt-structs.h>
#include <timer.h>
#include <asm/io.h>


DECLARE_GLOBAL_DATA_PTR;

#if CONFIG_IS_ENABLED(OF_PLATDATA)
struct lpc178x_timer_plat {
  struct dtd_lpc178x_timer dtd;
};
#endif

/* Driver private data. Contains timer id. Could be either 0 or 1. */
struct lpc178x_timer_priv {
  struct lpc178x_timer_reg *timer;
};

static inline u32 lpc178x_timer_get_curr_value(struct lpc178x_timer_reg *timer)
{
  u32 timebase;


  timebase = readl(&timer->tc);
  return timebase;
}

#if CONFIG_IS_ENABLED(BOOTSTAGE)
ulong timer_get_boot_us(void)
{
  uint64_t  ticks = 0;
  uint32_t  rate;
  uint64_t  us;
  int ret;

  ret = dm_timer_init();

  if (!ret) {
    /* The timer is available */
    rate = timer_get_rate(gd->timer);
    timer_get_count(gd->timer, &ticks);
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
  } else if (ret == -EAGAIN) {
    /* We have been called so early that the DM is not ready,... */
    ofnode node = offset_to_ofnode(-1);
    struct rk_timer *timer = NULL;

    /*
     * ... so we try to access the raw timer, if it is specified
     * via the tick-timer property in /chosen.
     */
    node = ofnode_get_chosen_node("tick-timer");
    if (!ofnode_valid(node)) {
      debug("%s: no /chosen/tick-timer\n", __func__);
      return 0;
    }

    timer = (struct rk_timer *)ofnode_get_addr(node);

    /* This timer is down-counting */
    ticks = ~0uLL - lpc178x_timer_get_curr_value(timer);
    if (ofnode_read_u32(node, "clock-frequency", &rate)) {
      debug("%s: could not read clock-frequency\n", __func__);
      return 0;
    }
#endif
  } else {
    return 0;
  }

  us = (ticks * 1000) / rate;
  return us;
}
#endif

static int lpc178x_timer_get_count(struct udevice *dev, u32 *count)
{
  struct lpc178x_timer_priv *priv = dev_get_priv(dev);
  u32 cntr = lpc178x_timer_get_curr_value(priv->timer);

  /* timers are down-counting */
  *count = ~0ull - cntr;
  return 0;
}

static int lpc178x_clk_ofdata_to_platdata(struct udevice *dev)
{
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
  struct lpc178x_timer_priv *priv = dev_get_priv(dev);

  priv->timer = dev_read_addr_ptr(dev);
  if (!priv->timer)
    return -ENOENT;
#endif

  return 0;
}

static int lpc178x_timer_start(struct udevice *dev)
{
  struct lpc178x_timer_priv *priv = dev_get_priv(dev);

#if 0
  const uint64_t reload_val = ~0uLL;
  const uint32_t reload_val_l = reload_val & 0xffffffff;
  const uint32_t reload_val_h = reload_val >> 32;

  /* don't reinit, if the timer is already running and set up */
  if ((readl(&priv->timer->timer_ctrl_reg) & 1) == 1 &&
      (readl(&priv->timer->timer_load_count0) == reload_val_l) &&
      (readl(&priv->timer->timer_load_count1) == reload_val_h))
    return 0;

  /* disable timer and reset all control */
  writel(0, &priv->timer->timer_ctrl_reg);
  /* write reload value */
  writel(reload_val_l, &priv->timer->timer_load_count0);
  writel(reload_val_h, &priv->timer->timer_load_count1);
#endif

  /* enable timer */
  writel(1, &priv->timer->tcr);

  return 0;
}

static int lpc178x_timer_probe(struct udevice *dev)
{


#if CONFIG_IS_ENABLED(OF_PLATDATA)
  struct timer_dev_priv *uc_priv = dev_get_uclass_priv(dev);
  struct lpc178x_timer_priv *priv = dev_get_priv(dev);
  struct lpc178x_timer_plat *plat = dev_get_platdata(dev);

  priv->timer = map_sysmem(plat->dtd.reg[0], plat->dtd.reg[1]);
  uc_priv->clock_rate = plat->dtd.clock_frequency;
#endif
  struct ofnode_phandle_args args;
  u32 pcon_regs;
  int err;

  err = dev_read_phandle_with_args(dev, "pcop", NULL, 0, 0,
             &args);
  if (err) {
    debug("%s: can't find syscon device (%d)\n", __func__,
          err);
    return err;
  }
  if (args.args_count != 2) {
      debug("Invaild args_count: %d\n", args.args_count);
      return -EINVAL;
  }
  /* enable clock for timer */
  &pcon_regs = (u32 *)ofnode_get_addr(args.node);
  setbits_le32(&pcon_regs, args[1]);

  return lpc178x_timer_start(dev);
}

static const struct timer_ops lpc178x_timer_ops = {
  .get_count = lpc178x_timer_get_count,
};

static const struct udevice_id lpc178x_timer_ids[] = {
  { .compatible = "lpc178x-timer" },
  {}
};

U_BOOT_DRIVER(lpc178x_timer) = {
  .name = "lpc178x_timer",
  .id = UCLASS_TIMER,
  .of_match = lpc178x_timer_ids,
  .probe = _timer_probe,
  .ops  = &lpc178x_timer_ops,
  .flags = DM_FLAG_PRE_RELOC,
  .priv_auto_alloc_size = sizeof(struct lpc178x_timer_priv),
#if CONFIG_IS_ENABLED(OF_PLATDATA)
  .platdata_auto_alloc_size = sizeof(struct lpc178x_timer_plat),
#endif
  .ofdata_to_platdata = lpc178x_clk_ofdata_to_platdata,
};
