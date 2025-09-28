cd $WORKSPACE
dd if=/dev/zero of=rootfs.img bs=1M count=27
mkfs.ext4 -L rootfs rootfs.img 

mkdir -p /tmp/$USER-mnt
guestmount -a rootfs.img -m /dev/sda /tmp/$USER-mnt
cp -a --no-preserve=ownership $WORKSPACE/rootfs/* /tmp/$USER-mnt/
guestunmount /tmp/$USER-mnt/

virt-ls -la rootfs.img -m /dev/sda /
