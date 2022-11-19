### Writeup 5

> Unmount the ISO

We will try to mount the ISO image locally and see what we can do with it.

```bash
mkdir /tmp/iso
mount -o loop BornToSecHackMe-v1.1.iso /tmp/iso
```

```bash
casper  install  isolinux  md5sum.txt  preseed  README.diskdefines  ubuntu
```

In the casper directory, we can find a file called `filesystem.squashfs` which is a compressed filesystem. We can extract it with `unsquashfs`:

```bash
cp filesystem.squashfs ~/Desktop
cd ~/Desktop
unsquashfs filesystem.squashfs
```

We can now see the content of the filesystem:

```bash
bin   dev  home        lib    mnt  proc  run   selinux  sys  usr  vmlinuz
boot  etc  initrd.img  media  opt  root  sbin  srv      tmp  var
```

While looking at the content of the `root` directory, we can see a file called `README.MD` containing a congratulation message

If we do an ls -laR on the root directory, we can see that there is a .bash_history file containing the password of the user `zaz`:

```bash
...
chown thor:thor README 
ls
su thor
adduser zaz
646da671ca01bb5d84dbb5fb2238dc8e
cd ../zaz
...
```
>homes filesystem should be encrypted in the ISO, but none of them is. We can easily access any content on the server without restriction. We are officially root of the machine.