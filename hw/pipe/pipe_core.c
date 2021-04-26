#include "pipe_core.h"
#include <sys/ioctl.h>
#include <sys/mman.h>

static struct Pipe p[MAX_PIPE];
static int pipe_num = 0;

int host_pipe_init(char *path) {
    printf("[____PIPE____]: Initialize: path=%s\n", path);
    if (pipe_num == MAX_PIPE) {
        return -1;
    }
    int idx = pipe_num++;
    int o_flag = O_RDWR;
    if ((p[idx].fd=open(path, o_flag)) < 0) {
        return -1;
    }
    if ((p[idx].buf=mmap(NULL, PIPE_BUF_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0)) == ((void *)-1)) {
        return -1;
    }
    printf("[____PIPE____]: Initialize success!\n");
    return 0;
}

struct Pipe* get_pipe(int idx) {
    if (idx >= pipe_num) {
        return NULL;
    }
    return &p[idx];
}
