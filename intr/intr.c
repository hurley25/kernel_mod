/*
 * =====================================================================================
 *
 *       Filename:  intr.c
 *
 *    Description:  intr 内核模块
 *
 *        Version:  1.0
 *        Created:  2015年01月04日 15时27分48秒
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
#include <linux/interrupt.h>
#include <linux/sched.h>

static int irq;
static char *interface;

module_param(interface, charp, 0644);
module_param(irq, int, 0644);

static irqreturn_t irq_handler(int intrno, void *args)
{
        printk("PID = %d Name = %s\n", current->pid, current->comm);
        return 0;
}

static int __init intr_init(void)
{
        if (request_irq(irq, &irq_handler, IRQF_SHARED, interface, &irq)) {
                printk(KERN_ERR "intr: cannot register IRQ %d\n", irq);
                return -EIO;
        }

        printk("%s Request on IRQ %d succeeded\n", interface, irq);

	return 0;
}

static void __exit intr_exit(void)
{
        free_irq(irq, &irq);
        printk("Freeing IRQ %d\n", irq);
}

module_init(intr_init);
module_exit(intr_exit);

MODULE_AUTHOR("qianyi.lh");
MODULE_LICENSE("GPL");

