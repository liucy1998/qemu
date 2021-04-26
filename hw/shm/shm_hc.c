#include "shm_hc.h"
#include "hw/memory_access.h"
#include "shm_core.h"

int hc_rw_host_shm(struct kvm_run *run, CPUState *cpu, uint64_t *rax, bool is_read) {
    int idx = (int)run->hypercall.args[0];
    struct SHM *shm = get_shm(idx);
    if (shm == NULL) {
        return -1;
    }

    uint64_t host_addr, host_addr_offset, guest_addr, size;

    host_addr_offset = (uint64_t)run->hypercall.args[1];
    guest_addr = (uint64_t)run->hypercall.args[2];
    size = (uint64_t)run->hypercall.args[3];
    printf("[____SHM____]: Args: idx=%d, offset=%ld, guest_addr=0x%lx, size=%ld\n", idx, host_addr_offset, guest_addr, size);

    if ((host_addr_offset+size) > shm->size) {
        *rax = -1;
        return -1;
    }

    host_addr = (uint64_t)shm->mmap_addr + host_addr_offset;

    int ret;
    if (is_read) {
        // read from host, write to guest
        ret = write_virtual_memory(guest_addr, (uint8_t*)host_addr, size, cpu) == true ? 0 : -1;
    }
    else {
        // read from guest, write to host 
        ret = read_virtual_memory(guest_addr, (uint8_t*)host_addr, size, cpu) == true ? 0 : -1;
    }

    *rax = (ret == 0 ? size : -2);
    return ret;
}