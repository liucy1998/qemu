/*
 * Host-Guest Pipe
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/qdev-properties.h"
#include "qemu/module.h"
#include "pipe_core.h"
// #include "sysemu/kvm.h"


typedef struct PipeState {
    /*< private >*/
    PCIDevice parent_obj;
    /*< public >*/
    char *pipe_path;
    uint8_t debug;

} PipeState;

#define TYPE_PIPE "pipe"

#define PCI_TEST_DEV(obj) \
    OBJECT_CHECK(PipeState, (obj), TYPE_PIPE)

static void
pci_pipe_reset(PipeState *d)
{
    // Pipe should not be reset!
   return;
}

static void pci_pipe_realize(PCIDevice *pci_dev, Error **errp)
{
    PipeState *d = PCI_TEST_DEV(pci_dev);
    if (host_pipe_init(d->pipe_path, d->debug) < 0) {
        fprintf(stderr, "Cannot initialize pipe!");
    }
}

static void qdev_pci_pipe_reset(DeviceState *dev)
{
    PipeState *d = PCI_TEST_DEV(dev);
    pci_pipe_reset(d);
}

static Property pci_pipe_properties[] = {
    DEFINE_PROP_STRING("path", PipeState, pipe_path),
    DEFINE_PROP_UINT8("debug", PipeState, debug, 0),
    DEFINE_PROP_END_OF_LIST(),
};

static void pipe_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);

    k->realize = pci_pipe_realize;
    k->class_id = PCI_CLASS_OTHERS;
    dc->desc = "Host-Guest Pipe";
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
    dc->reset = qdev_pci_pipe_reset;
    device_class_set_props(dc, pci_pipe_properties);
}

static const TypeInfo pipe_info = {
    .name          = TYPE_PIPE,
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(PipeState),
    .class_init    = pipe_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    },
};

static void pipe_register_types(void)
{
    type_register_static(&pipe_info);
}

type_init(pipe_register_types)
