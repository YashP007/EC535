// ul/ktimer.c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    // What is the purpose of this code?
    // This code is a user-space program that opens the mytimer device
    // and checks for errors in opening the device.
    int fd = open("/dev/mytimer", O_RDWR);
    if (fd < 0) { // Error handling
        perror("open");
        printf("Failed to open /dev/mytimer\n\tPlease ensure that the mytimer module is loaded.\n");
        return 1;
    }
    printf("Opened /dev/mytimer successfully!\n");
    close(fd);
    return 0;
}
