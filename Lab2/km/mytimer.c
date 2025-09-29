
/* Necessary includes for device drivers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system_misc.h> /* cli(), *_flags */
#include <linux/uaccess.h>
#include <asm/uaccess.h> /* copy_from/to_user */

/* is a kernel module macro that specifies the license under 
which your Linux kernel module is distributed. It’s required 
for proper integration with the Linux kernel.*/
MODULE_LICENSE("Dual BSD/GPL"); 

#define MYTIMER_MAJOR 61
#define DEV_NAME "mytimer"

static int my_open(struct inode *i, struct file *f) { return 0; } // fopen
static int my_release(struct inode *i, struct file *f) { return 0; } // fclose
static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off) { return 0; } // fread
static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off) { return len; } // fwrite

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};

static int __init my_init(void) {
    printk(KERN_INFO "mytimer loaded\n");
    return register_chrdev(MYTIMER_MAJOR, DEV_NAME, &fops);
}

static void __exit my_exit(void) {
    unregister_chrdev(MYTIMER_MAJOR, DEV_NAME);
    printk(KERN_INFO "mytimer unloaded\n");
}

module_init(my_init);
module_exit(my_exit);