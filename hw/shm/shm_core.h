#ifndef H_SHM_CORE
#define H_SHM_CORE

#include "qemu/osdep.h"

#define MAX_SHM 32

struct SHM {
    void *mmap_addr;
    uint64_t size;
    uint8_t debug;
};

int host_shm_init(char *path, uint64_t size, uint8_t debug);
struct SHM* get_shm(int idx);

#endif