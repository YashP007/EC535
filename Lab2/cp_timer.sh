# Workspace root; allow override from environment.
: "${WORKSPACE:=$(pwd)}"
UL_DIR="$WORKSPACE/ul"
KM_DIR="$WORKSPACE/km"
ROOTFS="$WORKSPACE/rootfs"
DST_UL="$ROOTFS/usr/ul"
DST_KM="$ROOTFS/usr/km"

# ---- Build modules ----
make -C "$UL_DIR"
make -C "$KM_DIR"

# ---- Create destination dirs ----
mkdir -p "$DST_UL" "$DST_KM"

# ---- Copy contents ----
# Copy *all* contents of ul/ (sources, Makefile, .ko, etc.)
echo "[info] Copying ul/ -> $DST_UL"
cp "$UL_DIR/Makefile" "$DST_UL/"
cp "$UL_DIR/ktimer.c" "$DST_UL/"
cp "$UL_DIR/ktimer" "$DST_UL/"

# Copy only mytimer.c and mytimer.ko from km/
echo "[info] Copying km/mytimer.c and km/mytimer.ko -> $DST_KM"
cp "$KM_DIR/Makefile" "$DST_KM/"
cp "$KM_DIR/mytimer.c" "$DST_KM/"
cp "$KM_DIR/mytimer.ko" "$DST_KM/"