#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Amit Barzilai");
MODULE_DESCRIPTION("A RAM handled file buffer");


static struct proc_dir_entry *proc_entry;

static ssize_t ram_mod_read(struct file *fp, char __user *user_buf, size_t req_size, loff_t *offset)
{
    static char message[] = "Hi there!\n";
    size_t len = sizeof(message) - 1;
    size_t copied;
    if (*offset >= len)
        return 0;

    size_t amount_to_read = req_size;
    if (req_size > len)
        amount_to_read = len;
    copied = copy_to_user(user_buf, message, amount_to_read);
    *offset += amount_to_read;
    return amount_to_read;

}
static struct proc_ops proc_operations = {
    .proc_read    = ram_mod_read
};

static int ram_mod_init(void)
{
	printk(KERN_ALERT "Hello, world\n");
    proc_entry = proc_create("barz.txt", 0, NULL, &proc_operations);
    if (proc_entry == NULL) {
        return -ENOMEM;
    }

	return 0;
}

static void ram_mod_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
    proc_remove(proc_entry);
}

module_init(ram_mod_init);
module_exit(ram_mod_exit);
