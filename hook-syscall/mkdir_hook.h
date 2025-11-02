/**
 * Include hooks to functions. The hooks array contains funcs that can be installed with the ftrace_helper API.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/version.h>
#include "ftrace_helper.h"


#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage pid_t (*orig_mkdir)(const struct pt_regs *);

asmlinkage long hooked_mkdir(const struct pt_regs *registers)
{

    long res;
    long copy_error;
    char __user *pathname = (char *) registers->di;
    char dir_name[NAME_MAX] = {0};

    copy_error = strncpy_from_user(dir_name, pathname, NAME_MAX);


    if (copy_error > 0)
    {
        printk(KERN_INFO "hook-mkdir: PID %d created the directory %s\n", current->pid, dir_name);
    }

    res = orig_mkdir(registers);
    return res;
}
#else

#endif

static struct ftrace_hook hooks[] = {
    HOOK("sys_mkdir", hooked_mkdir, &orig_mkdir)
};

