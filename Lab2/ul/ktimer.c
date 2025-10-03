// ul/ktimer.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <errno.h>

#define DEVICE_PATH "/dev/mytimer"
#define MAX_MSG_LEN 128
#define MAX_BUFFER_SIZE 256

/**
 * Print usage information
 */
void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s [OPTIONS]\n", prog_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -l              List active timers\n");
    fprintf(stderr, "  -s SEC MSG      Set timer for SEC seconds with message MSG\n");
    fprintf(stderr, "  -m COUNT        Set maximum number of timers (optional)\n");
    fprintf(stderr, "  -h              Show this help message\n");
}

/**
 * List active timers by reading from the device
 */
int list_timers() {
    int fd = open(DEVICE_PATH, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    
    if (bytes_read < 0) {
        perror("Failed to read from device");
        close(fd);
        return 1;
    }

    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);  // Device already includes newline
    }
    // If bytes_read == 0, no timers active, print nothing per spec

    close(fd);
    return 0;
}

/**
 * Set a timer with given seconds and message
 */
int set_timer(int seconds, const char *message) {
    if (seconds < 1 || seconds > 86400) {
        fprintf(stderr, "Error: seconds must be between 1 and 86400\n");
        return 1;
    }

    if (strlen(message) > MAX_MSG_LEN) {
        fprintf(stderr, "Error: message too long (max %d characters)\n", MAX_MSG_LEN);
        return 1;
    }

    // First, check if a timer with this message already exists
    int fd = open(DEVICE_PATH, O_RDONLY);
    bool timer_updated = false;
    
    if (fd >= 0) {
        char buffer[MAX_BUFFER_SIZE];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            
            // Parse each line to find matching message
            char *line = strtok(buffer, "\n");
            while (line) {
                char *space_pos = strrchr(line, ' ');
                if (space_pos) {
                    *space_pos = '\0';  // Null-terminate at the last space
                    
                    // Check if messages match
                    if (strcmp(line, message) == 0) {
                        timer_updated = true;
                        break;
                    }
                }
                line = strtok(NULL, "\n");
            }
        }
        close(fd);
    }

    // Now set the new timer
    fd = open(DEVICE_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    char command[MAX_BUFFER_SIZE];
    int cmd_len = snprintf(command, sizeof(command), "SET %d %s", seconds, message);
    
    ssize_t bytes_written = write(fd, command, cmd_len);
    if (bytes_written < 0) {
        if (errno == ENOSPC) {
            // Timer limit reached - need to get actual limit from somewhere
            // For now, we'll read the current state to count active timers
            close(fd);
            fd = open(DEVICE_PATH, O_RDONLY);
            if (fd >= 0) {
                char buffer[MAX_BUFFER_SIZE];
                ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
                int timer_count = 0;
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    char *line = strtok(buffer, "\n");
                    while (line) {
                        timer_count++;
                        line = strtok(NULL, "\n");
                    }
                }
                printf("%d timer(s) already exist(s)!\n", timer_count > 0 ? timer_count : 1);
                close(fd);
            } else {
                printf("1 timer(s) already exist(s)!\n");
            }
        } else {
            perror("Failed to write to device");
            close(fd);
        }
        return 1;
    }

    close(fd);
    
    // Print update message if timer was updated
    if (timer_updated) {
        printf("The timer %s was updated!\n", message);
    }
    
    return 0;
}

/**
 * Set maximum number of timers - sends COUNT command to kernel module
 */
int set_max_timers(int count) {
    if (count < 1 || count > 5) {
        fprintf(stderr, "Error: count must be between 1 and 5\n");
        return 1;
    }

    int fd = open(DEVICE_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    char command[MAX_BUFFER_SIZE];
    // Write the count value into the command string
    int cmd_len = snprintf(command, sizeof(command), "COUNT %d", count);
    // command now contains "COUNT <count>"
    
    ssize_t bytes_written = write(fd, command, cmd_len);
    if (bytes_written < 0) {
        perror("Failed to write to device");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

int main(int argc, char *argv[]) {
    int opt;
    int l_flag = 0, s_flag = 0, m_flag = 0;
    int seconds = 0, max_count = 0;
    char *message = NULL;

    // Parse command line arguments
    while ((opt = getopt(argc, argv, "ls:m:h")) != -1) {
        switch (opt) {
            // List timers
            case 'l':
                l_flag = 1;
                break;
            // Set timer
            case 's':
                s_flag = 1;
                seconds = atoi(optarg);
                // The message is the remaining arguments combined
                if (optind < argc) {
                    // Calculate total length needed for message
                    int total_len = 0;
                    for (int i = optind; i < argc; i++) {
                        total_len += strlen(argv[i]) + 1; // +1 for space
                    }
                    
                    message = malloc(total_len);
                    if (!message) {
                        fprintf(stderr, "Memory allocation failed\n");
                        return 1;
                    }
                    
                    strcpy(message, argv[optind]);
                    for (int i = optind + 1; i < argc; i++) {
                        strcat(message, " ");
                        strcat(message, argv[i]);
                    }
                } else {
                    fprintf(stderr, "Error: -s requires SEC and MSG arguments\n");
                    print_usage(argv[0]);
                    return 1;
                }
                break;
            // Set max timers (ie the number of concurrent timers, controlled by userspace application)
            case 'm':
                m_flag = 1;
                max_count = atoi(optarg);
                break;
            // Help message printing (documentation)
            case 'h':
                print_usage(argv[0]);
                return 0;
            case '?':
                print_usage(argv[0]);
                return 1;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    /* Error handling =======================================================*/
    // Check for conflicting options
    int flag_count = l_flag + s_flag + m_flag;
    if (flag_count > 1) {
        fprintf(stderr, "Error: only one option allowed at a time\n");
        print_usage(argv[0]);
        if (message) free(message);
        return 1;
    }

    if (flag_count == 0) {
        fprintf(stderr, "Error: no option specified\n");
        print_usage(argv[0]);
        return 1;
    }
    /* =====================================================================*/

    int result = 0;

    // Execute the requested operation
    if (l_flag) {
        result = list_timers();
    } else if (s_flag) {
        if (!message) {
            fprintf(stderr, "Error: message required for -s option\n");
            return 1;
        }
        result = set_timer(seconds, message);
    } else if (m_flag) {
        result = set_max_timers(max_count);
    }

    if (message) {
        free(message);
    }

    return result;
}
