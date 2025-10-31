#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/version.h>


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Amit Barzilai");
MODULE_DESCRIPTION("Hooks the fork syscall and prints a to the kernel buffer the PID that called the syscall.");

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

static int fork_hook_init(void)
{
	printk(KERN_INFO "Hooking the fork syscall\n");
	return 0;
}

static void fork_hook_exit(void)
{
	printk(KERN_ALERT "Removing the fork syscall hook\n");
}

module_init(fork_hook_init);
module_exit(fork_hook_exit);
