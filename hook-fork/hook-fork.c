#include <linux/init.h>
#include <linux/module.h>


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Amit Barzilai");
MODULE_DESCRIPTION("Hooks the fork syscall and prints a to the kernel buffer the PID that called the syscall.");

static int hello_init(void)
{
	printk(KERN_ALERT "Hello, world\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
