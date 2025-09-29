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

# Workspace root; allow override from environment.
WORKSPACE="${WORKSPACE:-$(pwd)}"

UL_DIR="$WORKSPACE/ul"
KM_DIR="$WORKSPACE/km"
ROOTFS="$WORKSPACE/rootfs"
DST_UL="$ROOTFS/usr/src/ul"
DST_KM="$ROOTFS/usr/src/km"

echo "[info] WORKSPACE=$WORKSPACE"

# ---- Sanity checks ----
need_files=(
  "$UL_DIR/ktimer.c"
  "$UL_DIR/Makefile"
  "$KM_DIR/mytimer.c"
  "$KM_DIR/Makefile"
)
for f in "${need_files[@]}"; do
  if [[ ! -f "$f" ]]; then
    echo "[error] Missing required file: $f" >&2
  fi
done

# ---- Build modules ----
echo "[info] Building ul/"
make -C "$UL_DIR"

echo "[info] Building km/"
make -C "$KM_DIR"

# Ensure build artifacts exist
if [[ ! -f "$UL_DIR/ktimer.ko" ]]; then
  echo "[error] Build failed: $UL_DIR/ktimer.ko not found (did ul/ compile cleanly?)" >&2
fi
if [[ ! -f "$KM_DIR/mytimer.ko" ]]; then
  echo "[error] Build failed: $KM_DIR/mytimer.ko not found (did km/ compile cleanly?)" >&2
fi

# ---- Create destination dirs ----
mkdir -p "$DST_UL" "$DST_KM"

# ---- Copy contents ----
# Copy *all* contents of ul/ (sources, Makefile, .ko, etc.)
echo "[info] Syncing ul/ -> $DST_UL"
rsync -a --delete "$UL_DIR/" "$DST_UL/"

# Copy only mytimer.c and mytimer.ko from km/
echo "[info] Installing km/mytimer.c and km/mytimer.ko -> $DST_KM"
install -m 0644 "$KM_DIR/mytimer.c" "$DST_KM/"
install -m 0644 "$KM_DIR/mytimer.ko" "$DST_KM/"

echo "[ok] Done."

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
