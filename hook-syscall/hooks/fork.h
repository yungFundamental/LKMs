/*
 * While using this code I noticed that it wasn't being called. After research and testing, modern systems rarely use fork.
 * To hook the creation of processes I'd recommend hooking the sys_clone syscall.
 */
#include <linux/sched.h>
#include <linux/version.h>
#include "ftrace_helper.h"


#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage pid_t (*orig_fork)(const struct pt_regs *);

static asmlinkage pid_t hooked_fork(const struct pt_regs *registers)
{
    pid_t current_pid;
    pid_t res;

    current_pid = current->pid;
    res = orig_fork(registers);

    if (res <= -1)
    {
        printk(KERN_ALERT "hook-fork: error - sys_fork of %d returned %d", current_pid, res);
        return res;
    }
    if (res == 0)
    {
        printk(KERN_INFO "hook-fork: child_process - sys_fork of %d returned %d", current_pid, res);
        return res;
    }

    printk(KERN_INFO "hook-fork: parent_process - sys_fork of %d returned %d", current_pid, res);
    return res;
}
#else

#endif

const struct ftrace_hook fork_hook = HOOK("sys_fork", hooked_fork, &orig_fork);

