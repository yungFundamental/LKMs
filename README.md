# Linux Kernel Modules
## Introduction
A project to contain a bunch of random LKMs. Each LKM will be set in a dedicated directory with its own Makefile.

The development was done in nvchad (vim user btw), and because of this you will find `compile_commands.json` files in some of the directories for better LSP support.

## The Modules
### hello-world
Prints hello and goodbye when initialized / exitted. An introduction to LKM.

### proc-file
Creates a `/proc` entry file, called `barz.txt`.
This virtual file keeps the content in the RAM and outputs it everytime for the user.

### hook-syscall
When loaded, redirects syscalls to custom functions.
Feel free to create your own hooks!
> Note: the LKM uses `ftrace` to hook the syscalls. Changes done to the Linux Kernel in version 5.11 broke some of the methods, meaning it would have to be updated to match the more recent version. It still works on common distros like Ubuntu 20.04

### sigroot
Implements the hook-syscall on `sys_kill`.
Whenever a signal of 64 is sent, the sending process will become root user.

### log-ls
Implemented custom hooking framework. See trampoline hooks.

This module shows the trampoline module off, and simply adds a log after every `ls` call by logging the `iterate_dir` kernel function.


## Trampoline Hooks
In this project, I learned how to implement hooks.

At the beginning I used xcellerators ftrace helper. A simple header file that uses ftrace to install hooks in functions.
Later, I felt like this was cheating. Plus ftrace is easily detectable by checking /sys/kernel/tracing.

To solve this myself, I implemented my own hooking mechanism, based off trampoline hooking.
When disassembling the Linux Kernel, I noticed that the first 5 bytes of each function are reserved.
During runtime, those 5 bytes either filled with `0x0f1f440000` (a 5 byte NOP) or a CALL NEAR instruction to the ftrace logic.
> [!INFO]
> CALL NEAR is one opcode byte and 4 bytes for the relative address of the function.

Since the function will always have those 5 bytes reserved, we can replace them in runtime to a `jmp` instruction to a custom function - and that is exactly what the `trampoline` module enables us to do!
Implementing your own hooks has never been easier, and harder to detect!

In addition, hook implementers can decide when the original function should be called or not. We can just call the original function, which will run the function from the address **after** the 5 bytes changed.

### Version Proofing
The trampoline module works only for Kernel versions that don't include Intel Control-flow Enforcement Technology (CET), meaning kernel versions below 5.18. 

While the module doesn't support CET just yet, adding support should be relatively simple. All we need to do is alter the 5 bytes that are after the `endbr64` instruction.

