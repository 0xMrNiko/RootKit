#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

#define LOG_FILE "/var/log/net.log" // LOG File
#define INITIAL_BUFFER_SIZE 1048576 // 1 MB Buffer Size

static int (*real_recv)(int sockfd, void *buf, size_t len, int flags) = NULL;

static char *recv_buffer = NULL;
static size_t recv_buffer_len = 0;
static size_t recv_buffer_capacity = 0;

void log_data(const char *message) {
    FILE *logfile = fopen(LOG_FILE, "a");
    if (logfile) {
        fprintf(logfile, "%s\n", message);
        fclose(logfile);
    }
}

void log_full_response() {
    if (recv_buffer_len > 0) {
        log_data(recv_buffer);
        free(recv_buffer);
        recv_buffer = NULL;
        recv_buffer_len = 0;
        recv_buffer_capacity = 0;
    }
}

void append_to_buffer(const char *data, size_t len) {
    if (recv_buffer_capacity == 0) {
        recv_buffer_capacity = INITIAL_BUFFER_SIZE;
        recv_buffer = malloc(recv_buffer_capacity);
        if (!recv_buffer) {
            return;
        }
    }

    while (recv_buffer_len + len > recv_buffer_capacity) {
        recv_buffer_capacity *= 2;
        char *new_buffer = realloc(recv_buffer, recv_buffer_capacity);
        if (!new_buffer) {
            free(recv_buffer);
            recv_buffer = NULL;
            recv_buffer_len = 0;
            recv_buffer_capacity = 0;
            return;
        }
        recv_buffer = new_buffer;
    }

    memcpy(recv_buffer + recv_buffer_len, data, len);
    recv_buffer_len += len;
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags) {
    if (!real_recv) {
        real_recv = dlsym(RTLD_NEXT, "recv");
    }

    ssize_t ret = real_recv(sockfd, buf, len, flags);

    if (ret > 0) {
        append_to_buffer(buf, ret);

        if (ret < len) {
            log_full_response();
        }
    }

    return ret;
}