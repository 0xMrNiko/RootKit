#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/file.h>
#include <linux/dcache.h>
#include <linux/limits.h>
#include <linux/string.h>
#include <asm/unistd.h>
#include <linux/kallsyms.h>
#include <linux/namei.h>
#include <linux/tty.h>
#include <linux/sched.h>
#include <linux/time.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mr.Niko");
MODULE_DESCRIPTION("LKM Logging Module");

#define LOG_FILE "/var/log/logger.log"
#define LOG_SIZE 4096
#define MAX_ARG_STRLEN 4096
#define CWD_SIZE PATH_MAX


unsigned long **sys_call_table;

asmlinkage long (*orig_execve)(const char __user *filename,
                               const char __user *const __user *argv,
                               const char __user *const __user *envp);

static void get_cwd_string(char *buf, size_t size) {
    struct path cwd_path;
    char *cwd_buffer = kmalloc(size, GFP_KERNEL);

    if (cwd_buffer) {
        if (kern_path(".", LOOKUP_FOLLOW, &cwd_path) == 0) {
            snprintf(cwd_buffer, size, "[CWD: %s]", d_path(&cwd_path, cwd_buffer, size));
            strncpy(buf, cwd_buffer, size);
        } else {
            snprintf(buf, size, "[CWD: N/A]");
        }
        kfree(cwd_buffer);
    } else {
        snprintf(buf, size, "[CWD: N/A]");
    }
}

static void get_tty_string(char *buf, size_t size) {
    struct tty_struct *tty = get_current_tty();
    
    if (tty && tty->driver) {
        snprintf(buf, size, "[TTY: /dev/pts/%s]", tty->name + 3);
    } else {
        snprintf(buf, size, "[TTY: N/A]");
    }
}

static void get_current_time_string(char *buf, size_t size) {
    struct timespec ts;
    struct tm tm;

    getnstimeofday(&ts);
    time_to_tm(ts.tv_sec, 0, &tm);

    snprintf(buf, size, "[TIME: %02d:%02d:%02d]", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

static void log_command(const char *command, const char *args, const char *cwd, const char *tty) {
    struct file *file;
    mm_segment_t old_fs;
    loff_t pos = 0;
    char *log_entry;
    char time_str[32];

    log_entry = kmalloc(LOG_SIZE, GFP_KERNEL);

    if (!log_entry) {
        return;
    }

    get_current_time_string(time_str, sizeof(time_str));

    snprintf(log_entry, LOG_SIZE, "Mr.Niko %s | %s | %s | [BIN: %s] %s\n",
             time_str, tty, cwd, command, args ? args : "N/A");

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    file = filp_open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (!IS_ERR(file)) {
        vfs_write(file, log_entry, strlen(log_entry), &pos);
        filp_close(file, NULL);
    }
    set_fs(old_fs);

    kfree(log_entry);
}

static asmlinkage long hook_execve(const char __user *filename,
                                   const char __user *const __user *argv,
                                   const char __user *const __user *envp) {
    char *kernel_filename;
    char *args = NULL;
    char *cwd = NULL;
    char *tty = NULL;
    int i;

    kernel_filename = kmalloc(PATH_MAX, GFP_KERNEL);
    if (!kernel_filename)
        return -ENOMEM;

    cwd = kmalloc(CWD_SIZE, GFP_KERNEL);
    if (!cwd) {
        kfree(kernel_filename);
        return -ENOMEM;
    }

    tty = kmalloc(PATH_MAX, GFP_KERNEL);
    if (!tty) {
        kfree(kernel_filename);
        kfree(cwd);
        return -ENOMEM;
    }

    if (strncpy_from_user(kernel_filename, filename, PATH_MAX) < 0) {
        kfree(kernel_filename);
        kfree(cwd);
        kfree(tty);
        return -EFAULT;
    }

    get_cwd_string(cwd, CWD_SIZE);
    get_tty_string(tty, PATH_MAX);

    if (argv) {
        args = kmalloc(MAX_ARG_STRLEN, GFP_KERNEL);
        if (args) {
            args[0] = '\0';
            for (i = 0; argv[i]; i++) {
                char *arg = kmalloc(MAX_ARG_STRLEN, GFP_KERNEL);
                if (!arg)
                    break;
                if (strncpy_from_user(arg, argv[i], MAX_ARG_STRLEN) >= 0) {
                    strcat(args, arg);
                    strcat(args, " ");
                }
                kfree(arg);
            }
        }
    }

    log_command(kernel_filename, args, cwd, tty);

    kfree(kernel_filename);
    kfree(args);
    kfree(cwd);
    kfree(tty);

    return orig_execve(filename, argv, envp);
}

static void disable_write_protection(void) {
    write_cr0(read_cr0() & (~0x10000));
}

static void enable_write_protection(void) {
    write_cr0(read_cr0() | 0x10000);
}

static int __init lkm_init(void) {

    sys_call_table = (unsigned long **)kallsyms_lookup_name("sys_call_table");
    if (!sys_call_table) {
        return -EFAULT;
    }

    orig_execve = (void *)sys_call_table[__NR_execve];

    disable_write_protection();
    sys_call_table[__NR_execve] = (unsigned long *)hook_execve;
    enable_write_protection();

    return 0;
}

static void __exit lkm_exit(void) {

    if (sys_call_table) {
        disable_write_protection();
        sys_call_table[__NR_execve] = (unsigned long *)orig_execve;
        enable_write_protection();
    }
}

module_init(lkm_init);
module_exit(lkm_exit);
