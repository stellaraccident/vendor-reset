/*
Vendor Reset - Vendor Specific Reset
Copyright (C) 2020 Geoffrey McRae <geoff@hostfission.com>
Copyright (C) 2020 Adam Madsen <adam@ajmadsen.com>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <linux/module.h>
#include <linux/pci.h>

#include "ioctl.h"
#include "hook.h"
#include "vendor-reset-dev.h"

/*
 * Set PCI_DEV_FLAGS_NO_BUS_RESET on devices that have the
 * VENDOR_RESET_CFG_NO_BUS_RESET flag in their config entry.
 *
 * This must happen at module load time, before vfio-pci attempts a bus
 * reset during VM startup. The ftrace hook in hook.c intercepts
 * pci_dev_specific_reset, but the IOMMU group-level bus reset in vfio
 * goes through pci_reset_bus which does NOT call pci_dev_specific_reset.
 * The only way to prevent it is to have NO_BUS_RESET set on the device
 * before vfio opens the group.
 *
 * Only devices with VENDOR_RESET_CFG_NO_BUS_RESET are affected. Devices
 * with working vendor-reset handlers (e.g. Vega 20 BACO) do not need
 * this — their handler provides a real reset that preempts bus reset via
 * the ftrace hook.
 *
 * NOTE: The proper upstream fix for devices that cannot survive a bus
 * reset is a PCI quirk in drivers/pci/quirks.c:
 *   DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_ATI, 0x7550, quirk_no_bus_reset);
 * This module-level workaround serves as a proof-of-concept for that fix.
 */
static void vendor_reset_apply_no_bus_reset(void)
{
  struct pci_dev *pdev = NULL;
  const struct vendor_reset_cfg *cfg;

  for_each_pci_dev(pdev)
  {
    cfg = vendor_reset_cfg_find(pdev->vendor, pdev->device);
    if (cfg && (cfg->flags & VENDOR_RESET_CFG_NO_BUS_RESET))
    {
      pdev->dev_flags |= PCI_DEV_FLAGS_NO_BUS_RESET;
      pci_info(pdev, "vendor-reset: set NO_BUS_RESET for %s [%04x:%04x]\n",
               cfg->info_str, pdev->vendor, pdev->device);
    }
  }
}

static int __init vendor_reset_init(void)
{
  int ret;

  ret = vendor_reset_ioctl_init();
  if (ret)
    return ret;

  ret = vendor_reset_hook_init();
  if (ret)
    goto err;

  vendor_reset_apply_no_bus_reset();

  return 0;

err:
  vendor_reset_ioctl_exit();
  return ret;
}

static void __exit vendor_reset_exit(void)
{
  vendor_reset_hook_exit();
  vendor_reset_ioctl_exit();
}

module_init(vendor_reset_init);
module_exit(vendor_reset_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Geoffrey McRae <geoff@hostfission.com>");
MODULE_AUTHOR("Adam Madsen <adam@ajmadsen.com>");
MODULE_VERSION("0.1.1");
