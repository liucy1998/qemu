#ifndef H_SHM_HC
#define H_SHM_HC

#include <linux/kvm.h>
#include "qemu/typedefs.h"
#include <stdbool.h>
#include <stdint.h>

int hc_rw_host_shm(struct kvm_run *run, CPUState *cpu, uint64_t *size, bool is_read);

#endif