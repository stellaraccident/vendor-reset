/*
Vendor Reset - Vendor Specific Reset
Copyright (C) 2020 Geoffrey McRae <geoff@hostfission.com>
Copyright (C) 2026 Stella (AMD) - Navi 48 (RDNA4) support

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

/*
 * Navi 48 (RDNA4, gfx12, e.g. RX 9070/9070 XT) reset handler.
 *
 * These GPUs have a firmware-driven architecture (PSP + SMU) where
 * reset requires the PSP to reload SMU microcode before the card
 * becomes functional. A PCIe bus reset kills the firmware state
 * without a clean shutdown sequence, and no hardware-level reset
 * can recover it — only a full PSP firmware reload (performed by
 * amdgpu during driver init) can bring the card back.
 *
 * The fix has two parts:
 *
 *   1. VENDOR_RESET_CFG_NO_BUS_RESET in the device-db entry causes
 *      PCI_DEV_FLAGS_NO_BUS_RESET to be set at module load time,
 *      preventing the kernel/vfio from issuing a bus reset.
 *
 *   2. This no-op reset handler claims device-specific reset support,
 *      so the kernel doesn't fall through to other reset methods. It
 *      returns success without touching the hardware — the card stays
 *      in whatever state the guest driver left it (a valid post-fini
 *      state after clean shutdown). The next amdgpu driver load does
 *      the proper PSP/SMU init sequence to bring everything back.
 *
 * This is not a hack — it is the architecturally correct behavior for
 * devices whose reset requires firmware cooperation that a bus reset
 * cannot provide.
 *
 * NOTE: The proper upstream fix is a PCI quirk in the kernel:
 *   DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_ATI, 0x7550, quirk_no_bus_reset);
 * in drivers/pci/quirks.c. This vendor-reset entry serves as a
 * proof-of-concept and interim workaround pending that kernel change.
 * This approach is consistent with the vendor-reset project's guidance
 * that simple bus-reset suppressions should ultimately live in the kernel.
 */

#include "vendor-reset-dev.h"
#include "common.h"

static int amd_navi48_reset(struct vendor_reset_dev *dev)
{
  vr_info(dev, "navi48: no-op reset (firmware-managed device, bus reset suppressed)\n");
  return 0;
}

const struct vendor_reset_ops amd_navi48_ops =
{
  .version = {1, 0},
  .probe = amd_common_probe,
  .pre_reset = NULL,
  .reset = amd_navi48_reset,
  .post_reset = NULL,
};
