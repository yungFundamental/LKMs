#include <linux/init.h>
#include <linux/module.h>
#include "trampoline/trampoline.h"


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Amit Barzilai");
MODULE_DESCRIPTION("Trampoline hook");

static int hook_ls_init(void)
{
	printk(KERN_ALERT "Hello, world\n");
	return 0;
}

static void hook_ls_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hook_ls_init);
module_exit(hook_ls_exit);
