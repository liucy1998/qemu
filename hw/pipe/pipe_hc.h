#ifndef H_PIPE_HC
#define H_PIPE_HC

#include <linux/kvm.h>
#include "qemu/typedefs.h"
#include <stdbool.h>
#include <stdint.h>

int hc_rw_host_pipe(struct kvm_run *run, CPUState *cpu, uint64_t *size, bool is_read);

#endif