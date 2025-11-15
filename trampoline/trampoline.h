#ifndef TRAMPOLINE_HOOKS
#define TRAMPOLINE_HOOKS

typedef struct hook {
    const char *target_name;
    void *function;
    void *original_function;

}hook_t;


int tramp_hook_install(hook_t *hook);
int tramp_hook_uninstall(hook_t *hook);


#endif // !TRAMPOLINE_HOOKS
#define TRAMPOLINE_HOOKS

