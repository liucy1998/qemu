#ifndef H_PIPE_CORE
#define H_PIPE_CORE

#include "qemu/osdep.h"

#define MAX_PIPE 32
// 64 pages
#define PIPE_BUF_SIZE 262144 

struct Pipe {
    int fd;
    void *buf;
};

int host_pipe_init(char *path);
struct Pipe* get_pipe(int idx);

#endif