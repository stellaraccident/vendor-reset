# Vendor Reset

> **This fork** ([stellaraccident/vendor-reset](https://github.com/stellaraccident/vendor-reset))
> adds **experimental support for AMD Navi 48 (RDNA4, RX 9070/9070 XT)** VFIO
> passthrough. The Navi 48 fix suppresses the PCIe bus reset that is fatal to
> these firmware-driven GPUs and provides a no-op device-specific reset handler.
> This is an interim workaround — the correct long-term fix is a PCI quirk in the
> kernel (`DECLARE_PCI_FIXUP_HEADER` in `drivers/pci/quirks.c`). See the Navi 48
> section below for details.
>
> Upstream: [gnif/vendor-reset](https://github.com/gnif/vendor-reset)

The goal of this project is to provide a kernel module that is capable of
resetting hardware devices into a state where they can be re-initialized or
passed through into a virtual machine (VFIO). While it would be great to have
these in the kernel as PCI quirks, some of the reset procedures are very complex
and would never be accepted as a quirk (ie AMD Vega 10).

By providing this as an out of tree kernel module, vendors will be able to
easily create pull requests to add functionality to this module, and users will
be able to easily update this module without requiring a complete kernel rebuild.

## Patching the kernel

TL;DR - No patching required.

This module has been written to use `ftrace` to hook `pci_dev_specific_reset`,
allowing it to handle device resets directly without patching the running
kernel. Simply modprobing the module is enough to enable the reset routines for
all supported hardware.

## Requirements

Ensure your kernel has the following options enabled:

```
CONFIG_FTRACE=y
CONFIG_KPROBES=y
CONFIG_PCI_QUIRKS=y
CONFIG_KALLSYMS=y
CONFIG_KALLSYMS_ALL=y
CONFIG_FUNCTION_TRACER=y
```

## Installing

This module can be installed either using the standard `make`, `make install`
pattern, or through `dkms` (recommended).

    dkms install .

## Usage

Either `modprobe vendor-reset` or add the device to the appropriate place to
load it at system boot, such as `/etc/modules` (Debian). Consult your
distribution's documentation as to the best way to perform this.

**NOTE: ** This module must be loaded EARLY, the default reset the kernel will
try to perform completely breaks the GPU which this module can not recover from.
Please consult your distributions documentation on how to do this, for most
however it will be as simple as adding `vendor-reset` to `/etc/modules` and
updating your initrd.

For Fedora with dracut, add to `/etc/dracut.conf.d/vfio.conf`:
```
force_drivers+=" vfio vfio_iommu_type1 vfio_pci vendor-reset "
```

## Supported Devices

| Vendor | Family | Common Name(s) | Reset Method |
|---|---|---|---|
|AMD|Polaris 10| RX 470, 480, 570, 580, 590 | BACO |
|AMD|Polaris 11| RX 460, 560 | BACO |
|AMD|Polaris 12| RX 540, 550 | BACO |
|AMD|Vega 10| Vega 56/64/FE | BACO |
|AMD|Vega 20| Radeon VII | Mode1 + BACO |
|AMD|Vega 20| Instinct MI100 | Mode1 + BACO |
|AMD|Navi 10| 5600XT, 5700, 5700XT | Mode1 |
|AMD|Navi 12| Pro 5600M | Mode1 |
|AMD|Navi 14| Pro 5300, RX 5300, 5500XT | Mode1 |
|AMD|**Navi 48**| **RX 9070, 9070 XT** | **Bus reset suppression** (this fork) |

### Navi 48 (RDNA4) — Experimental

The RX 9070 / 9070 XT (Navi 48, `1002:7550`) cannot survive a PCIe bus reset.
These GPUs have a firmware-driven architecture where the PSP (Platform Security
Processor) must reload SMU (System Management Unit) microcode for the card to
function. A bus reset disrupts this firmware state irreversibly — the card enters
D3cold with corrupted config space and is unrecoverable without a host power cycle.

**How the fix works:**

1. At module load, `PCI_DEV_FLAGS_NO_BUS_RESET` is set on Navi 48 devices,
   preventing the kernel and vfio from issuing a bus reset.
2. A no-op device-specific reset handler claims reset support, so the kernel
   doesn't fall through to other reset methods.
3. The card stays in its post-shutdown state after a guest VM stops.
4. The next amdgpu driver load (in the guest) reinitializes the card via the
   PSP firmware reload sequence.

**Limitations:**
- Guest display output on a physical monitor is intermittent (works on first
  boot after host power cycle, may not survive VM restart). Headless/compute
  passthrough is fully stable.
- This is a bus reset suppression, not a true ASIC reset. The card relies on
  the guest driver to reinitialize from a clean post-fini state.

**Proper upstream fix:** This should be a PCI quirk in the kernel:
```c
DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_ATI, 0x7550, quirk_no_bus_reset);
```
This vendor-reset entry serves as a proof-of-concept and interim workaround
while that kernel change is evaluated. Per the upstream project's guidance,
simple bus-reset suppressions should ultimately live in the kernel.

## Developing

If you are a vendor intending to add support for your device to this project
please first consider two things:

1. Can you fix your hardware/firmware to reset correctly using FLR or a BUS
   reset?
2. Is the reset simple enough that it should really be a kernel pci quirk
   (see: kernel/drivers/pci/quirk.c)?

If you answer yes to either of these questions this project is not for you.

### Build fix for kernel 6.17+

The `asm/unaligned.h` header was moved to `linux/unaligned.h` in recent kernels.
This fork includes the fix in `src/amd/amdgpu/atom.c`.
