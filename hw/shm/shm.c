/*
 * Host-Guest Shared Memory
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
#include "shm_core.h"
// #include "sysemu/kvm.h"


typedef struct SHMState {
    /*< private >*/
    PCIDevice parent_obj;
    /*< public >*/
    char *shm_path;
    uint64_t shm_size;
    uint8_t debug;

} SHMState;

#define TYPE_SHM "shm"

#define PCI_TEST_DEV(obj) \
    OBJECT_CHECK(SHMState, (obj), TYPE_SHM)

static void
pci_shm_reset(SHMState *d)
{
    // Shared memory should not be reset!
   return;
}

static void pci_shm_realize(PCIDevice *pci_dev, Error **errp)
{
    SHMState *d = PCI_TEST_DEV(pci_dev);
    if (host_shm_init(d->shm_path, d->shm_size, d->debug) < 0) {
        fprintf(stderr, "Cannot initialize shm!");
    }
}

static void qdev_pci_shm_reset(DeviceState *dev)
{
    SHMState *d = PCI_TEST_DEV(dev);
    pci_shm_reset(d);
}

static Property pci_shm_properties[] = {
    DEFINE_PROP_STRING("path", SHMState, shm_path),
    DEFINE_PROP_UINT64("size", SHMState, shm_size, 0),
    DEFINE_PROP_UINT8("debug", SHMState, debug, 0),
    DEFINE_PROP_END_OF_LIST(),
};

static void shm_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);

    k->realize = pci_shm_realize;
    k->class_id = PCI_CLASS_OTHERS;
    dc->desc = "Host-Guest Shared Memory";
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
    dc->reset = qdev_pci_shm_reset;
    device_class_set_props(dc, pci_shm_properties);
}

static const TypeInfo shm_info = {
    .name          = TYPE_SHM,
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(SHMState),
    .class_init    = shm_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    },
};

static void shm_register_types(void)
{
    type_register_static(&shm_info);
}

type_init(shm_register_types)
