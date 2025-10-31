#include <linux/init.h>
#include <linux/module.h>


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Amit Barzilai");
MODULE_DESCRIPTION("Hooks the fork syscall and prints a to the kernel buffer the PID that called the syscall.");

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
