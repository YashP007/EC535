# ul/Makefile — build a user-space ARM binary (not a kernel module)
CROSS ?= arm-linux-gnueabihf-
CC := $(CROSS)gcc
CFLAGS := -O2 -Wall -Wextra
# Optional: static link so it runs even on a minimal rootfs
LDFLAGS := -static

all: ktimer

ktimer: ktimer.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f ktimer
