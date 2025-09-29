#!/bin/bash
# EC535 rootfs.img Rebuilding Script
# Provided with stock-rootfs.tar.gz
ROOTFS_SIZE=256

# Ensure WORKSPACE is set
if [ -z "$WORKSPACE" ]; then
    echo "Error: WORKSPACE variable must be set"
    exit 1
fi

# Ensure mountpoint is empty
if [ "$(ls -A /tmp/$USER-mnt)" ]; then
    echo "Error: /tmp/$USER-mnt must be empty in order to be used as a mountpoint for your rootfs.img."
    echo "Make sure a filesystem is not already mounted there, and then run 'rm -rf /tmp/$USER-mnt'."
    exit 1
fi

// Check for the existance of of the following files, run Makefile, and then copy into rootfs/usr/src/km and rootfs/usr/src/ul
// Files to check for: ul/ktimer.c, ul/Makefile, km/mytimer.c, km/Makefile
if [ "$WORKSPACE/ul/ktimer.c" ] && [ "$WORKSPACE/ul/Makefile" ] && [ "$WORKSPACE/km/Makefile" ] && [ "$WORKSPACE/km/mytimer.c" ]; then
    echo "All required files exist. Running Makefile..."
    make -C $WORKSPACE/ul
    make -C $WORKSPACE/km

    echo "Copying compiled files into rootfs..."
    cp $WORKSPACE/ul/ktimer.ko $WORKSPACE/rootfs/usr/src/ul/
    cp $WORKSPACE/km/mytimer.ko $WORKSPACE/rootfs/usr/src/km/
else
    echo "Error: One or more required files are missing."
    echo "Please ensure the following files exist in your WORKSPACE directory:"
    echo "- ul/ktimer.c"
    echo "- ul/Makefile"
    echo "- km/mytimer.c"
    echo "- km/Makefile"
    exit 1
fi
if [ "$WORKSPACE/km/mytimer.c" ]; then
    echo "All required files exist. Running Makefile..."
    make -C $WORKSPACE/ul
    make -C $WORKSPACE/km

    echo "Copying compiled files into rootfs..."
    mkdir -p $WORKSPACE/rootfs/usr/ul/
    mkdir -p $WORKSPACE/rootfs/usr/km/
    cp $WORKSPACE/ul/ $WORKSPACE/rootfs/usr/ul/ 
    cp $WORKSPACE/km/ $WORKSPACE/rootfs/usr/km/ 
else
    echo "Error: One or more required files are missing."
    echo "Please ensure the following files exist in your WORKSPACE directory:"
    echo "- ul/ktimer.c"
    echo "- ul/Makefile"
    echo "- km/mytimer.c"
    echo "- km/Makefile"
    exit 1
fi

# Don't do anything unless a rootfs folder exists in the working dir
if [ -d "${WORKSPACE}/rootfs" ]; then
    # Delete rootfs.img if it exists
    if [ -e "rootfs.img" ]; then
        echo "Deleting old rootfs.img..."
        rm rootfs.img
    fi

    # Create a blank 128MB disk image formatted as ext4
    echo "Creating blank ${ROOTFS_SIZE}MB rootfs.img..."
    dd if=/dev/zero of=rootfs.img bs=1M count=$ROOTFS_SIZE
    echo "Formatting rootfs.img as ext4..."
    yes | mkfs.ext4 -L rootfs rootfs.img

    # Mount disk image and copy in files
    echo "Mounting rootfs.img on /tmp/${USER}-mnt..."
    mkdir -p /tmp/$USER-mnt
    guestmount -a rootfs.img -m /dev/sda /tmp/$USER-mnt
    echo "Copying files..."
    cp -a --no-preserve=ownership $WORKSPACE/rootfs/* /tmp/$USER-mnt/
    echo "Unmounting rootfs.img..."
    guestunmount /tmp/$USER-mnt/

    # List files so user can confirm copy completed successfully
    virt-ls -la rootfs.img -m /dev/sda /
    echo "All done!"
else
    echo "Error: rootfs does not exist in your WORKSPACE directory"
fi
