#ifndef H_PIPE_CORE
#define H_PIPE_CORE

#include "qemu/osdep.h"

#define MAX_PIPE 32
// 64 pages
#define PIPE_BUF_SIZE 262144 

struct Pipe {
    int fd;
    void *buf;
    uint8_t debug;
};

int host_pipe_init(char *path, uint8_t debug);
struct Pipe* get_pipe(int idx);

#endif