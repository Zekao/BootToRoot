### Writeup 4

> Dirtycow exploit (https://github.com/dirtycow/dirtycow.github.io/wiki/PoCs)

Dirtycow is a computer security vulnerability of the Linux kernel that affected all Linux-based operating systems, including Android devices, that used older versions of the Linux kernel created before 2018. It is a local privilege escalation bug that exploits a race condition in the implementation of the copy-on-write mechanism in the kernel's memory-management subsystem. Computers and devices that still use the older kernels remain vulnerable

### Usage

You will have to compile `dirtycow.c` in any session of the virtual machine, by example you can use it with zaz:

ssh zaz@192.168.56.101
646da671ca01bb5d84dbb5fb2238dc8e

```bash
gcc -pthread dirtycow.c -lcrypt; ./a.out
```
You will have to wait a little bit and now you're able to add a new password for the user that will be created

And now, you have a root access :)
