#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include "trampoline/trampoline.h"


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Amit Barzilai");
MODULE_DESCRIPTION("Trampoline hook");


static int (*orig_iterate_dir)(struct file *, struct dir_context *);

static int hooked_iterate_dir(struct file *f, struct dir_context *dc)
{
    printk(KERN_ALERT "Get hooked douchebag!\n");
    return orig_iterate_dir(f, dc);
}

static const hook_t iterate_dir_hook = {
    .target_name = "iterate_dir",
    .original_function = (void *)&orig_iterate_dir,
    .function = hooked_iterate_dir
};

static int hook_ls_init(void)
{

	return 0;
}

static void hook_ls_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hook_ls_init);
module_exit(hook_ls_exit);
