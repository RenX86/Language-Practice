# Arch Linux Installation Guide

## Partitioning
1. Open `cfdisk` and create the following partitions:
   - Select **GPT** as the partition table.
2. Create partitions:
   - `/dev/sda1` = **300M** (EFI System Partition)
   - `/dev/sda2` = **4G** (Swap Partition)
   - `/dev/sda3` = **Remaining space** (Root Partition)

## Formatting Partitions
3. Format partitions:
   ```sh
   mkfs.ext4 /dev/sda3
   mkfs.fat -F 32 /dev/sda1
   mkswap /dev/sda2
   ```

## Mounting
4. Mount the partitions:
   ```sh
   mount /dev/sda3 /mnt
   mkdir -p /mnt/boot/efi
   mount /dev/sda1 /mnt/boot/efi
   swapon /dev/sda2
   ```

## Installing Essential Packages
5. Install the base system:
   ```sh
   pacstrap /mnt base linux linux-firmware sof-firmware base-devel grub efibootmgr nano networkmanager
   ```

## Generate fstab
6. Generate the fstab file:
   ```sh
   genfstab -U /mnt >> /mnt/etc/fstab
   cat /mnt/etc/fstab
   ```

## Chroot into the System
7. Change root into the new system:
   ```sh
   arch-chroot /mnt
   ```

## System Configuration
### Timezone
8. Set the timezone:
   ```sh
   ln -sf /usr/share/zoneinfo/Asia/Kolkata /etc/localtime
   hwclock --systohc
   ```

### Locale
9. Configure the locale:
   ```sh
   nano /etc/locale.gen  # Uncomment en_US.UTF-8
   locale-gen
   echo "LANG=en_US.UTF-8" > /etc/locale.conf
   echo "KEYMAP=us" > /etc/vconsole.conf
   ```

### Hostname & Network
10. Set hostname:
    ```sh
    echo "arch" > /etc/hostname
    ```

11. Enable NetworkManager:
    ```sh
    systemctl enable NetworkManager
    ```

### Root Password & User Creation
12. Set root password:
    ```sh
    passwd
    ```

13. Create a user:
    ```sh
    useradd -m -g users -G wheel -s /bin/bash Ren
    passwd Ren
    ```

14. Grant sudo privileges:
    ```sh
    EDITOR=nano visudo  # Uncomment %wheel ALL=(ALL) ALL
    ```

## Bootloader Installation
15. Install and configure GRUB:
    ```sh
    grub-install --target=x86_64-efi --efi-directory=/boot/efi --bootloader-id=GRUB
    grub-mkconfig -o /boot/grub/grub.cfg
    ```

## Finalizing Installation
16. Exit chroot and reboot:
    ```sh
    exit
    umount -a
    reboot
    ```

## Installing KDE Plasma (Optional)
17. Install KDE Plasma:
    ```sh
    sudo pacman -S plasma sddm kde-applications
    sudo systemctl enable --now sddm
    
