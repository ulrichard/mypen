#! /bin/sh

# sudo umount /media/windows
# dd if=/dev/sr1 of=windows.iso
# qemu-img create -f qcow ~/qemu/windows_2k.img 4G
# qemu-system-i386 -localtime -cdrom windows.iso -m 384 -boot d ~/qemu/windows_2k.img

sudo qemu-system-i386 -localtime -m 384 -usb -usbdevice host:0a93:0002 -net nic,model=ne2k_pci -net user ~/qemu/windows_2k.img


