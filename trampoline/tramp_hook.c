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
#define HOOKED_FUNCTION_NAME "iterate_dir"

static u8 call_opcode = 0xe8;

int make_page_writable(unsigned long address) {
    unsigned int level;
    pte_t *pte;
    
    pte = lookup_address(address, &level);
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

    calling_address = (void *)kallsyms_lookup_name(HOOKED_FUNCTION_NAME);
    if (!calling_address)
    {
	printk(KERN_ALERT "trampoline: couldn't find symbol to hook\n");
	return -1;
    }
    printk(KERN_INFO "trampoline: Hooker address: 0x%016lX, caller address: 0x%016lX\n", (long unsigned)hooker, (long unsigned)calling_address);

    if (make_page_writable((unsigned long)calling_address)) {
	printk(KERN_ALERT "trampoline: Unable to make page writable\n");
	return -1;
    }
    printk(KERN_INFO "trampoline: set memory page of %016lX as writable\n", (unsigned long)calling_address);

    relative_hooker_address = (void *)hooker - (calling_address + 5);
    printk(KERN_INFO "trampoline: calling function contains %016lX\n", *((unsigned long *)calling_address));
    printk(KERN_INFO "trampoline: Writing CALL to relative address %08X to %016lX\n", relative_hooker_address, (long unsigned)calling_address);
    memcpy(calling_address, &call_opcode, 1);
    memcpy(calling_address + 1, &relative_hooker_address, 4);
    
    printk(KERN_INFO "trampoline: Hooking calls to %s\n", HOOKED_FUNCTION_NAME);
    return 0;
}

static void sys_hooks_exit(void)
{
    printk(KERN_INFO "trampoline: Removing hooks not implemented yet.\n");
}

module_init(sys_hooks_init);
module_exit(sys_hooks_exit);
