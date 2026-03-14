/*
Vendor Reset - Vendor Specific Reset
Copyright (C) 2020 Geoffrey McRae <geoff@hostfission.com>

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

#include "amd/amd.h"

#define DEVICE_INFO(x) x, #x

#define _AMD_POLARIS10(op) \
    {PCI_VENDOR_ID_ATI, 0x67C0, op, DEVICE_INFO(AMD_POLARIS10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67C1, op, DEVICE_INFO(AMD_POLARIS10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67C2, op, DEVICE_INFO(AMD_POLARIS10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67C4, op, DEVICE_INFO(AMD_POLARIS10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67C7, op, DEVICE_INFO(AMD_POLARIS10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67D0, op, DEVICE_INFO(AMD_POLARIS10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67DF, op, DEVICE_INFO(AMD_POLARIS10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67C8, op, DEVICE_INFO(AMD_POLARIS10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67C9, op, DEVICE_INFO(AMD_POLARIS10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67CA, op, DEVICE_INFO(AMD_POLARIS10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67CC, op, DEVICE_INFO(AMD_POLARIS10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67CF, op, DEVICE_INFO(AMD_POLARIS10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6FDF, op, DEVICE_INFO(AMD_POLARIS10), 0}

#define _AMD_POLARIS11(op) \
    {PCI_VENDOR_ID_ATI, 0x67E0, op, DEVICE_INFO(AMD_POLARIS11), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67E3, op, DEVICE_INFO(AMD_POLARIS11), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67E8, op, DEVICE_INFO(AMD_POLARIS11), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67EB, op, DEVICE_INFO(AMD_POLARIS11), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67EF, op, DEVICE_INFO(AMD_POLARIS11), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67FF, op, DEVICE_INFO(AMD_POLARIS11), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67E1, op, DEVICE_INFO(AMD_POLARIS11), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67E7, op, DEVICE_INFO(AMD_POLARIS11), 0}, \
    {PCI_VENDOR_ID_ATI, 0x67E9, op, DEVICE_INFO(AMD_POLARIS11), 0}

#define _AMD_POLARIS12(op) \
    {PCI_VENDOR_ID_ATI, 0x6980, op, DEVICE_INFO(AMD_POLARIS12), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6981, op, DEVICE_INFO(AMD_POLARIS12), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6985, op, DEVICE_INFO(AMD_POLARIS12), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6986, op, DEVICE_INFO(AMD_POLARIS12), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6987, op, DEVICE_INFO(AMD_POLARIS12), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6995, op, DEVICE_INFO(AMD_POLARIS12), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6997, op, DEVICE_INFO(AMD_POLARIS12), 0}, \
    {PCI_VENDOR_ID_ATI, 0x699F, op, DEVICE_INFO(AMD_POLARIS12), 0}

#define _AMD_VEGA10(op) \
    {PCI_VENDOR_ID_ATI, 0x6860, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6861, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6862, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6863, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6864, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6867, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6868, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x6869, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x686a, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x686b, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x686c, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x686d, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x686e, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x686f, op, DEVICE_INFO(AMD_VEGA10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x687f, op, DEVICE_INFO(AMD_VEGA10), 0}

#define _AMD_VEGA20(op) \
    {PCI_VENDOR_ID_ATI, 0x66a0, op, DEVICE_INFO(AMD_VEGA20), 0}, \
    {PCI_VENDOR_ID_ATI, 0x66a1, op, DEVICE_INFO(AMD_VEGA20), 0}, \
    {PCI_VENDOR_ID_ATI, 0x66a2, op, DEVICE_INFO(AMD_VEGA20), 0}, \
    {PCI_VENDOR_ID_ATI, 0x66a3, op, DEVICE_INFO(AMD_VEGA20), 0}, \
    {PCI_VENDOR_ID_ATI, 0x66a4, op, DEVICE_INFO(AMD_VEGA20), 0}, \
    {PCI_VENDOR_ID_ATI, 0x66a7, op, DEVICE_INFO(AMD_VEGA20), 0}, \
    {PCI_VENDOR_ID_ATI, 0x66af, op, DEVICE_INFO(AMD_VEGA20), 0}

#define _AMD_NAVI10(op) \
    {PCI_VENDOR_ID_ATI, 0x7310, op, DEVICE_INFO(AMD_NAVI10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x7312, op, DEVICE_INFO(AMD_NAVI10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x7318, op, DEVICE_INFO(AMD_NAVI10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x7319, op, DEVICE_INFO(AMD_NAVI10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x731a, op, DEVICE_INFO(AMD_NAVI10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x731b, op, DEVICE_INFO(AMD_NAVI10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x731e, op, DEVICE_INFO(AMD_NAVI10), 0}, \
    {PCI_VENDOR_ID_ATI, 0x731f, op, DEVICE_INFO(AMD_NAVI10), 0}

#define _AMD_NAVI14(op) \
    {PCI_VENDOR_ID_ATI, 0x7340, op, DEVICE_INFO(AMD_NAVI14), 0}, \
    {PCI_VENDOR_ID_ATI, 0x7341, op, DEVICE_INFO(AMD_NAVI14), 0}, \
    {PCI_VENDOR_ID_ATI, 0x7347, op, DEVICE_INFO(AMD_NAVI14), 0}, \
    {PCI_VENDOR_ID_ATI, 0x734F, op, DEVICE_INFO(AMD_NAVI14), 0}

#define _AMD_NAVI12(op) \
    {PCI_VENDOR_ID_ATI, 0x7360, op, DEVICE_INFO(AMD_NAVI12), 0}, \
    {PCI_VENDOR_ID_ATI, 0x7362, op, DEVICE_INFO(AMD_NAVI12), 0}

#define _AMD_ARCTURUS(op) \
    {PCI_VENDOR_ID_ATI, 0x738c, op, DEVICE_INFO(AMD_VEGA20), 0} /* Instinct MI100 */

/*
 * Navi 48 (RDNA4): RX 9070/9070 XT
 *
 * These GPUs cannot survive a PCIe bus reset — their firmware-driven
 * architecture (PSP + SMU) requires a full firmware reload sequence
 * that only the amdgpu driver can perform. A bus reset puts the card
 * into an unrecoverable state (D3cold with corrupted config space).
 *
 * VENDOR_RESET_CFG_NO_BUS_RESET suppresses the bus reset at module
 * load time by setting PCI_DEV_FLAGS_NO_BUS_RESET on the device.
 * The no-op reset handler (amd_navi48_ops) provides a device-specific
 * reset that returns success, preventing the kernel from falling
 * through to bus reset.
 *
 * NOTE: The correct long-term fix is a PCI quirk in the kernel:
 *   DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_ATI, 0x7550, quirk_no_bus_reset);
 * This vendor-reset entry serves as a proof-of-concept for that fix.
 * See: kernel/drivers/pci/quirks.c
 */
#define _AMD_NAVI48(op) \
    {PCI_VENDOR_ID_ATI, 0x7550, op, DEVICE_INFO(AMD_NAVI48), VENDOR_RESET_CFG_NO_BUS_RESET}

static const struct vendor_reset_cfg vendor_reset_devices[] = {
    _AMD_POLARIS10(&amd_polaris10_ops),
    _AMD_POLARIS11(&amd_polaris10_ops),
    _AMD_POLARIS12(&amd_polaris10_ops),
    _AMD_VEGA10(&amd_vega10_ops),
    _AMD_VEGA20(&amd_vega20_ops),
    _AMD_NAVI10(&amd_navi10_ops),
    _AMD_NAVI14(&amd_navi10_ops),
    _AMD_NAVI12(&amd_navi10_ops),
    _AMD_ARCTURUS(&amd_vega20_ops),
    _AMD_NAVI48(&amd_navi48_ops),

    /* end of array guard */
    {.vendor = 0}
};
