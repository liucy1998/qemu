#include "pipe_hc.h"
#include "hw/memory_access.h"
#include "pipe_core.h"
#include <errno.h>
#include <string.h>

int hc_rw_host_pipe(struct kvm_run *run, CPUState *cpu, uint64_t *rax, bool is_read) {
    int idx = (int)run->hypercall.args[0];
    struct Pipe *pipe = get_pipe(idx);
    if (pipe == NULL) {
        return -1;
    }

    uint64_t guest_addr, size;

    guest_addr = (uint64_t)run->hypercall.args[1];
    size = (uint64_t)run->hypercall.args[2];
    if (pipe->debug) {
        printf("[____PIPE____]: Args: idx=%d, guest_addr=0x%lx, size=%ld\n", idx, guest_addr, size);
    }

    int ret;

    if (pipe->fd <= 0) {
        ret = *rax = -1;
        goto out;
    }

    char *buf = pipe->buf;
    if (is_read) {
        // read from host, write to guest
        int64_t data_size;
        if ((data_size=read(pipe->fd, buf, size)) < 0) {
            printf("[____PIPE____]: read fifo error! errno: %d, msg: %s\n", errno, strerror(errno));
            ret = *rax = -1;
            goto out;
        }
        ret = write_virtual_memory(guest_addr, (uint8_t*)buf, data_size, cpu) == true ? 0 : -1;
        *rax = ret == -1 ? -1 : data_size; 
    }
    else {
        // read from guest, write to host 
        if (size > PIPE_BUF_SIZE) {
            // TODO: mmap a larger space
            ret = *rax = -1;
            goto out;
        }
        ret = read_virtual_memory(guest_addr, (uint8_t*)buf, size, cpu) == true ? 0 : -1;
        if (ret == -1) {
            *rax = -1;
            goto out;
        }
        uint64_t written = 0;
        while(written < size) {
            ssize_t t = write(pipe->fd, buf+written, size-written);
            if (t < 0) {
                printf("[____PIPE____]: write fifo error! errno: %d, msg: %s\n", errno, strerror(errno));
                ret = *rax = -1;
                goto out;
            }
            written += t;
        }
        *rax = size;
    }

out:
    return ret;
}