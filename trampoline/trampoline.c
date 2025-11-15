#include <linux/kallsyms.h>
#include <asm/pgtable.h>
#include <linux/types.h>
#include "trampoline.h"


static int make_page_writable(unsigned long address) {
    unsigned int level;
    pte_t *pte;
    
    pte = lookup_address(address, &level);
    if (!pte) 
	    return -EINVAL;
    
    set_pte_atomic(pte, pte_mkwrite(*pte));
    __flush_tlb_one(address);
    return 0;
}

static int make_page_readonly(unsigned long address) {
    unsigned int level;
    pte_t *pte;
    
    pte = lookup_address(address, &level);
    if (!pte)
        return -EINVAL;
    
    // Clear the writable bit
    set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));
    __flush_tlb_one(address);
    return 0;
}

int tramp_hook_install(hook_t *hook)
{
    u32 relative_hooker_address;
    void *target;

    target = (void *)kallsyms_lookup_name(hook->target_name);
    if (!target) {
        printk(KERN_ALERT "Failed looking for symbol %s while installing hook", hook->target_name);
        return -EINVAL;
    }
    make_page_writable(target)


    hook->original_function = target + 0 // TODO Add bytes added
    return 0;
}

