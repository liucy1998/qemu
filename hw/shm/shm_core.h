#ifndef H_SHM_CORE
#define H_SHM_CORE

#include "qemu/osdep.h"

#define MAX_SHM 32

struct SHM {
    void *mmap_addr;
    uint64_t size;
};

int host_shm_init(char *path, uint64_t size);
struct SHM* get_shm(int idx);

#endif