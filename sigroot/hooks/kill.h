/*
 * While using this code I noticed that it wasn't being called. After research and testing, modern systems rarely use kill.
 * To hook the creation of processes I'd recommend hooking the sys_clone syscall.
 */
#include <linux/sched.h>
#include <linux/version.h>
#include "ftrace_helper.h"

#define SET_ROOT_SIGNAL 64

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

void set_root()
{
    
}

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage int (*orig_kill)(const struct pt_regs *);

static asmlinkage int hooked_kill(const struct pt_regs *registers)
{
    pid_t current_pid;
    pid_t res;

    current_pid = current->pid;
    res = orig_kill(registers);

    if (res <= -1)
    {
        printk(KERN_ALERT "hook-kill: error - sys_kill of %d returned %d", current_pid, res);
        return res;
    }
    if (res == 0)
    {
        printk(KERN_INFO "hook-kill: child_process - sys_kill of %d returned %d", current_pid, res);
        return res;
    }

    printk(KERN_INFO "hook-kill: parent_process - sys_kill of %d returned %d", current_pid, res);
    return res;
}
#else

static asmlinkage int (*orig_kill)(pid_t pid, int sig);
static asmlinkage int hooked_kill(pid_t pid, int sig)
{
    pid_t current_pid;
    pid_t res;

    if (sig == SET_ROOT_SIGNAL)
    {
	
    }
    res = orig_kill();

    return res;
}
#endif

const struct ftrace_hook kill_hook = HOOK("sys_kill", hooked_kill, &orig_kill);

