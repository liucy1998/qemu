#include "shm_core.h"
#include <sys/ioctl.h>
#include <sys/mman.h>

static struct SHM shm[MAX_SHM];
static int shm_num = 0;

int host_shm_init(char *path, uint64_t size) {
    printf("[____SHM____]: Initialize: path=%s, size=%lu\n", path, size);
    if (shm_num == MAX_SHM) {
        return -1;
    }
    int idx = shm_num++;
    int fd;
    if ((fd=open(path, O_RDWR, S_IRWXU|S_IRWXG|S_IRWXO)) < 0) {
        return -1;
    }
    if ((shm[idx].mmap_addr=mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) < 0) {
        return -1;
    }
    shm[idx].size = size;
    close(fd);
    printf("[____SHM____]: Initialize success!\n");
    return 0;
}

struct SHM* get_shm(int idx) {
    if (idx >= shm_num) {
        return NULL;
    }
    return &shm[idx];
}
