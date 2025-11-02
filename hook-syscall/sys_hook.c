#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include "ftrace_helper.h"
#include "mkdir_hook.h"


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Amit Barzilai");
MODULE_DESCRIPTION("Hooks the fork syscall and prints a to the kernel buffer the PID that called the syscall.");


static int sys_hooks_init(void)
{
    int err;
    err = fh_install_hooks(hooks, ARRAY_SIZE(hooks));
    if (err)
    {
        printk(KERN_ALERT "sys_hook: Failed to install hooks\n");
        return err;
    }
    
    printk(KERN_INFO "sys_hook: Hooking syscalls\n");
    return 0;
}

static void sys_hooks_exit(void)
{
    fh_remove_hooks(hooks, ARRAY_SIZE(hooks));
    printk(KERN_INFO "sys_hook: Removed hooks\n");
}

module_init(sys_hooks_init);
module_exit(sys_hooks_exit);
