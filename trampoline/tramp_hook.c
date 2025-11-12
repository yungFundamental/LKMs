#include <linux/string.h>
#include <linux/mman.h>
#include <asm/cacheflush.h>
#include <asm/pgtable.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kallsyms.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <asm/tlbflush.h>  


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Amit Barzilai");
MODULE_DESCRIPTION("Trampoline hook");

typedef pte_t *(*lookup_address_t)(unsigned long address, unsigned int *level);

static u8 call_opcode = 0xe8;
static lookup_address_t my_lookup_address;

int make_page_writable(unsigned long address) {
    unsigned int level;
    pte_t *pte;
    
    pte = my_lookup_address(address, &level);
    if (!pte) 
	return -EINVAL;
    
    set_pte_atomic(pte, pte_mkwrite(*pte));
    __flush_tlb_one(address);
    return 0;
}

void hooker(void)
{
    printk(KERN_INFO "Trampoline: get hooked douchebag!\n");
}

static int sys_hooks_init(void)
{
    void *calling_address;
    u32 relative_hooker_address;
    unsigned long hooked_func_page;
    struct page *page;
    char arr[4] = "abc";
    my_lookup_address = (lookup_address_t)kallsyms_lookup_name("lookup_address");
    
    if (!my_lookup_address) {
        pr_err("Failed to find lookup_address\n");
        return -EINVAL;
    }
    
    pr_info("Found lookup_address at %p\n", my_lookup_address);

    calling_address = (void *)kallsyms_lookup_name("iterate_dir");
    if (!calling_address)
    {
	printk(KERN_ALERT "trampoline: couldn't find symbol to hook\n");
	return -1;
    }
    printk(KERN_INFO "trampoline: Hooker address: 0x%016lX, caller address: 0x%016lX\n", (long unsigned)hooker, (long unsigned)calling_address);

    page = virt_to_page((unsigned long)calling_address);
    set_page_dirty(page);

    hooked_func_page = (unsigned long)calling_address & PAGE_MASK;
    printk(KERN_INFO "trampoline: Calculated page addr at %016lX\n", hooked_func_page);
    // if (set_memory_rw(hooked_func_page, 1)) {
    //     printk(KERN_ERR "trampoline: Failed to set page for RW\n");
    //     return -EPERM;
    // }
    
    if (make_page_writable((unsigned long)calling_address))
    {
	printk(KERN_ALERT "trampoline: Unable to make page writable\n");
	return -1;
    }
    printk(KERN_INFO "trampoline: set page addr at %016lX as RW\n", hooked_func_page);

    relative_hooker_address = (void *)hooker - (calling_address + 5);
    printk(KERN_INFO "trampoline: Writing %08X to %016lX\n", relative_hooker_address, (long unsigned)calling_address);
    printk(KERN_INFO "trampoline: calling function contains %016lX\n", *((unsigned long *)calling_address));
    // memcpy(calling_address, &call_opcode, 1);
    printk(KERN_INFO "trampoline: Wrote CALL opcode\n");
    // memcpy(calling_address + 1, &relative_hooker_address, 4);
    memcpy(calling_address, arr, 1);
    
    printk(KERN_INFO "trampoline: Hooking syscalls\n");
    return 0;
}

static void sys_hooks_exit(void)
{
    printk(KERN_INFO "trampoline: Removing hooks not implemented yet.\n");
}

module_init(sys_hooks_init);
module_exit(sys_hooks_exit);
