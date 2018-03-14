/*
 * lpc_pcon.c
 *
 *  Created on: Mar 14, 2018
 *      Author: linux
 */

#include <common.h>
#include <dm.h>
#include <misc.h>
#include <dm/device-internal.h>
#include <dm/lists.h>

static int lpc_pconp_bind(struct udevice *dev)
{
  struct udevice *child;
  struct driver *drv;
  int ret;

  debug("%s(dev=%p)\n", __func__, dev);

  drv = lists_driver_lookup_name("lpc178x_clock");
  if (!drv) {
    debug("Cannot find driver '%s'\n", "lpc178x_clock");
    return -ENOENT;
  }

  ret = device_bind_driver(dev, "lpc178x_clock", dev->name ,
                            &child);
  return ret;
}

static const struct misc_ops lpc_pconp_ops = {
};

static const struct udevice_id lpc_pconp_ids[] = {
  {.compatible = "nxp,lpc-pconp"},
  { }
};

U_BOOT_DRIVER(lpc_pconp) = {
  .name   = "lpc-pconp",
  .id   = UCLASS_MISC,
  .of_match = lpc_pconp_ids,
  .bind   = lpc_pconp_bind,
  .ops    = &lpc_pconp_ops,
};

