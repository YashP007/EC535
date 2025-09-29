// km/mytimer.c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>   // copy_from_user, copy_to_user
#include <linux/timer.h>     // struct timer_list, timer_setup, mod_timer
#include <linux/jiffies.h>   // jiffies, HZ
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");

#define MYTIMER_MAJOR 61
#define DEV_NAME      "mytimer"
#define MAX_MSG       128

/* ---------- Forward decls ---------- */
static int     my_open   (struct inode *i, struct file *f);
static int     my_release(struct inode *i, struct file *f);
static ssize_t my_read   (struct file *f, char __user *ubuf, size_t len, loff_t *ppos);
static ssize_t my_write  (struct file *f, const char __user *ubuf, size_t len, loff_t *ppos);

/* ---------- File ops ---------- */
static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_release,
    .read    = my_read,
    .write   = my_write,
};

/* ---------- Single-timer state ---------- */
static struct timer_list my_timer;
static char  my_msg[MAX_MSG + 1];         // message printed on expiry
static bool  my_active = false;           // is a timer currently scheduled?
static unsigned long my_expires_at = 0;   // absolute jiffies when it will fire

/* ---------- Timer callback ---------- */
static void mytimer_cb(struct timer_list *t)
{
    // Spec: the KERNEL prints the message when the timer expires.
    printk(KERN_INFO "%s\n", my_msg);

    my_active = false;
    my_expires_at = 0;
}

/* ---------- Module lifecycle ---------- */
static int __init my_init(void)
{
    int ret = register_chrdev(MYTIMER_MAJOR, DEV_NAME, &fops);
    if (ret < 0) {
        pr_err("mytimer: failed to register major %d\n", MYTIMER_MAJOR);
        return ret;
    }
    timer_setup(&my_timer, mytimer_cb, 0);
    pr_info("mytimer loaded\n");
    return 0;
}

static void __exit my_exit(void)
{
    del_timer_sync(&my_timer);
    unregister_chrdev(MYTIMER_MAJOR, DEV_NAME);
    pr_info("mytimer unloaded\n");
}

/* ---------- File ops impls ---------- */
static int my_open(struct inode *i, struct file *f)
{
    return 0;
}

static int my_release(struct inode *i, struct file *f)
{
    return 0;
}

/*
 * Read format (single shot):
 *   If a timer is active, return one line: "<MSG> <SEC_REMAIN>\n"
 *   If no timer is active, return nothing (0 bytes) per the lab behavior.
 */
static ssize_t my_read(struct file *f, char __user *ubuf, size_t len, loff_t *ppos)
{
    char kbuf[256];
    int n = 0;

    if (*ppos > 0)
        return 0;  // one-shot read

    if (!my_active) {
        // No pending timers: print nothing and return 0
        return 0;
    } else {
        unsigned long now = jiffies;
        unsigned long rem_j = (my_expires_at > now) ? (my_expires_at - now) : 0;
        // round down to seconds (lab expects an integer SEC offset)
        unsigned long rem_sec = rem_j / HZ;
        n = scnprintf(kbuf, sizeof(kbuf), "%s %lu\n", my_msg, rem_sec);
    }

    if (n > len) n = len;
    if (copy_to_user(ubuf, kbuf, n))
        return -EFAULT;

    *ppos += n;
    return n;
}

/*
 * Write format (simple parser):
 *   "SET <sec> <msg...>\n"
 *     - schedules a timer <sec> seconds from now that will printk("<msg>")
 *
 * Notes:
 *   - This is the minimal path to get a working timer.
 *   - You’ll extend this to handle "update if same MSG", -m, etc.
 */
static ssize_t my_write(struct file *f, const char __user *ubuf, size_t len, loff_t *ppos)
{
    char kbuf[256];
    unsigned long sec = 0;
    int i, mstart = -1;

    if (len >= sizeof(kbuf))
        len = sizeof(kbuf) - 1;

    if (copy_from_user(kbuf, ubuf, len))
        return -EFAULT;
    kbuf[len] = '\0';

    //
