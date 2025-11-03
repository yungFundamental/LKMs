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

