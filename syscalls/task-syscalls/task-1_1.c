/* Export functions here */
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

static ssize_t(*real_write)(int fd, const void *buf, size_t count) = NULL;
static ssize_t(*real_read)(int fd, const void *buf, size_t count) = NULL;

ssize_t write(int fd, const void *buf, size_t count) {
    if(count > 0) {
        real_write = dlsym(RTLD_NEXT, "write");
        real_write(fd, buf, count);
    }
}

ssize_t read(int fd, const void *buf, size_t count) {
    if(count > 0) {
        real_read = dlsym(RTLD_NEXT, "read");
        real_read(fd, buf, count);
    }
}