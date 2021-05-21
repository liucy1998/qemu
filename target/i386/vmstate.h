#ifndef VMSTATE_H
#define VMSTATE

#include "qemu/osdep.h"

int hc_vmstate(struct kvm_run *run, CPUState *cpu);

#endif