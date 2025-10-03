// km/mytimer.c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>   // copy_from_user, copy_to_user
#include <linux/timer.h>     // struct timer_list, timer_setup, mod_timer
#include <linux/jiffies.h>   // jiffies, HZ
#include <linux/string.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/slab.h>

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

/* ---------- Multiple Timer management ---------- */
struct mytimer_item {
    struct timer_list t;
    char msg[MAX_MSG + 1];
    bool active;
    u64 expires_jiffies;
    struct list_head node;
};

static LIST_HEAD(timer_list_head);
static DEFINE_SPINLOCK(timer_lock);
static int max_timers = 1;  // default, changeable via -m
static int active_count = 0;

/* ---------- Timer callback ---------- */
static void mytimer_cb(struct timer_list *t)
{
    struct mytimer_item *it = from_timer(it, t, t);

    // Print the message from kernel context
    printk(KERN_INFO "%s\n", it->msg);

    // Mark as inactive and free
    spin_lock_bh(&timer_lock);
    if (it->active) {
        it->active = false;
        active_count--;
        list_del(&it->node);
        kfree(it);
    }
    spin_unlock_bh(&timer_lock);
}

/* ---------- Module lifecycle ---------- */
static int __init my_init(void)
{
    int ret = register_chrdev(MYTIMER_MAJOR, DEV_NAME, &fops);
    if (ret < 0) {
        printk(KERN_ERR "mytimer: failed to register major %d\n", MYTIMER_MAJOR);
        return ret;
    }
    printk(KERN_INFO "mytimer loaded\n");
    return 0;
}

static void __exit my_exit(void)
{
    struct mytimer_item *it, *tmp;
    
    spin_lock_bh(&timer_lock);
    list_for_each_entry_safe(it, tmp, &timer_list_head, node) {
        del_timer_sync(&it->t);
        list_del(&it->node);
        kfree(it);
    }
    active_count = 0;
    spin_unlock_bh(&timer_lock);
    
    unregister_chrdev(MYTIMER_MAJOR, DEV_NAME);
    printk(KERN_INFO "mytimer unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

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
 * Read format: List all active timers
 * Each line: "<MSG> <SEC_REMAIN>\n"
 * If no timers active, return nothing (0 bytes)
 */
static ssize_t my_read(struct file *f, char __user *ubuf, size_t maxlen, loff_t *ppos)
{
    char *out;
    size_t n = 0;
    struct mytimer_item *it;

    if (*ppos > 0)
        return 0;  // one-shot read

    out = kmalloc(1024, GFP_KERNEL);
    if (!out)
        return -ENOMEM;

    // Snapshot the timer data under lock
    spin_lock_bh(&timer_lock);
    list_for_each_entry(it, &timer_list_head, node) {
        if (!it->active)
            continue;
        long ticks = (long)it->t.expires - (long)jiffies;
        if (ticks < 0)
            ticks = 0;
        long sec = ticks / HZ;
        if (sec < 0) 
            sec = 0;
        
        n += scnprintf(out + n, 1024 - n, "%s %ld\n", it->msg, sec);
        if (n >= 1024 - 128)  // leave room for one more entry
            break;
    }
    spin_unlock_bh(&timer_lock);

    // Now do copy_to_user outside the lock
    if (n == 0) {
        kfree(out);
        return 0;  // print nothing if no timers
    }

    if (n > maxlen)
        n = maxlen;

    if (copy_to_user(ubuf, out, n)) {
        kfree(out);
        return -EFAULT;
    }
    
    *ppos = n;
    kfree(out);
    return n;
}

/*
 * Helper function to set or update a timer
 * Returns: 1 if updated existing, 0 if created new, -ENOMEM if no memory, -ENOSPC if full
 */
static int set_or_update_timer(int sec, const char *msg)
{
    struct mytimer_item *it;
    u64 expires = get_jiffies_64() + (u64)sec * HZ;

    spin_lock_bh(&timer_lock);

    // Check if timer with same message already exists
    list_for_each_entry(it, &timer_list_head, node) {
        if (it->active && strncmp(it->msg, msg, MAX_MSG) == 0) {
            // Update existing timer
            it->expires_jiffies = expires;
            mod_timer(&it->t, (unsigned long)expires);
            spin_unlock_bh(&timer_lock);
            return 1;  // updated
        }
    }

    // Check if we have room for a new timer
    if (active_count >= max_timers) {
        spin_unlock_bh(&timer_lock);
        return -ENOSPC;  // full
    }

    // Create new timer
    it = kzalloc(sizeof(*it), GFP_KERNEL);
    if (!it) {
        spin_unlock_bh(&timer_lock);
        return -ENOMEM;
    }

    strncpy(it->msg, msg, MAX_MSG);
    it->msg[MAX_MSG] = '\0';
    it->active = true;
    it->expires_jiffies = expires;
    
    timer_setup(&it->t, mytimer_cb, 0);
    mod_timer(&it->t, (unsigned long)expires);
    
    list_add_tail(&it->node, &timer_list_head);
    active_count++;

    spin_unlock_bh(&timer_lock);
    return 0;  // created new
}

/*
 * Write handler - parse commands:
 *   "SET <sec> <msg...>"  - set/update timer
 *   "COUNT <num>"         - set max timer count
 */
static ssize_t my_write(struct file *f, const char __user *ubuf, size_t len, loff_t *ppos)
{
    char kbuf[256];
    unsigned long sec = 0;
    int count = 0;
    char *msg_start;
    int ret;

    if (len >= sizeof(kbuf))
        len = sizeof(kbuf) - 1;

    if (copy_from_user(kbuf, ubuf, len))
        return -EFAULT;
    kbuf[len] = '\0';

    // Remove trailing newline if present
    char *newline = strchr(kbuf, '\n');
    if (newline)
        *newline = '\0';

    if (strncmp(kbuf, "SET ", 4) == 0) {
        // Parse "SET <sec> <msg...>"
        ret = sscanf(kbuf + 4, "%lu", &sec);
        if (ret != 1)
            return -EINVAL;

        // Find the message part (after the second space)
        msg_start = strchr(kbuf + 4, ' ');
        if (!msg_start)
            return -EINVAL;
        msg_start++; // skip the space

        ret = set_or_update_timer(sec, msg_start);
        if (ret == 1) {
            // Timer was updated - this should be handled by userspace
            // But we need to signal success
        } else if (ret == -ENOSPC) {
            // This should be handled by userspace too
            return -ENOSPC;
        } else if (ret < 0) {
            return ret;
        }
        // ret == 0: new timer created successfully

    } else if (strncmp(kbuf, "COUNT ", 6) == 0) {
        // Parse "COUNT <num>"
        ret = sscanf(kbuf + 6, "%d", &count);
        if (ret != 1 || count < 1 || count > 5)
            return -EINVAL;

        spin_lock_bh(&timer_lock);
        max_timers = count;
        spin_unlock_bh(&timer_lock);

    } else {
        return -EINVAL;
    }

    return len;
}