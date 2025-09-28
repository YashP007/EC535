cd $WORKSPACE
source /ad/eng/courses/ec/ec535/bashrc_ec535
ln -sf linux-4.19.82-ti-rt-r33/arch/arm/boot/zImage ./
file zImage 

qemu-system-arm -M virt -m 512 -kernel zImage -nographic \
  -drive file=rootfs.img,if=none,format=raw,id=rfs \
  -device virtio-blk-device,drive=rfs \
  -append "earlyprintk=serial root=/dev/vda"
