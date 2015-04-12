/*
 * =====================================================================================
 *
 *       Filename:  sbd.c
 *
 *    Description:  sbd 内核模块
 *
 *        Version:  1.0
 *        Created:  2014年12月26日 12时27分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Qianyi.lh (liuhuan), qianyi.lh@alibaba-inc.com
 *        Company:  Alibaba-Inc Aliyun
 *
 * =====================================================================================
 */

#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>

#define DP_MAJOR 244
#define DP_MINOR 10

static int chropen;
struct cdev *chardev;
static int len;

static int char_read(struct file *, char __user *, size_t, loff_t *);
static int char_open(struct inode *, struct file *);
static int char_write(struct file *, const char __user *, size_t , loff_t *);
static int char_release(struct inode *, struct file *);

static const struct file_operations char_ops = {
        .read = char_read,
        .write = char_write,
        .open = char_open,
        .release = char_release,
};

static int char_open(struct inode *inode, struct file *file)
{
        if (chropen == 0) {
                chropen++;
        } else {
                printk(KERN_ALERT "Another process open the char device\n");
                return -1;
        }

        try_module_get(THIS_MODULE);

        return 0;
}

static int char_release(struct inode *inode, struct file *file)
{
        chropen--;
        module_put(THIS_MODULE);

        return 0;
}

static int char_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset)
{
        return 0;
}

static int char_write(struct file *filp, const char __user *buffer, size_t length, loff_t *offset)
{
        char buff[128];

        if (copy_from_user(buff, buffer, length)) {
                printk("copy form user error");
                return -1;
        }

        buff[length] = '\0';

        printk("From User: %s\n", buff);

        return 0;
}

static int __init sbd_init(void)
{
        dev_t dev;
        dev = MKDEV(DP_MAJOR, DP_MINOR);
        chardev = cdev_alloc();

        if (chardev == NULL) {
                return -1;
        }

        if (register_chrdev_region(dev, 10, "sbd")) {
                printk(KERN_ALERT "Register char dev error\n");
                return -1;
        }

        chropen = 0;
        len = 0;
        cdev_init(chardev, &char_ops);
        if (cdev_add(chardev, dev, 1)) {
                printk(KERN_ALERT"Add char dev error\n");
        }

        return 0;
}

static void __exit sbd_exit(void)
{
        len = 0;
        unregister_chrdev_region(MKDEV(DP_MAJOR, DP_MINOR), 10);
        cdev_del(chardev);
}

module_init(sbd_init);
module_exit(sbd_exit);

MODULE_AUTHOR("qianyi.lh");
MODULE_LICENSE("GPL");

