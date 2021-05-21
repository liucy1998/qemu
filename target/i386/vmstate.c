#include "qemu/osdep.h"
#include "qemu/main-loop.h"
#include "sysemu/kvm_int.h"
#include "migration/snapshot.h"
#include "qapi/error.h"
#include "sysemu/runstate.h"

#include "vmstate.h"

int hc_vmstate(struct kvm_run *run, CPUState *cpu) {
    int ret = 0;
    int op = (int)run->hypercall.args[0];
    if (op == 0) {
        vm_stop(RUN_STATE_PAUSED);
    }
    // Try to implement savevm/loadvm below, but looks buggy.
    // 
    // Error *err = NULL;
    // if (op == 0) {
    //     printf("SAVEVM!!\n");
    //     qemu_mutex_lock_iothread();
    //     kvm_cpu_synchronize_state(qemu_get_cpu(0));
    //     save_snapshot("image", &err);
    //     if (err) {
    //         error_reportf_err(err, "Error");
    //         ret = -1;
    //     }
    //     qemu_mutex_unlock_iothread();
    //     // qemu_system_shutdown_request(SHUTDWN_CAUSE_HOST_SIGNAL);
    // }
    // else if (op == 1) {
    //     int saved_vm_running;
    //     qemu_mutex_lock_iothread();
    //     kvm_cpu_synchronize_state(qemu_get_cpu(0));
    //     saved_vm_running = runstate_is_running();
    //     vm_stop(RUN_STATE_RESTORE_VM);
    //     if (!load_snapshot("image", &err) && saved_vm_running) {
    //         vm_start();
    //     }
    //     if (err) {
    //         error_reportf_err(err, "Error");
    //         ret = -1;
    //     }
    //     qemu_mutex_unlock_iothread();
    // }
    // else {
    //     ret = -1;
    // }
    return ret;
}