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

