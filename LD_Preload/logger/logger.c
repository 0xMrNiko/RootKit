#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <time.h>
#include <pwd.h>

#define LOG_FILE_PATH "/var/log/logger.log" // Path to the log file

static int (*original_execve)(const char *pathname, char *const argv[], char *const envp[]) = NULL;
static int (*original_execveat)(int dirfd, const char *pathname, char *const argv[], char *const envp[], int flags) = NULL;

const char *get_command_name(const char *pathname) {
    const char *command = strrchr(pathname, '/');
    if (command) {
        return command + 1;  
    }
    return pathname;  
}

const char *get_username() {
    const char *user = getenv("USER");
    if (user == NULL) {
        struct passwd *pw = getpwuid(geteuid());
        if (pw != NULL) {
            user = pw->pw_name;
        } else {
            user = "unknown";
        }
    }
    return user;
}

void log_command(const char *pathname, char *const argv[]) {
    FILE *logfile = fopen(LOG_FILE_PATH, "a");
    if (logfile != NULL) {
        time_t rawtime;
        struct tm *timeinfo;
        char time_buffer[80];

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", timeinfo);

        const char *user = get_username();

        const char *tty = ttyname(STDIN_FILENO);
        if (tty == NULL) {
            tty = "N/A";
        }

        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            strcpy(cwd, "N/A");
        }

        const char *bin_path = pathname;

        fprintf(logfile, "Mr.Niko %s [TIME: %s] | [TTY: %s] | [CWD: %s] | [BIN: %s] ", user, time_buffer, tty, cwd, bin_path);

        const char *command_name = get_command_name(pathname);
        fprintf(logfile, "%s ", command_name);

        for (int i = 1; argv[i] != NULL; i++) {
            fprintf(logfile, "%s ", argv[i]);
        }

        fprintf(logfile, "\n");
        fclose(logfile);
    }
}

int execve(const char *pathname, char *const argv[], char *const envp[]) {
    if (original_execve == NULL) {
        original_execve = dlsym(RTLD_NEXT, "execve");
    }

    log_command(pathname, argv);
    return original_execve(pathname, argv, envp);
}

int execveat(int dirfd, const char *pathname, char *const argv[], char *const envp[], int flags) {
    if (original_execveat == NULL) {
        original_execveat = dlsym(RTLD_NEXT, "execveat");
    }

    log_command(pathname, argv);
    return original_execveat(dirfd, pathname, argv, envp, flags);
}